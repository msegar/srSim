
CXX = clang++
CXXFLAGS = -std=c++11 -stdlib=libc++
EXECS = srSim

SRC = $(wildcard src/*.cpp)

all: $(EXECS)

srSim: $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $(EXECS)
clean:
	/bin/rm -f $(EXECS)

