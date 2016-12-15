#loque

## Made by The Unescaped Characters.
---
## Installation instructions:
1. You'll want to install this by compiling the source, though *some* precompiled binaries can be found in a tag on the [releases](https://github.com/University-of-Utah-CS3505/edu-app-unescaped-characters/releases) page. (The Windows build doesn't have the server, Linux build may be missing libraries since it expects you to compile it, and Mac may or may not have precompiled binaries whatsoever.)
2. First install the listed dependencies:
    *. [tmxlite](https://github.com/fallahn/tmxlite)
    *. [sfml](http://www.sfml-dev.org/)
    *. [mysqlconnector-c++](https://dev.mysql.com/downloads/connector/cpp/)
    *. [qt-creator](https://www.qt.io/ide/)
3. Clone this repository. `git clone git@github.com:University-of-Utah-CS3505/edu-app-unescaped-characters.git`
4. Configure and build the game-client and server with cmake.
```sh
cd game-client
cmake ./
make
cd ../server
cmake ./
make
```
5. Finally open portal/portal.pro with qtcreator and build it.
6. Arrange the files to look like this:
```sh
 naelstrof@Loque  ~/Documents/loque  tree
 .
 ├── game-client
 │   ├── assets
 │   └── loque.exe
 ├── portal
 │   └── loque.exe
 └── server
     └── server.exe
```
7. You're done!

## Running Loque
1. First start up the server with `./server/server`.
2. Then run the portal with `./portal/loque`.
3. Have fun!

## Troubleshooting
> I don't have a server.exe!

I couldn't get the server to cross compile to windows, sorry!

You can still run the game by opening power-shell and typing
```powershell
cd game-client
./loque <mapid> <userid>
```
`mapid` is an integer from 0 to 5, and `userid` can be any bogus integer.

> I can't get it to compile!

Tough luck, if your package manager doesn't have the packages you need, you can try using the precompiled binaries, but they're mostly incomplete.
