

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <SFML/Network.hpp>
#include <iostream>
#include <memory>
#include <string.h>
#include <thread>
#include "../../shared/json.hpp"

using json = nlohmann::json;

constexpr int DEFAULT_PORT = 5002;

void printUsage(const std::string& progname) {
    std::cout << std::endl;
    std::cout << progname << " - Run the loque game server." << std::endl;
    std::cout << std::endl;
    std::cout << "USAGE: " << progname << " [options]" << std::endl;
    std::cout << "\t-port <int>" << std::endl;
    std::cout << "\t\tport for client connections (default is " << DEFAULT_PORT << ")" << std::endl;
    std::cout << "\t-h" << std::endl;
    std::cout << "\t\tdisplay this help message" << std::endl;
}

void handleLogin(json& req, json& resp) {
    std::cout << "TODO: Handle logins!" << std::endl;    
}

void handleClient(std::unique_ptr<sf::TcpSocket> client) {
    sf::Packet reqPacket;
    int status = client->receive(reqPacket);
    if (status != sf::Socket::Done) {
        std::cerr << "ERROR: Unable to receive request from " <<
            client->getRemoteAddress().toString() <<
            ". receive() returned status " << status << std::endl;
        return;
    }

    std::string rawMessage;
    reqPacket >> rawMessage;
    try {
        json req = json::parse(rawMessage);
        auto reqType = req.find("request-type");
        if (reqType == req.end()) {
            std::cerr << "ERROR: Request object contained no 'request-type' field. " <<
                "Raw request message: " << rawMessage << std::endl;
            return;
        }
        json resp;
        if (*reqType == "LOGIN") {
            handleLogin(req, resp);
        } else {
            std::cerr << "ERROR: Unrecognized request type. Raw request message: " << rawMessage << std::endl;
            return;
        }
        sf::Packet respPacket;
        respPacket << resp.dump();
        status = client->send(respPacket);
        if (status != sf::Socket::Done) {
            std::cerr << "ERROR: Unable to send response. send() returned status " << status << "." << std::endl;
            return;
        }
    } catch (std::exception& e) {
        std::cerr << "ERROR: Unable to parse request. Exception: " << e.what() << std::endl;
    }
}

int main(int argc, char **argv) {
    int port = DEFAULT_PORT;
    if (argc > 1) {
        for (int i = 1; i < argc; i += 2) {
            if (strcmp(argv[i], "-port") == 0 && argc > i + 1) {
                port = std::stoi(argv[i+1]);
            }
            if (strcmp(argv[i], "-h") == 0) {
                printUsage(std::string(argv[0]));
                return 0;
            }
        }
    }
    std::cout << "Starting Loque Server on port " << port << "." << std::endl;
    sql::Driver *driver = get_driver_instance();
    sf::TcpListener listener;
    int status = listener.listen(port);
    if (status != sf::Socket::Done) {
        std::cerr << "ERROR: Unable to listen on port " << port << "." <<
           "listen() returned status " << status << "." << std::endl;
        return 1;
    }
    while (true) {
        std::unique_ptr<sf::TcpSocket> client(new sf::TcpSocket);
        int status = listener.accept(*client);
        if (status != sf::Socket::Done) {
            std::cerr << "ERROR: Unable to accept client connection." <<
               "accept() returned status " << status << "." << std::endl;
            continue;
        }
        std::cout << "Accepted client connection with " <<
            client->getRemoteAddress().toString() << std::endl;
        std::thread worker(handleClient, std::move(client));
        worker.detach();
    }
}
