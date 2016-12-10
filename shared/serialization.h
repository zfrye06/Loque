
// Defines functions and types for serializing and deserializing 
// Loque server requests and responses as sf::Packets. 
//
// These are private to clients. Types and functions public to clients
// are defined in loqueclient.h. 

#ifndef LOQUE_SERIALIZATION_H_
#define LOQUE_SERIALIZATION_H_

#include <SFML/Network.hpp>
#include "loqueclient.h"

/* ReqType */

enum ReqType {
    LOGIN,
    CREATE_ACC,
    ADD_CLASS,
    CREATE_CLASS,
    DELETE_CLASS,
    POST_STATS,
    GET_USER_STATS,
    GET_USER_LEVEL_INFO,
    GET_ENABLED_LEVELS,
    GET_ENABLED_CLASS_LEVELS,
    ENABLE_LEVEL,
    DISABLE_LEVEL,
    GET_CLASS_STATS,
    GET_ALL_LEVELS,
    GET_ALL_CLASS_STATS,
};

inline sf::Packet& operator<<(sf::Packet& packet, const ReqType& t) {
    packet << static_cast<int>(t);
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, ReqType& t) {
    int i;
    packet >> i;
    t = static_cast<ReqType>(i);
    return packet;
}

// EOF indicators for serializing streams or compound data types.
namespace loque {
namespace serialization {
    const int TERM_SCORE = -2; // Indicates EOF when deserializing high scores.
    const int TERM_CLASSID = -3; // Indicates EOF when deserializing class IDs.
    const int TERM_LEVELID = -4;  // Indicates EOF when deserializing level IDs.
    const int TERM_TIME = -5; // Indicates EOF when deserializing level IDs.

    // Indicates EOF when serializing UserStats. 
    UserStats termUser() {
        UserStats s;
        s.userId = -1;
        s.username = "";
        s.totalSecPlayed = -1;
        s.totalScore = -1;
        return s;
    }

    bool isTermUser(const UserStats& s) {
        return s.userId == -1; 
    }

    // Indicates EOF when serializing LevelInfo. 
    LevelInfo termLevelInfo() {
        LevelInfo info;
        info.id = -1;
        info.name = "";
        info.description = "";
        return info;
    }

    bool isTermLevelInfo(const LevelInfo& info) {
        return info.id == -1;
    }

    // Indicates EOF when serializing LevelRecord.
    LevelRecord termLevelRecord() {
        LevelRecord record;
        record.highScore = -2;
        record.bestCompletionTimeSecs = -2;
        record.level = termLevelInfo();
        return record;
    }

    bool isTermLevelRecord(const LevelRecord& record) {
        return record.bestCompletionTimeSecs == -2; 
    }

    // Indicates EOF when serializing ClassStats.
    ClassStats termClassStats() {
        ClassStats stats;
        stats.classId = -1;
        stats.className = "";
        return stats;
    }

    bool isTermClassStats(const ClassStats& stats) {
        return stats.classId == -1;
    }
}
}

/* Status */

inline sf::Packet& operator>>(sf::Packet& packet, Status& s) {
    int i;
    packet >> i;
    s = static_cast<Status>(i);
    return packet;
}

/* UserType */

inline sf::Packet& operator<<(sf::Packet& packet, const UserType& ut) {
    packet << static_cast<int>(ut);
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, UserType& ut) {
    int i;
    packet >> i;
    ut = static_cast<UserType>(i);
    return packet;
}

/* LoginResult */

inline sf::Packet& operator<<(sf::Packet& packet, const LoginResult& res) {
    packet << res.userId << res.userType;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, LoginResult& res) {
    packet >> res.userId >> res.userType;
    return packet;
}

/* GameStats */

inline sf::Packet& operator<<(sf::Packet& packet, const GameStats& stats) {
    packet << stats.levelId << stats.secToComplete << stats.pointsScored;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, GameStats& stats) {
    packet >> stats.levelId >> stats.secToComplete >> stats.pointsScored;
    return packet;
}

/* LevelInfo */

inline sf::Packet& operator<<(sf::Packet& packet, const LevelInfo& info) {
    packet << info.id << info.name << info.description;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, LevelInfo& info) {
    packet >> info.id >> info.name >> info.description;
    return packet; 
}

inline sf::Packet& operator<<(sf::Packet& packet, const std::vector<LevelInfo>& infos) {
    for (auto& info : infos) {
        packet << info; 
    }
    packet << loque::serialization::termLevelInfo();
    return packet; 
}

inline sf::Packet& operator>>(sf::Packet& packet, std::vector<LevelInfo>& infos) {
    while (true) {
        infos.push_back(LevelInfo());
        packet >> infos.back();
        if (loque::serialization::isTermLevelInfo(infos.back())) {
            infos.pop_back();
            break;
        }
    }
    return packet;
}

