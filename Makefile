# Definice kompilátoru a příkazů
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

# Seznam všech zdrojových souborů
SRC = IPK_OMEGA.cpp NetworkInterface.cpp Packetsender.cpp PortRangeParser.cpp ResultFormatter.cpp TCPScanner.cpp UDPScanner.cpp scanner.cpp

# Seznam objektových souborů 
OBJ = $(SRC:.cpp=.o)

# Název výsledného binárního souboru
TARGET = ipk-l4-scan


# Pravidlo pro kompilaci
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

# Pravidlo pro vytvoření objektového souboru
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ) $(TARGET)


rebuild: clean $(TARGET)
