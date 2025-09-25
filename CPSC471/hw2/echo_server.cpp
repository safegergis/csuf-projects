#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

class EchoServer {
private:
    std::vector<std::pair<std::string, std::string>> messageLog;
    int serverSocket;
    int port;
    std::string serverIP;

public:
    EchoServer() : serverSocket(-1), port(0) {}

    void getPort(int argc, char* argv[]) {
        if (argc > 1) {
            port = std::atoi(argv[1]);
        } else {
            std::cout << "Enter port number: ";
            std::cin >> port;
        }

        if (port < 10000 || port > 20000) {
            std::cout << "Warning: Recommended to use ports between 10000-20000" << std::endl;
        }
    }

    std::string getLocalIP() {
        char hostname[256];
        if (gethostname(hostname, sizeof(hostname)) == 0) {
            struct hostent* host = gethostbyname(hostname);
            if (host != nullptr) {
                return std::string(inet_ntoa(*((struct in_addr*)host->h_addr_list[0])));
            }
        }
        return "127.0.0.1";
    }

    void startServer() {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1) {
            std::cerr << "Error creating socket" << std::endl;
            return;
        }

        int opt = 1;
        if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            std::cerr << "Error setting socket options" << std::endl;
            close(serverSocket);
            return;
        }

        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);

        if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Error binding socket" << std::endl;
            close(serverSocket);
            return;
        }

        serverIP = getLocalIP();
        std::cout << "Server is at address: " << serverIP << std::endl;
        std::cout << "Server is using port: " << port << std::endl;
        std::cout << std::endl;

        if (listen(serverSocket, 5) < 0) {
            std::cerr << "Error listening on socket" << std::endl;
            close(serverSocket);
            return;
        }

        while (true) {
            struct sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof(clientAddr);

            int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
            if (clientSocket < 0) {
                std::cerr << "Error accepting connection" << std::endl;
                continue;
            }

            std::string clientIP = inet_ntoa(clientAddr.sin_addr);
            std::cout << "The client at " << clientIP << " has connected to the server" << std::endl;

            handleClient(clientSocket, clientIP);
        }
    }

    void handleClient(int clientSocket, const std::string& clientIP) {
        if (messageLog.empty()) {
            std::cout << "The server has no message log to send to this client" << std::endl;
            char nullChar = '\0';
            send(clientSocket, &nullChar, 1, 0);
        } else {
            std::cout << "The server sent the message log to this client" << std::endl;
            std::string logData;
            for (const auto& entry : messageLog) {
                logData += entry.first + ": " + entry.second + "\n";
            }
            send(clientSocket, logData.c_str(), logData.length(), 0);
        }

        char buffer[1024];
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

            if (bytesReceived <= 0) {
                break;
            }

            std::string message(buffer);
            message = message.substr(0, message.find('\n'));

            if (message == "quit") {
                break;
            }

            std::cout << "Client " << clientIP << " sent: \"" << message << "\"" << std::endl;
            messageLog.push_back({clientIP, message});
        }

        std::cout << "The client at " << clientIP << " has disconnected from the server" << std::endl;
        close(clientSocket);
    }

    ~EchoServer() {
        if (serverSocket != -1) {
            close(serverSocket);
        }
    }
};

int main(int argc, char* argv[]) {
    EchoServer server;
    server.getPort(argc, argv);
    server.startServer();
    return 0;
}