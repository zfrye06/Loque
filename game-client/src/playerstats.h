#ifndef LQ_PLAYERSTATS_H_
#define LQ_PLAYERSTATS_H_

#include <exception>

#include "../shared/loqueclient.h"

class PlayerStats {
private:
    double time;
    double score;
    int mapId;
    int userId;
    LoqueClient* client;
public:
    void startTime();
    PlayerStats( int userId, int mapId );
    ~PlayerStats();
    void sendStats();
    void setScore(double s);
    void update(double dt);
    double getScore();
};

extern PlayerStats* playerStats;

#endif
