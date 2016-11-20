#ifndef SERVERUTIL_H
#define SERVERUTIL_H
#include <vector>
#include <map>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>


class ServerUtil
{
    public:
        static void toggleMaps(int classID, std::vector<int> mapIDs, bool enabled);
        static void levelCompleted(int userID, int mapID, int score, int time);
        static vector<int> getEnabledMaps(int classID);
        static map<int, int> getTotalScores(int classID);
        static map<int, int> getTotalTimes(int classID);
        static int getHighestLevelCompleted(int userID);
        static int getClassLevelAverage(int classID, int mapID);
        static int getIntFromTable(std::string table, std::string field);
        static std::string getStringFromTable(std::string table, std::string field);
    protected:
    private:

};

#endif // SERVERUTIL_H