/* UserStats */

inline sf::Packet& operator<<(sf::Packet& packet, const UserStats& stats) {
    packet << stats.userId << stats.username << stats.totalSecPlayed << stats.totalScore;
    for (auto& elem : stats.highScores) {
        packet << elem.first << elem.second; 
    }
    packet << loque::serialization::TERM_SCORE;
    for (auto& elem : stats.completionTimes) {
        packet << elem.first << elem.second;
    }
    packet << loque::serialization::TERM_TIME;
    for (auto elem : stats.classIds) {
        packet << elem;
    }
    packet << loque::serialization::TERM_CLASSID;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, UserStats& stats) {
    packet >> stats.userId >> stats.username >> stats.totalSecPlayed >> stats.totalScore;
    while (true) {
        int lid;
        packet >> lid;
        if (lid == loque::serialization::TERM_SCORE) {
            break;
        }
        int score;
        packet >> score;
        stats.highScores[lid] = score;
    }
    while (true) {
        int lid;
        packet >> lid;
        if (lid == loque::serialization::TERM_TIME) {
            break;
        }
        int time;
        packet >> time;
        stats.completionTimes[lid] = time;
    }
    while (true) {
        int classId;
        packet >> classId;
        if (classId == loque::serialization::TERM_CLASSID) {
            break;
        }
        stats.classIds.push_back(classId); 
    }
    return packet;
}

/* LevelRecord */

inline sf::Packet& operator<<(sf::Packet& packet, const LevelRecord& record) {
    packet << record.highScore << record.bestCompletionTimeSecs << record.level;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, LevelRecord& record) {
    packet >> record.highScore >> record.bestCompletionTimeSecs >> record.level;
    return packet;
}

/* UserLevelInfo */

inline sf::Packet& operator<<(sf::Packet& packet, const UserLevelInfo& info) {
    for (auto& classInfo : info) {
        packet << classInfo.classId << classInfo.className;
        for (auto& record : classInfo.levelRecords) {
            packet << record;
        }
        packet << loque::serialization::termLevelRecord();
    }
    packet << loque::serialization::TERM_CLASSID; 
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, UserLevelInfo& info) {
    while (true) {
        ClassLevelInfo classInfo;
        packet >> classInfo.classId;
        if (classInfo.classId == loque::serialization::TERM_CLASSID) {
            break;
        }
        packet >> classInfo.className;
        while (true) {
            LevelRecord record;
            packet >> record;
            if (loque::serialization::isTermLevelRecord(record)) {
                break; 
            }
            classInfo.levelRecords.push_back(record);
        }
        info.push_back(classInfo); 
    }
    return packet;
}

/* EnabledLevels */

inline sf::Packet& operator<<(sf::Packet& packet, const std::vector<int>& levelIds) {
    for (auto id : levelIds) {
        packet << id;
    }
    packet << loque::serialization::TERM_LEVELID;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, std::vector<int>& levelIds) {
    while (true) {
        int levelId;
        packet >> levelId;
        if (levelId == loque::serialization::TERM_LEVELID) {
            break;
        }
        levelIds.push_back(levelId); 
    } 
    return packet;
}

/* ClassStats */

inline sf::Packet& operator<<(sf::Packet& packet, const ClassStats& stats) {
    packet << stats.classId << stats.className; 
    for (auto& ustats : stats.studentStats) {
        packet << ustats;
    }
    packet << loque::serialization::termUser();
    for (auto& info : stats.enabledLevels) {
        packet << info; 
    }
    packet << loque::serialization::termLevelInfo();
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, ClassStats& stats) {
    packet >> stats.classId >> stats.className; 
    while (true) {
        stats.studentStats.push_back(UserStats()); 
        packet >> stats.studentStats.back();
        if (loque::serialization::isTermUser(stats.studentStats.back())) {
            stats.studentStats.pop_back(); 
            break;
        }
    }
    while (true) {
        stats.enabledLevels.push_back(LevelInfo());
        packet >> stats.enabledLevels.back();
        if (loque::serialization::isTermLevelInfo(stats.enabledLevels.back())) {
            stats.enabledLevels.pop_back();
            break;
        }
    }
    return packet;
}

inline sf::Packet& operator<<(sf::Packet& packet, const std::vector<ClassStats>& stats) {
    for (auto& cstats : stats) {
        packet << cstats;
    }
    packet << loque::serialization::termClassStats();
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, std::vector<ClassStats>& stats) {
    while (true) {
        ClassStats cStats;
        stats.push_back(cStats); 
        packet >> stats.back();
        if (loque::serialization::isTermClassStats(stats.back())) {
            stats.pop_back(); 
            break;
        }
    }
    return packet; 
}

#endif
