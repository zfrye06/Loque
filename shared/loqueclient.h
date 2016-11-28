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

    // Returns whether the requested user exists (i.e. type != DNE). 
    bool successful() const;
};

// A generic action result. This indicates success or failure
// of an API action itself, not of the associated network
// transaction (which is indicated by the returned Status). 
struct ActionResult {
    bool success;       // Whether the action succeeded. 
    std::string reason; // Reason for failure, if any. 
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
// All methods which make calls to the server block and
// return an sf::Socket::Status value indicating network success
// or failure. A call's associated out-parameter indicates whether
// the request's business logic and database interaction have succeeded on the server.
class LoqueClient {
 public:

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
    // The output ActionResult indicates whether the action succeeded and,
    // if not, why it failed (e.g. class DNE). 
    Status addClassroom(int userId, int classId, ActionResult& res);

    // Creates a classroom with the given namewhose sole member is the given user.
    // The given userId should be that of an instructor. 
    Status createClassroom(int userId, const std::string& className, ActionResult& res);

    // Adds a game record for the given user. This should be called
    // after each completed level. 
    Status postGameStats(int userId, const GameStats& stats, ActionResult& res);
    
    // Retrieves the statistics for the given user. This should only
    // be called for the currently logged-in user. Prefer LoqueClient::getClassStats
    // to get classroom statistics.
    Status getUserStats(int userId, UserStats& stats);

    // Retrieves the enabled levels for the given user, placing their IDs in levelIds. 
    Status getEnabledLevels(int userId, std::vector<int>& levelIds); 

    // Enables a level for the given class. UserId must be an instructor id.
    Status enableLevel(int userId, int classId, int levelId, ActionResult& res);

    // Disables a level for the given class. UserId must be an instructor id. 
    Status disableLevel(int userId, int classId, int levelId, ActionResult& res);

    // Retrieves statistics for the given class. UserId must be an instructor id. 
    Status getClassStats(int userId, int classId, ClassStats& stats); 
    
 private:
    
    const std::string host;
    const int port;
    sf::TcpSocket conn;

    Status makeRequest(sf::Packet&, sf::Packet&);
};

#endif
