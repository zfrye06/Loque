
#include <SFML/Network.hpp>
#include <iostream>
#include <mysql.h>
#include "../include/json.hpp"

using json = nlohmann::json;

constexpr int PORT = 5002;

int main() {
    std::cout << "Starting Loque Server on port " << PORT << std::endl;
    sf::TcpListener listener;
    if (listener.listen(PORT) != sf::Socket::Done) {
        std::cerr << "ERROR: Unable to listen on port " << PORT << "." << std::endl;
        return 1;
    }
    while (true) {
        sf::TcpSocket client;
        if (listener.accept(client) != sf::Socket::Done) {
            std::cerr << "ERROR: Unable to accept client connection." << std::endl;
            return 1;
        }
        std::cout << "Accepted client connection" << std::endl;

        sf::Packet message;
        if (client.receive(message) != sf::Socket::Done) {
            std::cerr << "ERROR: Unable to receive packet." << std::endl;
            continue;
        }
        std::string raw;
        message >> raw;
        std::cout << "RECEIVED: " << raw << std::endl;
        try {
            json req = json::parse(raw);
            auto packet_type = req.find("packet-type");
            if (packet_type != req.end()) {
                std::cout << "packet-type: " << *packet_type << std::endl;
            } else {
                std::cerr << "ERROR: Unable to find 'packet-type' field." << std::endl;
                continue;
            }

            json resp;
            resp["user-type"] = "ADMIN";
            sf::Packet respMessage;
            respMessage << resp.dump();
            if (client.send(respMessage) != sf::Socket::Done) {
                std::cerr << "ERROR: Unable to send response." << std::endl;
            } else {
                std::cout << "Sent response message." << std::endl;
            }
            
        } catch (std::exception &e) {
            std::cerr << "ERROR: " << e.what() << std::endl;
        }
    }
}
