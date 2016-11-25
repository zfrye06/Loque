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
#include "../../shared/json.hpp"

using json = nlohmann::json;

const int DEFAULT_PORT = 5001;
const std::string DB_ADDR = "tcp://bernardcosgriff.com:3306";
const std::string DB_USER = "teamaccess";
const std::string DB_PASS = "password";
sql::Driver *driver = get_driver_instance();
sql::Connection *conn = driver->connect(DB_ADDR, DB_USER, DB_PASS);
sql::PreparedStatement *pstmt;
sql::ResultSet *rs;

/*
 * Returns true if user is succesfully added and false if not (i.e. username already in use).
 */
bool addUser(std::string username, std::string password, bool isAdmin) {
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
        return false;
    }
}
/*
 * Enables/Disables maps for classes
 */
void toggleMaps(int classID, std::vector<int> mapIDs, bool enable) {
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
void levelCompleted(int userID, int mapID, int newScore, int newTime) {
    try {
        bool update = false;
        // Grab levelsCompleted flag
        pstmt = conn->prepareStatement(
                "SELECT levelsCompleted FROM User WHERE userID = ?");
        pstmt->setInt(1, userID);
        rs = pstmt->executeQuery();
        if(rs->next()){
            // Set the flag for the level just completed
            unsigned int flag = rs->getInt(1);
            flag = flag | 1;
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
                    pstmt->(4, mapID);
                    pstmt->execute();
                }

                //Otherwise just insert a new row
            } else{
                pstmt = conn->prepareStatement("INSERT INTO ScoreInfo(userID, mapID, levelScore, completionTime) VALUES(?, ?, ?, ?)");
                pstmt->setInt(1, userID);
                pstmt->setInt(2, mapID);
                pstmt->setInt(3, newScore);
                pstmt->(4, newTime);
                pstmt->execute();
            }
        }
        else{
            std::cout << "User does not exist." << std::endl;
            return;
        }


    } catch (sql::SQLException &e) {
        std::cout << "Level completion update failed: " << e.what() << std::endl;
    }
}

/*
 * Returns all maps enabled for the specified class.
 */
std::vector<int> getEnabledMaps(int classID) {
    try {
        pstmt = conn->prepareStatement(
                "SELECT mapID FROM MapAssociations WHERE classID = ?");
        pstmt->setInt(1, classID);
        rs = pstmt->executeQuery();
        std::vector<int> v;
        while (rs->next()) {
            v.push_back(rs->getInt(1));
        }
        delete pstmt;
        return v;
    } catch (sql::SQLException &e) {
        std::cout << "Select Failed: " << e.what() << std::endl;
    }
}

/*
 *
 */
std::map<int, int> getTotalScores(int classID) {

}

/*
 *
 */
std::map<int, int> getTotalTimes(int classID) {

}

/*
 * Returns the highest level completed by the specified user
 */
int getHighestLevelCompleted(int userID) {

}

/*
 * Gets the average high score for the class with classID on the level with mapID.
 */
int getClassLevelAverage(int classID, int mapID) {

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

void handleLogin(sql::Connection &dbconn, json &req, json &resp) {
    resp["user-type"] = "ADMIN";
    resp["user-id"] = 1235;
}

void handleClient(std::unique_ptr<sf::TcpSocket> client, std::unique_ptr<sql::Connection> dbconn) {
    sf::Packet reqPacket;
    int status = client->receive(reqPacket);
    if (status != sf::Socket::Done) {
        std::cerr << "ERROR: Unable to receive request from " <<
                  client->getRemoteAddress().toString() <<
                  ". receive() returned status " << status << std::endl;
        return;
    }

    std::string rawMessage;
    reqPacket >> rawMessage;
    try {
        json req = json::parse(rawMessage);
        auto reqType = req.find("request-type");
        if (reqType == req.end()) {
            std::cerr << "ERROR: Request object contained no 'request-type' field. " <<
                      "Raw request message: " << rawMessage << std::endl;
            return;
        }
        json resp;
        if (*reqType == "LOGIN") {
            handleLogin(*dbconn, req, resp);
        } else {
            std::cerr << "ERROR: Unrecognized request type. Raw request message: " << rawMessage << std::endl;
            return;
        }
        sf::Packet respPacket;
        respPacket << resp.dump();
        status = client->send(respPacket);
        if (status != sf::Socket::Done) {
            std::cerr << "ERROR: Unable to send response. send() returned status " << status << "." << std::endl;
            return;
        }
        std::cout << "Successful response to " << client->getRemoteAddress().toString() << "." << std::endl;
    } catch (std::exception &e) {
        std::cerr << "ERROR: Unable to parse request. Exception: " << e.what() << std::endl;
    }
}

int main(int argc, char **argv) {
    conn->setSchema("3505");
    levelCompleted(1, 1, 100, 10);
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
        std::thread worker(handleClient, std::move(client), std::move(dbconn));
        worker.detach();
    }
}
