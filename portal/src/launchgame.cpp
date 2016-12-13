#include <QProcess>
#include "launchgame.h"

void launchGame(int levelId, int userId) {
    QStringList args;
    args << QString::number(levelId) << QString::number(userId);
#ifndef WIN32
    QString loqueExec("../game-client/bin/loque");
#else
    QString loqueExec("../game-client/bin/loque.exe");
#endif
    QString loqueWorkingDir("../game-client");
    QProcess::startDetached(loqueExec, args, loqueWorkingDir);
}
