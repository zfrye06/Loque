#include <iostream>
#include "loqueclient.h"
#include "serialization.h"

MapData::MapData(unsigned int id, std::string path, std::string thumbnailPath) :
    id(id), path(path), thumbnail(thumbnailPath) {}

std::string MapData::qtThumbnailPath() {
    return ":/assets/candySky.jpg";
}

MapData Maps[] = {
    MapData(0, "assets/Zapper_Level_1.tmx", "assets/images/candySky.jpg"),
    MapData(1, "assets/Zapper_Level_2.tmx", "assets/images/candySky.jpg"),
    MapData(2, "assets/Platform_Level_1.tmx", "assets/images/candySky.jpg"),
    MapData(3, "assets/Castle_Level_1.tmx", "assets/images/candySky.jpg"),
    MapData(4, "assets/Castle_Level_2.tmx", "assets/images/candySky.jpg")
};
std::size_t MapCount = 4;

std::ostream& operator<<(std::ostream& out, Status s) {
    switch (s) {
    case Status::OK:
        out << "Status<OK>";
        break;
    case Status::NETWORK_ERR:
        out << "Status<NETWORK_ERR>";
        break;
    case Status::DB_ERR:
        out << "Status<DB_ERR>";
        break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, UserType t) {
    switch (t) {
    case UserType::ADMIN:
        out << "UserType<ADMIN>";
        break;
    case UserType::STUDENT:
        out << "UserType<STUDENT>";
        break;
    case UserType::DNE:
        out << "UserType<DNE>";
        break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const LevelInfo& info) {
    out << "LevelInfo<id: " << info.id <<
        ", name: " << info.name <<
        ", description: " << info.description <<
        ">";
    return out; 
}

UserInfo::UserInfo() : username(""), userId(-1), type(UserType::DNE) {}
UserInfo::UserInfo(std::string username, int userId, UserType type) :
    username(username), userId(userId), type(type) {}

std::ostream& operator<<(std::ostream& out, const UserInfo& info) {
    out << "UserInfo<username: " << info.username <<
        ", userId: " << info.userId <<
        ", type: " << info.type <<
        ">";
    return out;
}

bool LevelRecord::hasCompleted() const {
    return bestCompletionTimeSecs != -1; 
}

std::ostream& operator<<(std::ostream& out, const LevelRecord& record) {
    out << "LevelRecord<highScore: " << record.highScore <<
        ", bestCompletionTimeSecs: " << record.bestCompletionTimeSecs <<
        ", level: " << record.level <<
        ">";
    return out;
}

std::ostream& operator<<(std::ostream& out, const ClassLevelInfo& info) {
    out << "ClassLevelInfo<classId: " << info.classId <<
        ", className: " << info.className <<
        ", levelRecords: ";
    for (auto& r : info.levelRecords) out << r << ", ";
    out << ">";
    return out;
}

LoqueClient::LoqueClient() : host("127.0.0.1"), port(5001) {}

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

Status LoqueClient::createClassroom(int userId, const std::string& className, ClassStats& classStats) {
    sf::Packet toSend;
    toSend << ReqType::CREATE_CLASS << userId << className << classStats;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (status != OK) {
        return status;
    }
    toReceive >> status >> classStats;
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

Status LoqueClient::getEnabledClassLevels(int classID, std::vector<int>& levelIds) {
    sf::Packet toSend;
    toSend << ReqType::GET_ENABLED_CLASS_LEVELS << classID;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (status != OK) {
        return status;
    }
    toReceive >> status >> levelIds;
    return status;
}

Status LoqueClient::getUserLevelInfo(int userId, UserLevelInfo& out) {
    sf::Packet toSend;
    toSend << ReqType::GET_USER_LEVEL_INFO << userId;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (status != OK) {
        return status; 
    }
    toReceive >> status >> out;
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

Status LoqueClient::getClassStats(int classId, ClassStats& stats) {
    sf::Packet toSend;
    toSend << ReqType::GET_CLASS_STATS << classId;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (status != OK) {
        return status;
    }
    toReceive >> status >> stats;
    return status;
}

Status LoqueClient::getAllClassStats(int userId, std::vector<ClassStats>& stats) {
    sf::Packet toSend;
    toSend << ReqType::GET_ALL_CLASS_STATS << userId;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (status != OK) {
        return status;
    }
    toReceive >> status >> stats;
    return status;
}

Status LoqueClient::getAllLevels(std::vector<LevelInfo>& out) {
    sf::Packet toSend;
    toSend << ReqType::GET_ALL_LEVELS;
    sf::Packet toReceive;
    auto status = makeRequest(toSend, toReceive);
    if (status != OK) {
        return status;
    }
    toReceive >> status >> out; 
    return status;
}

Status LoqueClient::makeRequest(sf::Packet& request, sf::Packet& response) {
    sf::TcpSocket conn;
    auto status = conn.connect(host, port, sf::seconds(2));
    if (status != sf::Socket::Done) {
        return NETWORK_ERR; 
    }
    status = conn.send(request);
    if (status != sf::Socket::Done) {
        return NETWORK_ERR;
    }
    status = conn.receive(response);
    // Seems as though receive sets the status to disconnected, but this is OK for our game client.
    if (status != sf::Socket::Done && status != sf::Socket::Disconnected) {
        return NETWORK_ERR;
    }
    return OK;
}
