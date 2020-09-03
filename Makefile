BIN = bin
SRC = src

VPATH = src:./$(SRC)

SOURCES = $(wildcard $(SRC)/*.cpp)
OBJECTS = $(patsubst $(SRC)/%.cpp, $(BIN)/%.o, $(SOURCES))
TARGET = event-motor

$(TARGET): $(OBJECTS)
	g++ $^ -o $@ -std=c++17 -g

$(BIN)/%.o: %.cpp
	g++ -c $< -o $@ -std=c++17 -g

clean:
	rm bin/*.o
	rm $(TARGET)
