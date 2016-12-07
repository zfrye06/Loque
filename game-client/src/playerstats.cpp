#include "playerstats.h"

PlayerStats *playerStats;

PlayerStats::PlayerStats(int userId, int levelId) : userId(userId) {
    stats.secToComplete = 0;
    stats.levelId = levelId;
    stats.pointsScored = 0;
}

void PlayerStats::startTime() {
    time = 0;
    stats.secToComplete = 0;
    stats.pointsScored = 0;
}

void PlayerStats::setScore(double s) {
    stats.pointsScored = s;
}

double PlayerStats::getScore() {
    return stats.pointsScored;
}

void PlayerStats::sendStats() {
    stats.secToComplete += time;
    auto status = client.postGameStats(userId, stats);
    if (status != Status::OK) {
        std::cerr << "ERROR: Unable to post player stats: " << status << std::endl;
    }
}

void PlayerStats::update(double dt) {
    time += dt;
}
