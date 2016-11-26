#ifndef LQ_LOQUECLIENT_H_
#define LQ_LOQUECLIENT_H_

#include <SFML/Network.hpp>
#include <unordered_map>
#include <vector>

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

// Information about a single user (admin or student).
struct UserStats {
    // TODO: Add function to get list of completed levels.
    int userId;
    std::string username; 
    int totalSecPlayed;
    int totalScore;
    std::unordered_map<LevelId, int> highScores;

    // If the user is an instructor, these reflect the classes they manage.
    // If the user is a student, these reflect the classes they are a member of. 
    std::vector<int> classIds;
};

// Information about a class in aggregate. 
struct ClassStats {
    // TODO: What fields should we add here?
    std::vector<UserStats> studentStats; 
};

// Use an instance of LoqueClient to make API
// calls to a running Loque Server.
//
// All methods which make calls to the server block and
// return an sf::Socket::Status value indicating sucess
// or failure. 
class LoqueClient {
 public:

    // TODO: We need to return some record of success/failure for postgamestats,
    // enableLevel, disableLevel, and addClassroom.

    LoqueClient(const std::string& address, int port);

    // Attempts to login the user, placing the result in the result parameter.
    Status attemptLogin(const std::string& username,
                        const std::string& userpass,
                        LoginResult& result);

    // Attempts to create an account of the given type. Upon success,
    // the user is logged in and result reflects his user type. Upon failure,
    // result.userType is set to DNE. 
    Status createAccount(const std::string& username,
                         const std::string& userpass,
                         UserType type, 
                         LoginResult& result);

    // Adds the student with the given id to the given classroom. 
    Status addClassroom(int userId, int classId);

    // Adds a game record for the given user. This should be called
    // after each completed level. 
    Status postGameStats(int userId, const GameStats& stats);
    
    // Retrieves the statistics for the given user. This should only
    // be called for the currently logged-in user. Prefer LoqueClient::getClassStats
    // to get classroom statistics.
    Status getUserStats(int userId, UserStats& stats);

    // Enables a level for the given class. UserId must be an instructor id.
    Status enableLevel(int userId, int classId, LevelId id);

    // Disables a level for the given class. UserId must be an instructor id. 
    Status disableLevel(int userId, int classId, LevelId id);

    // Retrieves statistics for the given class. UserId must be an instructor id. 
    Status getClassStats(int userId, int classId, ClassStats& stats); 
    
 private:
    
    const std::string host;
    const int port;
    sf::TcpSocket conn;

    Status makeRequest(sf::Packet&, sf::Packet *);
};

#endif
