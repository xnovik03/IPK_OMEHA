# Nastavení proměnných
CXX = g++
CXXFLAGS = -std=c++11
SOURCES = IPK_OMEGA.cpp NetworkInterface.cpp PortRangeParser.cpp TCPscanner.cpp UDPscanner.cpp Packetsender.cpp ResultFormatter.cpp
OBJECTS = $(SOURCES:.cpp=.o)  # Vytvoří seznam objektových souborů
EXEC = ipk-l4-scan

# Kompilace projektu
all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC)

# Kompilace jednotlivých zdrojových souborů
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Čistící cíl
clean:
	rm -f $(EXEC) $(OBJECTS)

