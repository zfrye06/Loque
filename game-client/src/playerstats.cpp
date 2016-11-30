#include "playerstats.h"

PlayerStats* playerStats;

PlayerStats::PlayerStats(int userId, int mapId) {
    client = new LoqueClient();
    time = 0;
    score = 0;
    this->mapId = mapId;
    this->userId = mapId;
}

PlayerStats::~PlayerStats() {
    delete client;
}

void PlayerStats::startTime() {
    time = 0;
    score = 10;
}

void PlayerStats::setScore(double s) {
    score = s;
}

double PlayerStats::getScore() {
    return score;
}

void PlayerStats::sendStats() {
    GameStats stats;
    stats.pointsScored = score;
    stats.levelId = mapId;
    stats.secToComplete = time;
    //ActionResult r;
    //client->postGameStats(userId,stats,r);
    //if (!r.success) {
    //    throw std::runtime_error(r.reason);
    //}
}

void PlayerStats::update( double dt ) {
    time += dt;
}
