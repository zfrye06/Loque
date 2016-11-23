#ifndef LQ_LOQUECLIENT_H_
#define LQ_LOQUECLIENT_H_

#include <SFML/Network.hpp>

typedef sf::Socket::Status status;

enum UserType {
    Admin,
    Student,
    DNE
};

struct LoginResult {
    int userId;
    UserType userType;

    // Returns whether the requested user exists (e.g. type != DNE). 
    bool successful() const;
};

// Use an instance of LoqueClient to make API
// calls to a running Loque Server.
//
// All methods which make calls to the server block and
// return an sf::Socket::Status value indicating sucess
// or failure. 
class LoqueClient {
 public:

    LoqueClient(const std::string& address, int port);

    // Attempts to login the user, placing the result in the result parameter.
    status attemptLogin(const std::string& username,
                        const std::string& userpass,
                        LoginResult& result);

 private:
    
    const std::string host;
    const int port;
    sf::TcpSocket conn;

    status sendRequest(sf::Packet& req, sf::Packet& resp);
};

#endif
