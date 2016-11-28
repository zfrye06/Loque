#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <SFML/Network.hpp>
#include <iostream>
#include <memory>
#include <string.h>
#include <thread>
#include <math.h>
#include "../../shared/serialization.h"

const int DEFAULT_PORT = 5001;
const std::string DB_ADDR = "tcp://bernardcosgriff.com:3306";
const std::string DB_USER = "teamaccess";
const std::string DB_PASS = "password";
 sql::Driver *driver = get_driver_instance();
// sql::Connection *conn = driver->connect(DB_ADDR, DB_USER, DB_PASS);
 sql::PreparedStatement *pstmt;
 sql::ResultSet *rs;


/*
 * Adds a class with class name and associates the teacher with their class.
 */
bool addClass(sql::Connection *conn, int userID, std::string className){
    bool success = false;
    try {
        pstmt = conn->prepareStatement(
                "INSERT INTO ClassAssociations(userID, className) VALUES(?, ?)");
        pstmt->setInt(1, userID);
        pstmt->setString(2, className);
        success = pstmt->execute();
        return success;
    } catch (sql::SQLException &e) {
        std::cout << "Insert New Class Failed: " << e.what() << std::endl;
        return success;
    }
}

 /*
  * Returns true if user is succesfully added and false if not (i.e. username already in use).
  */
 bool addUser(sql::Connection *conn, std::string username, std::string password, bool isAdmin) {
     try {
         pstmt = conn->prepareStatement(
                 "INSERT INTO User(username, password, isAdmin, levelsCompleted, totalScore, totalTime) VALUES (?, ?, ?, ?, ?, ?)");
         pstmt->setString(1, username);
         pstmt->setString(2, password);
         pstmt->setBoolean(3, isAdmin);
         pstmt->setInt(4, 0);
         pstmt->setInt(5, 0);
         pstmt->setInt(6, 0);
         pstmt->execute();
         return true;
     } catch (sql::SQLException &e) {
         std::cout << "Insert User Failed: " << e.what() << std::endl;
         return false;
     }
 }
 /*
  * Enables/Disables maps for classes
  */
 void toggleMaps(sql::Connection *conn, int classID, std::vector<int> mapIDs, bool enable) {
     if (enable) {
         // Not sure what the best way to insert only if entry does not already exist.
         // Currently just catch exception and move on.
         try {
             for (int mapID : mapIDs) {
                 pstmt = conn->prepareStatement(
                         "INSERT INTO MapAssociations(classID, mapID) VALUES(?, ?)");
                 pstmt->setInt(1, classID);
                 pstmt->setInt(2, mapID);
                 pstmt->execute();
             }
         } catch (sql::SQLException &e) {
             std::cout << "Insert Map Failed: " << e.what() << std::endl;
         }
     } else {
         // Delete does not throw an exception if there is nothing to delete.
         try {
             for (int mapID : mapIDs) {
                 pstmt = conn->prepareStatement(
                         "DELETE FROM MapAssociations WHERE classID = ? AND mapID = ?");
                 pstmt->setInt(1, classID);
                 pstmt->setInt(2, mapID);
                 pstmt->execute();
             }
         } catch (sql::SQLException &e) {
             std::cout << "Delete Map Failed: " << e.what() << std::endl;
         }
     }
     delete pstmt;
 }
 /*
  * Updates levels completed flag, high score, and completion time for the level.
  */
 void levelCompleted(sql::Connection *conn, int userID, int mapID, int newScore, int newTime) {
     try {
         bool update = false;

         // Update total time played and total score
         pstmt = conn->prepareStatement("UPDATE User SET totalTime = totalTime + ?, totalScore = totalScore + ? WHERE userID  = ?");
         pstmt->setInt(1, newTime);
         pstmt->setInt(2, newScore);
         pstmt->setInt(3, userID);
         pstmt->execute();

         // Grab levelsCompleted flag
         pstmt = conn->prepareStatement(
                 "SELECT levelsCompleted FROM User WHERE userID = ?");
         pstmt->setInt(1, userID);
         rs = pstmt->executeQuery();
         if(rs->next()){
             // Set the flag for the level just completed
             unsigned int flag = rs->getInt(1);

             // Wrong I think
             // flag = flag | mapID;

             pstmt = conn->prepareStatement("UPDATE User SET levelsCompleted = ? WHERE userID = ?");
             pstmt->setInt(1, flag);
             pstmt->setInt(2, userID);

             // See if they have completed this level before
             pstmt = conn->prepareStatement("SELECT levelHighScore, completionTime FROM ScoreInfo WHERE userID = ? AND mapID = ?");
             pstmt->setInt(1, userID);
             pstmt->setInt(2, mapID);
             rs = pstmt->executeQuery();

             //If they have ...
             if(rs->next()){
                 //See if they have a new high score or a faster completion time
                 int oldScore = rs->getInt(1);
                 int oldTime = rs->getInt(2);
                 if(newScore > oldScore){
                     oldScore = newScore;
                     update = true;
                 }
                 if(newTime < oldTime){
                     oldTime = newTime;
                     update  = true;
                 }
                 if(update) {
                     // Update in the database
                     pstmt = conn->prepareStatement(
                             "UPDATE ScoreInfo SET levelHighScore = ?, completionTime = ? WHERE userID = ? AND mapID = ?");
                     pstmt->setInt(1, oldScore);
                     pstmt->setInt(2, oldTime);
                     pstmt->setInt(3, userID);
                     pstmt->setInt(4, mapID);
                     pstmt->execute();
                 }

                 //Otherwise just insert a new row
             } else{
                 pstmt = conn->prepareStatement("INSERT INTO ScoreInfo(userID, mapID, levelHighScore, completionTime) VALUES(?, ?, ?, ?)");
                 pstmt->setInt(1, userID);
                 pstmt->setInt(2, mapID);
                 pstmt->setInt(3, newScore);
                 pstmt->setInt(4, newTime);
                 pstmt->execute();
             }
         }
         else{
             std::cout << "User does not exist." << std::endl;
             return;
         }


     } catch (sql::SQLException &e) {
         std::cout << "Level Completion Update Failed: " << e.what() << std::endl;
     }
 }

 /*
  * Returns all maps enabled for the specified class.
  */
 std::vector<int> getEnabledMaps(sql::Connection *conn, int classID) {
     std::vector<int> v;
     try {
         pstmt = conn->prepareStatement(
                 "SELECT mapID FROM MapAssociations WHERE classID = ?");
         pstmt->setInt(1, classID);
         rs = pstmt->executeQuery();
         while (rs->next()) {
             v.push_back(rs->getInt(1));
         }
         delete pstmt;
         return v;
     } catch (sql::SQLException &e) {
         std::cout << "Select Failed: " << e.what() << std::endl;
         return v;
     }
 }

 /*
  *
  */
 std::map<int, int> getTotalScores(sql::Connection *conn, int classID) {
     std::map<int, int> v;
     try {
         int user;
         int score;
         pstmt = conn->prepareStatement("SELECT userID, totalScore FROM User WHERE userID IN (SELECT userID FROM UserAssociations WHERE classID = ?)");
         pstmt->setInt(1, classID);
         rs = pstmt->executeQuery();
         while(rs->next()){
             user = rs->getInt(1);
             score = rs->getInt(2);
             v[user] = score;
         }
         delete pstmt;
         return v;
     } catch (sql::SQLException &e) {
         std::cout << "Select Total Scores Failed: " << e.what() << std::endl;
         return v;
     }
 }

 /*
  *
  */
 std::map<int, int> getTotalTimes(sql::Connection *conn, int classID) {
     std::map<int, int> v;
     try {
         int user;
         int time;
         pstmt = conn->prepareStatement("SELECT userID, totalTime FROM User WHERE userID IN (SELECT userID FROM UserAssociations WHERE classID = ?)");
         pstmt->setInt(1, classID);
         rs = pstmt->executeQuery();
         while(rs->next()){
             user = rs->getInt(1);
             time = rs->getInt(2);
             v[user] = time;
         }
         delete pstmt;
         return v;
     } catch (sql::SQLException &e) {
         std::cout << "Select Total Scores Failed: " << e.what() << std::endl;
         return v;
     }
 }

 /*
  * Returns the highest level completed by the specified user
  */
 int getHighestLevelCompleted(sql::Connection *conn, int userID) {
     int highestLevel = 0;
     try {
         unsigned int levels = 0;
         pstmt = conn->prepareStatement("SELECT levelsCompleted FROM User WHERE userID = ?");
         pstmt->setInt(1, userID);
         rs = pstmt->executeQuery();
         if(rs->next()){
             levels = rs->getInt(1);
         }
         else{
             std::cout << "User Does Not Exist" << std::endl;
             return 0;
         }
         for(unsigned int i = 128; i > 0; i = i/2){
             if((i & levels) != 0){
                 highestLevel = log2(i);
             }
         }
         delete pstmt;
         return highestLevel;
     } catch (sql::SQLException &e) {
         std::cout << "Select Total Scores Failed: " << e.what() << std::endl;
         return highestLevel;
     }
 }

 /*
  * Gets the average high score for the class with classID on the level with mapID.
  */
 int getClassLevelAverage(sql::Connection *conn, int classID, int mapID) {
     int total = 0;
     int entries = 0;

     try {
         pstmt = conn->prepareStatement("SELECT levelHighScore FROM ScoreInfo WHERE mapID = ? AND userID IN (SELECT userID FROM UserAssociations WHERE classID = ?)");
         pstmt->setInt(1, mapID);
         pstmt->setInt(2, classID);
         rs = pstmt->executeQuery();
         while(rs->next()) {
             total = total + rs->getInt(1);
             entries++;
         }
         delete pstmt;
         if(entries == 0) {
            return 0;
         } else {
             return total/entries;
         }
     } catch (sql::SQLException &e) {
         std::cout << "Select Average Scores Failed: " << e.what() << std::endl;
         return 0;
     }
 }

