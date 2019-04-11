SRC_DIR := .
OBJ_DIR := ./obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS := -lGLEW -lGL -lglfw  -L../lib/ -lcommon -lassimp
CPPFLAGS := -O3 -std=c++14 -Wall -Wextra -g
INCLUDES := -I../include/
GCC := g++ 

main.out: $(OBJ_FILES)
	$(GCC) -o $@ $^ $(LDFLAGS) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(GCC) $(CPPFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f $(OBJ_DIR)/*.o *.out

	
