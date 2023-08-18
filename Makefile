CXX = g++
CXXFLAGS = -std=c++17 -DWITH_SIGNALOID
SRC = src/main.cpp
TARGET = matrix

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
