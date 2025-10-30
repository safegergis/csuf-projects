#include <algorithm>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <mutex>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>

class EchoServer {
private:
  struct ClientInfo {
    int socket;
    std::string ip;
  };

  std::vector<std::pair<std::string, std::string>> messageLog;
  std::mutex messageLogMutex;
  std::vector<ClientInfo> connectedClients;
  std::mutex clientsMutex;
  int serverSocket;
  int port;
  std::string serverIP;

public:
  EchoServer() : serverSocket(-1), port(0) {}

  void getPort(int argc, char *argv[]) {
    if (argc > 1) {
      port = std::atoi(argv[1]);
    } else {
      std::cout << "Enter port number: ";
      std::cin >> port;
    }
  }

  std::string getLocalIP() {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
      struct hostent *host = gethostbyname(hostname);
      if (host != nullptr) {
        return std::string(
            inet_ntoa(*((struct in_addr *)host->h_addr_list[0])));
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
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) <
        0) {
      std::cerr << "Error setting socket options" << std::endl;
      close(serverSocket);
      return;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) <
        0) {
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

      int clientSocket =
          accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
      if (clientSocket < 0) {
        std::cerr << "Error accepting connection" << std::endl;
        continue;
      }

      std::string clientIP = inet_ntoa(clientAddr.sin_addr);
      std::cout << "The client at " << clientIP
                << " has connected to the server" << std::endl;

      std::thread clientThread(&EchoServer::handleClient, this, clientSocket,
                               clientIP);
      clientThread.detach();
    }
  }

  void broadcastToOthers(const std::string &message, int excludeSocket) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (const auto &client : connectedClients) {
      if (client.socket != excludeSocket) {
        send(client.socket, message.c_str(), message.length(), 0);
      }
    }
  }

  void broadcastToAll(const std::string &message) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (const auto &client : connectedClients) {
      send(client.socket, message.c_str(), message.length(), 0);
    }
  }

  void handleClient(int clientSocket, const std::string &clientIP) {
    {
      std::lock_guard<std::mutex> lock(clientsMutex);
      connectedClients.push_back({clientSocket, clientIP});
    }

    {
      std::lock_guard<std::mutex> lock(messageLogMutex);
      if (messageLog.empty()) {
        std::cout << "The server has no message log to send to this client"
                  << std::endl;
        char nullChar = '\0';
        send(clientSocket, &nullChar, 1, 0);
      } else {
        std::cout << "The server sent the message log to this client"
                  << std::endl;
        std::string logData;
        for (const auto &entry : messageLog) {
          logData += entry.first + ": " + entry.second + "\n";
        }
        send(clientSocket, logData.c_str(), logData.length(), 0);
      }
    }

    {
      std::lock_guard<std::mutex> lock(clientsMutex);
      std::string clientList = "Currently connected clients:\n";
      for (const auto &client : connectedClients) {
        clientList += client.ip + "\n";
      }
      send(clientSocket, clientList.c_str(), clientList.length(), 0);
    }

    std::string connectMsg =
        "Client " + clientIP + " has connected to the server\n";
    broadcastToOthers(connectMsg, clientSocket);

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

      std::cout << "Client " << clientIP << " sent: \"" << message << "\""
                << std::endl;

      {
        std::lock_guard<std::mutex> lock(messageLogMutex);
        messageLog.push_back({clientIP, message});
      }

      std::string broadcastMsg = clientIP + ": " + message + "\n";
      broadcastToOthers(broadcastMsg, clientSocket);
    }

    {
      std::lock_guard<std::mutex> lock(clientsMutex);
      connectedClients.erase(
          std::remove_if(connectedClients.begin(), connectedClients.end(),
                         [clientSocket](const ClientInfo &client) {
                           return client.socket == clientSocket;
                         }),
          connectedClients.end());
    }

    std::cout << "The client at " << clientIP
              << " has disconnected from the server" << std::endl;

    std::string disconnectMsg =
        "Client " + clientIP + " has disconnected from the server\n";
    broadcastToAll(disconnectMsg);

    close(clientSocket);
  }

  ~EchoServer() {
    if (serverSocket != -1) {
      close(serverSocket);
    }
  }
};

int main(int argc, char *argv[]) {
  EchoServer server;
  server.getPort(argc, argv);
  server.startServer();
  return 0;
}
