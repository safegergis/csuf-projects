#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

class EchoClient {
private:
    int clientSocket;
    std::string serverIP;
    int serverPort;

public:
    EchoClient() : clientSocket(-1), serverPort(0) {}

    void getServerInfo(int argc, char* argv[]) {
        if (argc >= 3) {
            serverIP = argv[1];
            serverPort = std::atoi(argv[2]);
        } else {
            std::cout << "Enter server IP address: ";
            std::getline(std::cin, serverIP);
            std::cout << "Enter server port number: ";
            std::cin >> serverPort;
            std::cin.ignore();
        }
    }

    bool connectToServer() {
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1) {
            std::cerr << "Error creating socket" << std::endl;
            return false;
        }

        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(serverPort);

        if (inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr) <= 0) {
            std::cerr << "Invalid server IP address" << std::endl;
            close(clientSocket);
            return false;
        }

        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Error connecting to server" << std::endl;
            close(clientSocket);
            return false;
        }

        std::cout << "Client is connected to the server." << std::endl;
        return true;
    }

    void receiveMessageLog() {
        char buffer[4096];
        memset(buffer, 0, sizeof(buffer));

        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            if (buffer[0] == '\0') {
                std::cout << "No prior message log from the server." << std::endl;
            } else {
                std::cout << "Client received message log:" << std::endl;
                std::cout << buffer;
            }
        }
    }

    void startMessaging() {
        std::string message;

        while (true) {
            std::cout << "Enter message to send to server: ";
            std::getline(std::cin, message);

            if (message == "quit") {
                send(clientSocket, message.c_str(), message.length(), 0);
                break;
            }

            std::cout << "Client sending message: \"" << message << "\"" << std::endl;
            send(clientSocket, message.c_str(), message.length(), 0);
        }

        std::cout << "Client is disconnected from the server." << std::endl;
    }

    ~EchoClient() {
        if (clientSocket != -1) {
            close(clientSocket);
        }
    }
};

int main(int argc, char* argv[]) {
    EchoClient client;
    client.getServerInfo(argc, argv);

    if (client.connectToServer()) {
        client.receiveMessageLog();
        client.startMessaging();
    }

    return 0;
}