void printUsage(const std::string &progname) {
    std::cout << std::endl;
    std::cout << progname << " - Run the loque game server." << std::endl;
    std::cout << std::endl;
    std::cout << "USAGE: " << progname << " [options]" << std::endl;
    std::cout << "\t-port <int>" << std::endl;
    std::cout << "\t\tport for client connections (default is " << DEFAULT_PORT << ")" << std::endl;
    std::cout << "\t-h" << std::endl;
    std::cout << "\t\tdisplay this help message" << std::endl;
}

void handleLogin(sql::Connection& dbconn,
                 std::string username, std::string userpass,
                 LoginResult& loginres) {
    loginres.userType = DNE;
    try {
        std::string query = "SELECT userID, isAdmin FROM User WHERE username = ? AND password = ?";
        std::unique_ptr<sql::PreparedStatement> pstmt(dbconn.prepareStatement(query));
        pstmt->setString(1, username);
        pstmt->setString(2, userpass);
        std::unique_ptr<sql::ResultSet> qRes(pstmt->executeQuery());
        if (qRes->next()) {
            int id = qRes->getInt(1);
            int isAdmin = qRes->getInt(2);
            loginres.userId = id;
            loginres.userType = isAdmin ? Admin : Student; 
        }
    } catch (sql::SQLException& e) {
        std::cerr << "ERROR: SQL Exception from handleLogin: " << e.what() << std::endl;
    }
}

