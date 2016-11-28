
#include "loqueclient.h"
#include "serialization.h"

LoqueClient::LoqueClient(const std::string& host, int port) : host(host), port(port) {}

Status LoqueClient::attemptLogin(const std::string& username,
                                 const std::string& userpass,
                                 LoginResult& result) {
    sf::Packet toSend;
    toSend << ReqType::LOGIN << username << userpass;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
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
    toSend << ReqType::CREATE_ACC <<  username << userpass << type;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (!ok(status)) {
        return status;
    }
    toReceive >> result;
    return status;
}

Status LoqueClient::addClassroom(int userId, int classId, ActionResult& result) {
    sf::Packet toSend;
    toSend << ReqType::ADD_CLASS << userId << classId;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (!ok(status)) {
        return status;
    }
    toReceive >> result;
    return status;
}

Status LoqueClient::createClassroom(int userId, const std::string& className, ActionResult& result) {
    sf::Packet toSend;
    toSend << ReqType::CREATE_CLASS << userId << className;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, to Receive);
    if (!ok(status)) {
        return status;
    }
    toReceive >> result;
    return status;
}

Status LoqueClient::postGameStats(int userId, const GameStats& stats, ActionResult& result) {
    sf::Packet toSend;
    toSend << ReqType::POST_STATS << userId << stats;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (!ok(status)) {
        return status;
    }
    toReceive >> result;
    return status;
}

Status LoqueClient::getUserStats(int userId, UserStats& stats) {
    sf::Packet toSend;
    toSend << ReqType::GET_USER_STATS << userId;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (!ok(status)) {
        return status;
    }
    toReceive >> stats;
    return status;
}

Status LoqueClient::getEnabledLevels(int userId, std::vector<int>& levelIds) {
    sf::Packet toSend;
    toSend << ReqType::GET_ENABLED_LEVELS << userId;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (!ok(status)) {
        return status;
    }
    packet >> levelIds;
    return status; 
}

Status LoqueClient::enableLevel(int userId, int classId, int levelId, ActionResult& result) {
    sf::Packet toSend;
    toSend << ReqType::ENABLE_LEVEL << userId << classId << levelId;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (!ok(status)) {
        return status;
    }
    toReceive >> result;
    return status;
}

Status LoqueClient::disableLevel(int userId, int classId, int levelId, ActionResult& result) {
    sf::Packet toSend;
    toSend << ReqType::DISABLE_LEVEL << userId << classId << levelId;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (!ok(status)) {
        return status;
    }
    toReceive >> result;
    return status;
}

Status LoqueClient::getClassStats(int userId, int classId, ClassStats& stats) {
    sf::Packet toSend;
    toSend << ReqType::GET_CLASS_STATS << userId << classId;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (!ok(status)) {
        return status;
    }
    toReceive >> stats;
    return status;
}

Status LoqueClient::makeRequest(sf::Packet& request, sf::Packet &response) {
    auto status = conn.connect(host, port);
    if (!ok(status)) {
        return status;
    }
    status = conn.send(request);
    if (!ok(status)) {
        return status;
    }
    status = conn.receive(response);
    return status;
}
