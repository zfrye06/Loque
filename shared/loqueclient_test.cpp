
#include <iostream>
#include <stdlib.h>
#include "loqueclient.h"

void testLogin() {
    LoqueClient client;
    LoginResult res;
    auto status = client.attemptLogin("notauser", "notauserpass", res);
    if (status != OK) {
        std::cerr << status << std::endl;
        exit(1);
    }
    if (res.userType != DNE) {
        std::cerr << "testLogin -  Error: Nonexistent user has unexpected user type. " <<
            res.userType;
        exit(1); 
    }
    status = client.createAccount("newusera", "newuserapass", UserType::STUDENT, res);
    if (status != OK) {
        std::cerr << status << std::endl;
        exit(1);
    }
    if (res.userType != UserType::STUDENT) {
        std::cerr << "testLogin - Error: Created user of wrong type. " << res.userType << std::endl;
        exit(1);
    }
    auto userId = res.userId;
    LoginResult res2;
    status = client.attemptLogin("newusera", "newuserapass", res2);
    if (status != OK) {
        std::cerr << status << std::endl;
        exit(1);
    }
    if (res2.userType != UserType::STUDENT) {
        std::cerr << "testLogin - Error: Logged in user has wrong type." <<
            res2.userType << std::endl;
        exit(1);
    }
    if (res2.userId != userId) {
        std::cerr << "testLogin - Error: Wrong user id. Got " << res2.userId <<
            ". Expected " << userId << std::endl;
        exit(1);
    }
}

int main() {
    testLogin();
    return 0; 
}