void handleCreateAcc(sql::Connection& dbconn,
                     std::string username, std::string userpass, UserType type,
                     LoginResult& res) {
    if(type == Admin){
        addUser(&dbconn, username, userpass, true);
    } else if(type == Student){
        addUser(&dbconn, username, userpass, false);
    } else{
        // Why do we have a DNE option in request?
        return;
    }
}

void handleAddClassroom(sql::Connection& dbconn,
                        int userId, int classId, // Need className not classID. ClassID will be assigned on insertion (auto increment).
                        ActionResult& res) {
//    addClass(&dbconn, userId, className);
}

void handlePostStats(sql::Connection& dbconn,
                     int userId, const GameStats& stats,
                     ActionResult& res) {
    levelCompleted(&dbconn, userId, stats.levelId, stats.pointsScored, stats.secToComplete);
}

void handleGetUserStats(sql::Connection& dbconn,
                        int userId,
                        UserStats& stats) {
    // Not sure what I need to retrieve here.
}

void handleEnableLevel(sql::Connection& dbconn,
                       int userId, int classId, int levelId,
                       ActionResult& res) {
    // Currently can take in multiple level ids. Will change if not needed.
    std::vector<int> v;
    v.push_back(levelId);
    toggleMaps(&dbconn, classId, v, true);
}

void handleDisableLevel(sql::Connection& dbconn,
                        int userId, int classId, int levelId,
                        ActionResult& res) {
    // Currently can take in multiple level ids. Will change if not needed.
    std::vector<int> v;
    v.push_back(levelId);
    toggleMaps(&dbconn, classId, v, false);
}

void handleGetClassStats(sql::Connection& dbconn,
                         int userId, int classId,
                         ClassStats& stats) {
    // Not sure what is wanted here.
}

