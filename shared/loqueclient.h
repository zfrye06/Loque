#ifndef LQ_LOQUECLIENT_H_
#define LQ_LOQUECLIENT_H_

#include <SFML/Network.hpp>

enum UserType {
    Admin,
    Student,
    DNE
};

struct LoginResult {
    UserType userType;
    int userId;
    bool successful() const;
};

class LoqueClient {
 private:
    sf::TcpSocket conn;
 public:

    LoqueClient(const std::string& address, int port);
    LoginResult attemptLogin(const std::string& username, const std::string& userpass);
};

#endif
