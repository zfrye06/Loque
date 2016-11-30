
#Using the Loque Client

 - Include the loqueclient header.
 - Compile the loqueclient.cpp file with your build. Be sure to link
   with the sfml/network and sfml/system libraries.

Example:
```c++

#include <iostream>
#include "../shared/loqueclient.h"

int main() {
    LoqueClient client;
    LoginResult loginRes;
    auto status = client.attemptLogin("userA", "password", loginRes);
    if (status != OK) {
       std::cerr << "ERROR: Unable to make request: " << status << std::endl;
       return 1;
    }
    if (loginRes.userType == DNE) {
       std::cerr "ERROR: Couldn't login user. Invalid username/pass." << std::endl;
       return 1;
    }
    std::vector<int> enabledLevels;
    status = client.getEnabledLevels(loginRes.userId, enabledLevels);
    if (status != OK) {
       std::cerr << "ERROR: Unable to make request: " << status << std::endl;
       return 1;
    }
    std::cout << "Enabled levels:" << std::endl;
    for (auto levelId : enabledLevels) {
        std::cout << "\t" << levelId << std::endl;
    }
    return 0; 
}
```

