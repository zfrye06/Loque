#include <QProcess>
#include <QDir>
#include <iostream>
#include "launchgame.h"

void launchGame(int levelId, int userId) {
    QStringList args;
    args << QString::number(levelId) << QString::number(userId);
#ifdef _WIN32
    QString loqueExec("../game-client/bin/loque.exe");
#else
    QString loqueExec("../game-client/bin/loque");
#endif
#ifdef __APPLE__
    QString loqueWorkingDir("../../../../game-client");
#else
    QString loqueWorkingDir("../game-client");
#endif
    std::cout << QDir::currentPath().toStdString() << std::endl;
    QProcess::startDetached(loqueExec, args, loqueWorkingDir);
}
