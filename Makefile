BIN = bin
SRC = src

VPATH = src:./$(SRC)

SOURCES = $(wildcard $(SRC)/*.cpp)
OBJECTS = $(patsubst $(SRC)/%.cpp, $(BIN)/%.o, $(SOURCES))
TARGET = main

$(TARGET): $(OBJECTS)
	g++ $^ -o $@

$(BIN)/%.o: %.cpp
	g++ -c $< -o $@

clean:
	rm bin/*.o
	rm $(TARGET)