void handleClient(std::unique_ptr<sf::TcpSocket> client,
                  std::unique_ptr<sql::Connection> dbconn) {
    sf::Packet reqPacket;
    auto status = client->receive(reqPacket);
    if (status != sf::Socket::Done) {
        std::cerr << "ERROR: Unable to receive request from " <<
                  client->getRemoteAddress().toString() <<
                  ". receive() returned status " << status << std::endl;
        return;
    }

    sf::Packet respPacket;
    ReqType rtype;
    reqPacket >> rtype;
    switch (rtype){
    case LOGIN:
    {
        std::string username, userpass;
        reqPacket >> username >> userpass;
        LoginResult res;
        handleLogin(*dbconn, username, userpass, res);
        respPacket << res;
        break;
    }
    case CREATE_ACC:
    {
        std::string username, userpass;
        UserType type;
        reqPacket >> username >> userpass >> type;
        LoginResult res;
        handleCreateAcc(*dbconn, username, userpass, type, res);
        respPacket << res;
        break;
    }
    case ADD_CLASS:
    {
        int userId, classId;
        reqPacket >> userId >> classId;
        ActionResult res;
        handleAddClassroom(*dbconn, userId, classId, res);
        respPacket << res;
        break;
    }
    case POST_STATS:
    {
        int userId;
        GameStats stats;
        respPacket >> userId >> stats;
        ActionResult res;
        handlePostStats(*dbconn, userId, stats, res);
        respPacket << res;
        return;
    }
    case GET_USER_STATS:
    {
        int userId;
        reqPacket >> userId;
        UserStats res;
        handleGetUserStats(*dbconn, userId, res);
        respPacket << res;
        break;
    }
    case ENABLE_LEVEL:
    {
        int userId, classId, lid;
        reqPacket >> userId >> classId >> lid;
        ActionResult res;
        handleEnableLevel(*dbconn, userId, classId, lid, res);
        respPacket >> res;
        break;
    }
    case DISABLE_LEVEL:
    {
        int userId, classId, lid;
        reqPacket >> userId >> classId >> lid;
        ActionResult res;
        handleDisableLevel(*dbconn, userId, classId, lid, res);
        respPacket >> res;
        break;
    }
    case GET_CLASS_STATS:
    {
        int userId, classId;
        reqPacket >> userId >> classId;
        ClassStats res;
        handleGetClassStats(*dbconn, userId, classId, res);
        respPacket >> res;
        break; 
    }
    default:
        std::cerr << "ERROR: Unrecognized request type " << static_cast<int>(rtype) << std::endl;
        return;
    }

    status = client->send(respPacket);
    if (status != sf::Socket::Done) {
        std::cerr << "ERROR: Unable to send response to " <<
            client->getRemoteAddress().toString() <<
            ". send() returned status " << status << std::endl;
    }
}

int main(int argc, char **argv) {
    int port = DEFAULT_PORT;
    if (argc > 1) {
        for (int i = 1; i < argc; i += 2) {
            if (strcmp(argv[i], "-port") == 0 && argc > i + 1) {
                port = std::stoi(argv[i + 1]);
            }
            if (strcmp(argv[i], "-h") == 0) {
                printUsage(std::string(argv[0]));
                return 0;
            }
        }
    }

    std::cout << "Starting Loque Server on port " << port << "." << std::endl;

    sql::Driver *driver = get_driver_instance();
    sf::TcpListener listener;
    int status = listener.listen(port);
    if (status != sf::Socket::Done) {
        std::cerr << "ERROR: Unable to listen on port " << port << "." <<
                  "listen() returned status " << status << "." << std::endl;
        return 1;
    }
    std::unique_ptr<sql::Connection> dbconn(driver->connect(DB_ADDR, DB_USER, DB_PASS));
    dbconn->setSchema("3505");
    while (true) {
        std::unique_ptr<sf::TcpSocket> client(new sf::TcpSocket);
        int status = listener.accept(*client);
        if (status != sf::Socket::Done) {
            std::cerr << "ERROR: Unable to accept client connection." <<
                      "accept() returned status " << status << "." << std::endl;
            continue;
        }
        std::cout << "Accepted client connection with " <<
                  client->getRemoteAddress().toString() << std::endl;
        std::unique_ptr<sql::Connection> dbconn(driver->connect(DB_ADDR, DB_USER, DB_PASS));
        dbconn->setSchema("3505");
        std::thread worker(handleClient, std::move(client), std::move(dbconn));
        worker.detach();
    }
}
