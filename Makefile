# Nastavení proměnných
CXX = g++
CXXFLAGS = -std=c++11  
SOURCES = IPK_OMEGA.cpp NetworkInterface.cpp PortRangeParser.cpp TCPscanner.cpp UDPscanner.cpp Packetsender.cpp ResultFormatter.cpp  
EXEC = ipk-l4-scan  

# Kompilace projektu
all: $(EXEC)

$(EXEC): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXEC)

# Čistící cíl
clean:
	rm -f $(EXEC)
