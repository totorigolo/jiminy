CXX=g++
CXXFLAGS=-Og -g -DMAP -std=c++11
LD=g++
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lpthread
SRC=server.cpp
OBJ=$(SRC:.cpp=.o)
EXE=ExecutezMoi

all: $(EXE)

$(EXE): $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf *.o
	rm -rf $(EXE)

.PHONY: all clean

