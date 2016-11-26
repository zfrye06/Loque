
// Defines functions and types for serializing and deserializing 
// Loque server requests and responses as sf::Packets. 
//
// These are private to clients. Types and functions public to clients
// are defined in loqueclient.h. 

#ifndef LOQUE_SERIALIZATION_H_
#define LOQUE_SERIALIZATION_H_

#include <SFML/network.hpp>
#include "loqueclient.h"

/* ReqType */

enum ReqType {
    LOGIN,
    CREATE_ACC,
    ADD_CLASS,
    POST_STATS,
    GET_USER_STATS,
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

/* ActionResult */

inline sf::Packet& operator<<(sf::Packet& packet, const ActionResult& res) {
    packet << res.success << res.reason;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, ActionResult& res) {
    packet >> res.success >> res.reason;
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

// EOF indicators for serialized userstats.
namespace ustats {
    const int TERM_SCORES = -1;
    const int TERM_CLASSES = -1; 
}

inline sf::Packet& operator<<(sf::Packet& packet, const UserStats& stats) {
    packet << stats.userId << stats.username << stats.totalSecPlayed << stats.totalScore;
    for (auto& elem : stats.highScores) {
        packet << elem.first << elem.second; 
    }
    packet << ustats::TERM_SCORES;
    for (auto elem : stats.classIds) {
        packet << elem;
    }
    packet << ustats::TERM_CLASSES;
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, UserStats& stats) {
    packet >> stats.userId >> stats.username >> stats.totalSecPlayed >> stats.totalScore;
    do {
        int lid;
        packet >> lid;
        if (lid == ustats::TERM_SCORES) {
            break;
        }
        int score;
        packet >> score;
        stats.highScores[lid] = score;
    } while (true);
    do {
        int classId;
        packet >> classId;
        if (classId == ustats::TERM_CLASSES) {
            break;
        }
        stats.classIds.push_back(classId); 
    } while (true);
    return packet;
}

/* ClassStats */

// EOF indicator for serialized class stats.
namespace cstats {
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

inline sf::Packet& operator<<(sf::Packet& packet, const ClassStats& stats) {
    for (auto& ustats : stats.studentStats) {
        packet << ustats;
    }
    packet << cstats::termUser();
    return packet;
}

inline sf::Packet& operator>>(sf::Packet& packet, ClassStats& stats) {
    do {
        UserStats s;
        packet >> s;
        if (cstats::isTermUser(s)) {
            break;
        }

        // TODO: Pass-by-value copy bites us here a bit. Use unique_ptr? 
        stats.studentStats.push_back(s); 
    } while (true);
    return packet;
}

#endif
