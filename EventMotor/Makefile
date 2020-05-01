BIN = bin
SRC = src

VPATH = src:./$(SRC)

SOURCES = $(wildcard $(SRC)/*.cpp)
OBJECTS = $(patsubst $(SRC)/%.cpp, $(BIN)/%.o, $(SOURCES))
TARGET = event-motor

$(TARGET): $(OBJECTS)
	g++ $^ -o $@ -std=c++17

$(BIN)/%.o: %.cpp
	g++ -c $< -o $@ -std=c++17

clean:
	rm bin/*.o
	rm $(TARGET)
