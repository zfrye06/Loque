
#include "loqueclient.h"
#include "serialization.h"

LoqueClient::LoqueClient(const std::string& host, int port) : host(host), port(port) {}

Status LoqueClient::attemptLogin(const std::string& username,
                                 const std::string& userpass,
                                 LoginResult& result) {
    sf::Packet toSend;
    toSend << ReqType::LOGIN << username << userpass;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, &toReceive);
    if (!ok(status)) {
        return status;
    }
    toReceive >> result;
    return status;
}

Status LoqueClient::createAccount(const std::string& username,
                                  const std::string& userpass,
                                  UserType type,
                                  LoginResult& result) {
    sf::Packet toSend;
    toSend << ReqType::CREATE_ACC << type << username << userpass;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, &toReceive);
    if (!ok(status)) {
        return status;
    }
    toReceive >> result;
    return status;
}

Status LoqueClient::postGameStats(int userId, const GameStats& stats) {
    sf::Packet toSend;
    toSend << ReqType::POST_STATS << userId << stats;
    return makeRequest(toSend, nullptr);
}

Status LoqueClient::makeRequest(sf::Packet& request, sf::Packet *response) {
    auto status = conn.connect(host, port);
    if (!ok(status)) {
        return status;
    }
    status = conn.send(request);
    if (!status || response == nullptr) {
        return status;
    }
    status = conn.receive(*response);
    return status;
}
