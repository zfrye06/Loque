#include <QProcess>
#include "launchgame.h"

void launchGame(int levelId, int userId) {
    QStringList args;
    args << QString::number(levelId) << QString::number(userId);
    QString loqueExec("/Users/asteele/Sandbox/edu-app-unescaped-characters/game-client/bin/loque");
    QString loqueWorkingDir("/Users/asteele/Sandbox/edu-app-unescaped-characters/game-client");
    QProcess::startDetached(loqueExec, args, loqueWorkingDir);
}
