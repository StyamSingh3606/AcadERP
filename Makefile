CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Werror -Iinclude

SOURCES = $(wildcard src/*.cpp)
TARGET = acaderp

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)