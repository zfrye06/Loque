
#include "loqueclient.h"

sf::Packet& operator<<(sf::Packet& packet, const UserType& ut) {
    packet << static_cast<int>(ut);
    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, UserType& ut) {
    int i;
    packet >> i;
    ut = static_cast<UserType>(i);
    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, LoginResult& res) {
    packet >> res.userId >> res.userType;
    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const GameStats& stats) {
    packet << stats.levelId << stats.secToComplete << stats.pointsScored;
    return packet;
}

LoqueClient::LoqueClient(const std::string& host, int port) : host(host), port(port) {}

Status LoqueClient::attemptLogin(const std::string& username,
                                 const std::string& userpass,
                                 LoginResult& result) {
    sf::Packet toSend;
    toSend << "LOGIN" << username << userpass;
    auto status = conn.send(toSend);
    if (!ok(status)) {
        return status;
    }
    sf::Packet toReceive;
    status = conn.receive(toReceive);
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
    toSend << "CREATE-ACC" << type << username << userpass;
    auto status = conn.send(toSend);
    if (!ok(status)) {
        return status;
    }
    sf::Packet toReceive;
    status = conn.receive(toReceive);
    if (!ok(status)) {
        return status;
    }
    toReceive >> result;
    return status;
}

Status LoqueClient::postGameStats(int userId, const GameStats& stats) {
    sf::Packet toSend;
    toSend << "POST-STATS" << stats;
    auto status = conn.send(toSend);
    if (!ok(status)) {
        return status;
    }
    sf::Packet toReceive;
    status = conn.receive(toReceive);
    return status;
}
