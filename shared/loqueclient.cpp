
#include "loqueclient.h"
#include "serialization.h"

std::ostream& operator<<(std::ostream& out, Status s) {
    switch (s) {
    case Status::OK:
        out << "OK";
        break;
    case Status::NETWORK_ERR:
        out << "NETWORK_ERR: Unable to access server.";
        break;
    case Status::DB_ERR:
        out << "DB_ERR: Unable to complete requested query." <<
            " Try checking the server logs for more information.";
        break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, UserType t) {
    switch (t) {
    case UserType::ADMIN:
        out << "UserType<Admin>";
        break;
    case UserType::STUDENT:
        out << "UserType<Student>";
        break;
    case UserType::DNE:
        out << "UserType<DNE>";
        break;
    }
    return out;
}

LoqueClient::LoqueClient() : host("192.168.0.1"), port(5001) {}

LoqueClient::LoqueClient(const std::string& host, int port) : host(host), port(port) {}

Status LoqueClient::attemptLogin(const std::string& username,
                                 const std::string& userpass,
                                 LoginResult& result) {
    sf::Packet toSend;
    toSend << ReqType::LOGIN << username << userpass;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (status != OK) {
        return status;
    }
    toReceive >> status >> result;
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
    if (status != OK) {
        return status;
    }
    toReceive >> status >> result;
    return status;
}

Status LoqueClient::addClassroom(int userId, int classId) {
    sf::Packet toSend;
    toSend << ReqType::ADD_CLASS << userId << classId;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (status != OK) {
        return status;
    }
    toReceive >> status;
    return status;
}

Status LoqueClient::createClassroom(int userId, const std::string& className) {
    sf::Packet toSend;
    toSend << ReqType::CREATE_CLASS << userId << className;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (status != OK) {
        return status;
    }
    toReceive >> status;
    return status;
}

Status LoqueClient::postGameStats(int userId, const GameStats& stats) {
    sf::Packet toSend;
    toSend << ReqType::POST_STATS << userId << stats;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (status != OK) {
        return status;
    }
    toReceive >> status;
    return status;
}

Status LoqueClient::getUserStats(int userId, UserStats& stats) {
    sf::Packet toSend;
    toSend << ReqType::GET_USER_STATS << userId;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (status != OK) {
        return status;
    }
    toReceive >> status >> stats;
    return status;
}

Status LoqueClient::getEnabledLevels(int userId, std::vector<int>& levelIds) {
    sf::Packet toSend;
    toSend << ReqType::GET_ENABLED_LEVELS << userId;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (status != OK) {
        return status;
    }
    toReceive >> status >> levelIds;
    return status; 
}

Status LoqueClient::enableLevel(int userId, int classId, int levelId) {
    sf::Packet toSend;
    toSend << ReqType::ENABLE_LEVEL << userId << classId << levelId;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (status != OK) {
        return status;
    }
    toReceive >> status;
    return status;
}

Status LoqueClient::disableLevel(int userId, int classId, int levelId) {
    sf::Packet toSend;
    toSend << ReqType::DISABLE_LEVEL << userId << classId << levelId;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (status != OK) {
        return status;
    }
    toReceive >> status;
    return status;
}

Status LoqueClient::getClassStats(int userId, int classId, ClassStats& stats) {
    sf::Packet toSend;
    toSend << ReqType::GET_CLASS_STATS << userId << classId;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (status != OK) {
        return status;
    }
    toReceive >> status >> stats;
    return status;
}

Status LoqueClient::makeRequest(sf::Packet& request, sf::Packet &response) {
    auto status = conn.connect(host, port, sf::seconds(3));
    if (status != sf::Socket::Done) {
        return NETWORK_ERR; 
    }
    status = conn.send(request);
    if (status != sf::Socket::Done) {
        return NETWORK_ERR;
    }
    status = conn.receive(response);
    if (status != sf::Socket::Done) {
        return NETWORK_ERR;
    }
    return OK;
}
