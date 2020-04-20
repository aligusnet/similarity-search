PROJECT_NAME = ssearch
CXX = g++
CXXFLAGS = -std=c++11 -O3
LDFLAGS =
LIBS =

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, obj/%.o, $(SRC))

ui: ui_normal

benchmark: benchmark_normal

test: build
	./$(PROJECT_NAME) test small

ui_small: build
	./$(PROJECT_NAME) ui small

ui_normal: build
	./$(PROJECT_NAME) ui normal

ui_gigantic: build
	./$(PROJECT_NAME) ui gigantic


benchmark_small: build
	./$(PROJECT_NAME) benchmark small

benchmark_normal: build
	./$(PROJECT_NAME) benchmark normal

benchmark_gigantic: build
	./$(PROJECT_NAME) benchmark gigantic


run:
	./$(PROJECT_NAME)

build: $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LIBS) $(OBJ) -o $(PROJECT_NAME)

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(PROJECT_NAME)
