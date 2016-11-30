
// Defines functions and types for serializing and deserializing 
// Loque server requests and responses as sf::Packets. 
//
// These are private to clients. Types and functions public to clients
// are defined in loqueclient.h. 

#ifndef LOQUE_SERIALIZATION_H_
#define LOQUE_SERIALIZATION_H_

#include <SFML/Network.hpp>
#include "loqueclient.h"

// EOF indicators for serializing streams or compound data types.=. 
namespace loque {
namespace serialization {
    const int TERM_SCORES = -1; // Indicates EOF when deserializing scores.
    const int TERM_CLASSES = -1; // Indicates EOF when deserializing classes.
    const int TERM_LEVELS = -1;  // Indicates EOF when deserializing level ids.

    // Indicates EOF when deserializing UserStats. 
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
}
}

/* Status */

inline sf::Packet& operator>>(sf::Packet& packet, Status& s) {
    int i;
    packet >> i;
    s = static_cast<Status>(i);
    return packet;
}

/* ReqType */

enum ReqType {
    LOGIN,
    CREATE_ACC,
    ADD_CLASS,
    CREATE_CLASS, 
    POST_STATS,
    GET_USER_STATS,
    GET_ENABLED_LEVELS,
    ENABLE_LEVEL,
    DISABLE_LEVEL,
    GET_CLASS_STATS
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

/* UserStats */

inline sf::Packet& operator<<(sf::Packet& packet, const UserStats& stats) {
    packet << stats.userId << stats.username << stats.totalSecPlayed << stats.totalScore;
    for (auto& elem : stats.highScores) {
        packet << elem.first << elem.second; 
    }
    packet << loque::serialization::TERM_SCORES;
    for (auto elem : stats.classIds) {
        packet << elem;
    }
    packet << loque::serialization::TERM_CLASSES;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, UserStats& stats) {
    packet >> stats.userId >> stats.username >> stats.totalSecPlayed >> stats.totalScore;
    do {
        int lid;
        packet >> lid;
        if (lid == loque::serialization::TERM_SCORES) {
            break;
        }
        int score;
        packet >> score;
        stats.highScores[lid] = score;
    } while (true);
    do {
        int classId;
        packet >> classId;
        if (classId == loque::serialization::TERM_CLASSES) {
            break;
        }
        stats.classIds.push_back(classId); 
    } while (true);
    return packet;
}

/* EnabledLevels */

inline sf::Packet& operator<<(sf::Packet& packet, const std::vector<int>& levelIds) {
    for (auto id : levelIds) {
        packet << id;
    }
    packet << loque::serialization::TERM_LEVELS;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, std::vector<int>& levelIds) {
    do {
        int levelId;
        packet >> levelId;
        if (levelId == loque::serialization::TERM_LEVELS) {
            break;
        }
        levelIds.push_back(levelId); 
    } while (true);
    return packet;
}

/* ClassStats */

inline sf::Packet& operator<<(sf::Packet& packet, const ClassStats& stats) {
    for (auto& ustats : stats.studentStats) {
        packet << ustats;
    }
    packet << loque::serialization::termUser();
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, ClassStats& stats) {
    do {
        UserStats s;
        packet >> s;
        if (loque::serialization::isTermUser(s)) {
            break;
        }

        // TODO: Pass-by-value copy bites us here a bit. Use unique_ptr? 
        stats.studentStats.push_back(s); 
    } while (true);
    return packet;
}

#endif
