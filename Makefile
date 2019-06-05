PROG_NAME := raytracer.exe
BIN_DIR := ./bin
SRC_DIR := ./src
OBJ_DIR := ./obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS := -lGLEW -lGL -lglfw  -L lib/ -lcommon -lassimp -lm -L/usr/local/lib -L/usr/lib64 -lIlmImf -lImath -lHalf -lIex -lIexMath -lIlmThread -pthread -lembree3
CPPFLAGS := -O3 -std=c++14 -Wall -Wextra -g
INCLUDES := -I include/ -pthread -I/usr/local/include/OpenEXR -I/usr/include/embree3

GCC := g++ 

$(BIN_DIR)/$(PROG_NAME):$(OBJ_FILES)
	$(GCC) $(CPPFLAGS) -o $@ $^ $(LDFLAGS) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(GCC) $(CPPFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/$(PROG_NAME)

	
