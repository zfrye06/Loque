
#include <SFML/Network.hpp>
#include <iostream>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

constexpr int PORT = 5001;

int main() {
    std::cout << "Starting Loque Server on port " << PORT << std::endl;
    sf::TcpListener listener;
    if (listener.listen(5001) != sf::Socket::Done) {
        std::cerr << "ERROR: Unable to listen on port " << PORT << "." << std::endl;
        return 1;
    }
    while (true) {
        sf::TcpSocket client;
        sf::Packet packet;
        if (listener.accept(client) != sf::Socket::Done) {
            std::cerr << "ERROR: Unable to accept client connection." << std::endl;
            return 1;
        }
        std::cout << "Accepted client connection" << std::endl;
        if (client.receive(packet) != sf::Socket::Done) {
            std::cerr << "ERROR: Unable to receive packet." << std::endl;
            return 1;
        }
        std::string message;
        packet >> message;
        std::cout << "Message received: " << message << std::endl;
    }
}
