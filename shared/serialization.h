
 
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
    POST_STATS
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

/* LoginRequest */

struct LoginRequest {
    std::string username;
    std::string userpass;
};

inline sf::Packet& operator>>(sf::Packet& packet, LoginRequest& r) {
    packet >> r.username >> r.userpass;
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

/* CreateAccountRequest */

struct CreateAccountRequest {
    std::string username;
    std::string userpass;
    UserType type; 
};

inline sf::Packet& operator>>(sf::Packet& packet, CreateAccountRequest& r) {
    packet >> r.username >> r.userpass >> r.type;
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

struct PostStatsRequest {
    int userId;
    GameStats stats;
};

inline sf::Packet& operator>>(sf::Packet& packet, PostStatsRequest& req) {
    packet >> req.userId >> req.stats;
    return packet;
}

#endif
