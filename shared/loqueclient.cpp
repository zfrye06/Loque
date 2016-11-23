
#include "loqueclient.h"

bool LoginResult::successful() const {
    return userType != UserType::DNE;
}

LoqueClient::LoqueClient(const std::string& address, int port) {}

LoginResult LoqueClient::attemptLogin(const std::string& username, const std::string& userpass) {
    LoginResult res;
    res.userType = UserType::Admin;
    res.userId = 12345;
    return res;
}
