CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

all: echo_server echo_client

echo_server: echo_server.cpp
	$(CXX) $(CXXFLAGS) -o echo_server echo_server.cpp

echo_client: echo_client.cpp
	$(CXX) $(CXXFLAGS) -o echo_client echo_client.cpp

clean:
	rm -f echo_server echo_client

.PHONY: all clean