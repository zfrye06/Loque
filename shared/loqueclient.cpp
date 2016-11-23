
#include "loqueclient.h"
#include "json.hpp"

using json = nlohmann::json;

void jsonFromPacket(sf::Packet& p, json& jobj) {
    std::string raw;
    p >> raw;
    jobj = json::parse(raw); 
}

UserType parseUserType(const std::string& s) {
    if (s == "ADMIN") return UserType::Admin;
    if (s == "STUDENT") return UserType::Student;
    if (s == "DNE") return UserType::DNE;
    throw std::invalid_argument("Unrecognized user type: " + s);
}

void makeLoginRequest(const std::string& username,
                      const std::string& userpass,
                      sf::Packet& request) {
    json req;
    req["request-type"] = "LOGIN";
    req["user-name"] = username;
    req["user-pass"] = userpass;
    request << req.dump();
}

void parseLoginResult(sf::Packet& res, LoginResult& result) {
    json resp;
    jsonFromPacket(res, resp); 
    result.userId = *resp.find("user-id");
    result.userType = parseUserType(*resp.find("user-type"));
}

bool LoginResult::successful() const {
    return userType != UserType::DNE;
}

LoqueClient::LoqueClient(const std::string& host, int port) : host(host), port(port) {}

status LoqueClient::attemptLogin(const std::string& username,
                                             const std::string& userpass,
                                             LoginResult& result) {
    sf::Packet outgoing, incoming;
    makeLoginRequest(username, userpass, outgoing);
    auto status = sendRequest(outgoing, incoming);
    if (status != sf::Socket::Done) {
        return status;
    }
    parseLoginResult(incoming, result);
    return status;
}

status LoqueClient::sendRequest(sf::Packet& req, sf::Packet& resp) {
    auto status = conn.connect(host, port);
    conn.disconnect();
    return status;
}
