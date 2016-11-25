#ifndef LQ_LOQUECLIENT_H_
#define LQ_LOQUECLIENT_H_

#include <SFML/Network.hpp>

typedef sf::Socket::Status Status;

inline bool ok(Status s) { return s == sf::Socket::Done; }

enum UserType {
    Admin,
    Student,
    DNE
};

// The result of an attempted login or account creation.
struct LoginResult {
    int userId;
    UserType userType;

    // Returns whether the requested user exists (e.g. type != DNE). 
    bool successful() const;
};

typedef int LevelId; 

// Information about a single completed game. 
struct GameStats {
    LevelId levelId;
    int secToComplete;
    int pointsScored;
};

// Use an instance of LoqueClient to make API
// calls to a running Loque Server.
//
// All methods which make calls to the server block and
// return an sf::Socket::Status value indicating sucess
// or failure. 
class LoqueClient {
 public:

    LoqueClient(const std::string& address, int port);

    // Attempts to login the user, placing the result in the result parameter.
    Status attemptLogin(const std::string& username,
                        const std::string& userpass,
                        LoginResult& result);

    // Attempts to create an account of the given type. 
    Status createAccount(const std::string& username,
                         const std::string& userpass,
                         UserType type, 
                         LoginResult& result);

    // Adds a game record for the given user. 
    Status postGameStats(int userId, const GameStats& stats);
    
 private:
    
    const std::string host;
    const int port;
    sf::TcpSocket conn;

    Status sendRequest(sf::Packet& req, sf::Packet& resp);
};

#endif
