CXX      := -gcc
CXXFLAGS := -pedantic-errors -Wall -Wextra -Wnarrowing -Wno-pedantic -std=c++11  -fopenmp
LDFLAGS  := -L/usr/lib -L/usr/lib64/ -L/usr/local/lib/ \
	-lstdc++ -lm -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLEW 
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
TARGET   := simpleuv
INCLUDE  := -I/usr/include -I./ -I./simpleuv -I./thirdparty -I./thirdparty/eigen -I./thirdparty/libigl/include -I./thirdparty/squeezer
SRC      :=                      \
   $(wildcard simpleuv/*.cpp)         \
   $(wildcard simpleuv/*.c)         \
   $(wildcard thirdparty/squeezer/*.c)         

OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(APP_DIR)/$(TARGET) $(OBJECTS) $(LDFLAGS)

.PHONY: all build clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*