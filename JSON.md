
#Server/Client Communication Overview

We use https://github.com/nlohmann/json, a single-header library.  The example
below demonstrates a simple request-response cycle from a client's (e.g. the
game-client or admin-portal) perspective.

```
#include <SFML/Network.hpp>
#include "include/json.hpp"

constexpr int SERVER_PORT = 5002;

int main() {

    // Connect to server.
    sf::TcpSocket socket;
    if (socket.connect("127.0.0.1", SERVER_PORT) != sf::Socket::Done) {
        std::cerr << "ERROR: Unable to connect to server." << std::endl;
        return 1;
    }

    // Send request. 
    sf::Packet toSend;
    json req;
    req["packet-type"] = "LOGIN";
    req["user-name"] = "nerd";
    req["login-hash"] = "8182384812358134234815"; 
    std::cout << "Sending request to server. Message: " << req.dump() << std::endl;
    toSend << req.dump();
    socket.send(toSend);

    // Receive response.
    sf::Packet toReceive;
    if (socket.recieve(toReceive) != sf::Socket::Done) {
       std::cerr << "ERROR: Unable to receive response from server." << std::endl;
       return 1;
    }
    std::string rawmsg;
    toReceive >> rawmsg;
    json resp = json::parse(rawmsg);
    auto userType = resp.find("user-type");
    if (userType != resp.end()) {
       std::cout << "user-type: " << *userType << std::endl;
    } else {
      std::cerr << "ERROR: Unable to find 'user-type' field in response from server." << std::endl;
      return 1;
    }

    return 0;
}
```

# API Documentation

## Login

### Client Sends
```
“{
"packet-type": “LOGIN”,
"user-name": “nerd”,
"login-hash": "SHA256 81407JFHASFU1hAsdf892#$HA",
}”
```

### Server Responds
```
“{
"user-type": “ADMIN" | "STUDENT" | "DNE”,
"user-id": integer,
}”
```
