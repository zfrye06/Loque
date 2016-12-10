#ifndef LQ_LOQUECLIENT_H_
#define LQ_LOQUECLIENT_H_

#include <SFML/Network.hpp>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

struct MapData {
    unsigned int id;
    std::string path;
    std::string thumbnail;
    MapData(unsigned int id, std::string path, std::string thumbnailPath);
};

extern MapData Maps[];
extern std::size_t MapCount;

enum Status {
    OK,
    NETWORK_ERR,
    DB_ERR
};
std::ostream& operator<<(std::ostream&, Status); 

enum UserType {
    ADMIN,
    STUDENT,
    DNE
};
std::ostream& operator<<(std::ostream&, UserType);

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

// Information about a single level. 
struct LevelInfo {
    int id;
    std::string name;
    std::string description;
};
std::ostream& operator<<(std::ostream&, const LevelInfo&); 

struct UserInfo {
    std::string username;
    int userId;
    UserType type;
    
    UserInfo(); 
    UserInfo(std::string username, int userId, UserType type);
};
std::ostream& operator<<(std::ostream&, const UserInfo&); 

// Information about a single user (admin or student).
struct UserStats {
    int userId;
    std::string username;
    int totalSecPlayed;
    int totalScore;
    
    // Maps level ids to scores.
    std::unordered_map<int, int> highScores;

    // Maps level ids to completion times
    std::unordered_map<int, int> completionTimes;

    // If the user is an instructor, these reflect the classes they manage.
    // If the user is a student, these reflect the classes they are a member of.
    std::vector<int> classIds;
};

// Provides a record of a user's best performance on a particular level.
// If LevelRecord::hasCompleted returns false, the user has not played the level.
struct LevelRecord {
    int highScore;
    int bestCompletionTimeSecs;
    LevelInfo level;

    bool hasCompleted() const; 
};
std::ostream& operator<<(std::ostream&, const LevelRecord&); 

// Per-class information on a user's completed levels. 
struct ClassLevelInfo {
    int classId;
    std::string className;
    std::vector<LevelRecord> levelRecords;
};
std::ostream& operator<<(std::ostream&, const ClassLevelInfo&);

typedef std::vector<ClassLevelInfo> UserLevelInfo; 

// Information about a class in aggregate. 
struct ClassStats {
    int classId;
    std::string className;
    std::vector<UserStats> studentStats;
    std::vector<LevelInfo> enabledLevels;

    int totalSecPlayed() const;
    const UserStats *highestScoringStudent() const;
    const UserStats *mostFrequentPlayer() const;
};

// Use an instance of LoqueClient to make API
// calls to a running Loque Server.
//
// All methods which make calls to the server block and return a Status
// indicating success or failure of the network and database interactions.
class LoqueClient {
public:

    LoqueClient();
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

    // Creates a classroom with the given name whose sole member is the given user.
    // The given userId should be that of an instructor.
    Status createClassroom(int userId, const std::string& className, ClassStats& classStats);

<<<<<<< HEAD
    //Deletes the class with the given id.
    Status deleteClass(int classID);
=======
    // Deletes the given classroom, removing all students and instructors. 
    Status deleteClassroom(int classId); 
>>>>>>> 486f366... server - Fix getClassStats. Add delete classroom.

    // Adds a game record for the given user. This should be called
    // after each completed level.
    Status postGameStats(int userId, const GameStats& stats);
    
    // Retrieves the statistics for the given user. This should only
    // be called for the currently logged-in user. Prefer LoqueClient::getClassStats
    // to get classroom statistics.
    Status getUserStats(int userId, UserStats& stats);

    // Retrieves information about the levels enabled for the given user, identified
    // by the class for which they are enabled.
    Status getUserLevelInfo(int userId, UserLevelInfo& out);

    // Retrieves a list of enabled level IDs for the given user.
    Status getEnabledLevels(int userId, std::vector<int>& levels);

    // Retrieves a list of enabled level IDs for the given class.
    Status getEnabledClassLevels(int classId, std::vector<int>& levels);

    // Enables a level for the given class. UserId must be an instructor id.
    Status enableLevel(int userId, int classId, int levelId);

    // Disables a level for the given class. UserId must be an instructor id.
    Status disableLevel(int userId, int classId, int levelId);

    // Retrieves the ClassStats associated with the given ID.
    Status getClassStats(int classId, ClassStats& stats);

    // Retrieves statistics for the given class. UserId should be an instructor id.
    Status getAllClassStats(int userId, std::vector<ClassStats>& stats);

    // Retrieves information about every loque level.
    Status getAllLevels(std::vector<LevelInfo>& out);
    
private:
    
    const std::string host;
    const int port;

    Status makeRequest(sf::Packet&, sf::Packet&);
};

#endif
