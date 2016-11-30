#include "playerstats.h"

PlayerStats *playerStats;

PlayerStats::PlayerStats(int userId, int levelId) : userId(userId) {
    stats.secToComplete = 0;
    stats.levelId = levelId;
    stats.pointsScored = 0;
}

void PlayerStats::startTime() {
    stats.secToComplete = 0;
    stats.pointsScored = 10;
}

void PlayerStats::setScore(double s) {
    stats.pointsScored = s;
}

double PlayerStats::getScore() {
    return stats.pointsScored;
}

void PlayerStats::sendStats() {
    client.postGameStats(userId, stats);
}

void PlayerStats::update(double dt) {
    stats.secToComplete += dt;
}
