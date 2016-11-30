#ifndef LQ_LOQUECLIENT_H_
#define LQ_LOQUECLIENT_H_

#include <SFML/Network.hpp>
#include <ostream>
#include <unordered_map>
#include <vector>

enum Status {
    OK,
    NETWORK_ERR,
    DB_ERR
};
std::ostream& operator<<(std::ostream&, Status s); 

enum UserType {
    Admin,
    Student,
    DNE
};

// The result of an attempted login or account creation.
struct LoginResult {
    int userId;
    UserType userType;

    // Returns whether the requested user exists (i.e. type != DNE). 
    bool successful() const;
};

// Information about a single completed game. 
struct GameStats {
    int levelId;
    int secToComplete;
    int pointsScored;
};

// Information about a single user (admin or student).
struct UserStats {
    int userId;
    std::string username; 
    int totalSecPlayed;
    int totalScore;
    
    // Maps level ids to scores.
    std::unordered_map<int, int> highScores;

    // If the user is an instructor, these reflect the classes they manage.
    // If the user is a student, these reflect the classes they are a member of. 
    std::vector<int> classIds;
};

// Information about a class in aggregate. 
struct ClassStats {
    std::vector<UserStats> studentStats;
};

// Use an instance of LoqueClient to make API
// calls to a running Loque Server.
//
// All methods which make calls to the server block and return a Status
// indicating success or failure of the network and database interactions.
class LoqueClient {
 public:

    LoqueClient(const std::string& address, int port);

    // Attempts to login the user, placing the result in the result parameter.
    // Upon a failed login, result.UserType is set to DNE. 
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

    // Creates a classroom with the given namewhose sole member is the given user.
    // The given userId should be that of an instructor. 
    Status createClassroom(int userId, const std::string& className);

    // Adds a game record for the given user. This should be called
    // after each completed level. 
    Status postGameStats(int userId, const GameStats& stats);
    
    // Retrieves the statistics for the given user. This should only
    // be called for the currently logged-in user. Prefer LoqueClient::getClassStats
    // to get classroom statistics.
    Status getUserStats(int userId, UserStats& stats);

    // Retrieves the enabled levels for the given user, placing their IDs in levelIds. 
    Status getEnabledLevels(int userId, std::vector<int>& levelIds); 

    // Enables a level for the given class. UserId must be an instructor id.
    Status enableLevel(int userId, int classId, int levelId);

    // Disables a level for the given class. UserId must be an instructor id. 
    Status disableLevel(int userId, int classId, int levelId);

    // Retrieves statistics for the given class. UserId must be an instructor id. 
    Status getClassStats(int userId, int classId, ClassStats& stats); 
    
 private:
    
    const std::string host;
    const int port;
    sf::TcpSocket conn;

    Status makeRequest(sf::Packet&, sf::Packet&);
};

#endif
