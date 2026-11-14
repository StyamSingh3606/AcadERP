CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Werror -Iinclude

TARGET = acaderp
SOURCES = $(wildcard src/*.cpp)

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
