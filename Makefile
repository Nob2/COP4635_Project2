CXX = g++
CXXFLAGS = -g -std=c++14 -pthread -Wall

main: server.o main.cpp
	$(CXX) $(CXXFLAGS) $^ -o main

client: clientManager.o client.cpp
	$(CXX) $(CXXFLAGS) $^ -o client

clean:
	rm -rf *.dSYM *.exe d*.txt main client
	$(RM) *.o *.gc*