TARGET_EXEC ?= aoc

BUILD_DIR ?= ./build
SRC_DIRS ?= .

SRCS := $(shell find $(SRC_DIRS) -name "*.cpp")
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d -not -path $(BUILD_DIR) -not -path . )
INC_FLAGS := $(addprefix -I,$(INC_DIRS) ../include /usr/include) 

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP
LDFLAGS += -lgd
#Warnings etc...
CXXFLAGS += -std=c++17 -Wall -fno-loop-interchange -fno-unswitch-loops -fno-lto -funsafe-math-optimizations
#Performance flags...
CXXFLAGS += -O3 -mtune=native -march=native

# Link program
$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Build c++ sources
$(BUILD_DIR)/%.cpp.o: %.cpp makefile
	@$(MKDIR_P) $(dir $@) 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

print-%  : ; @echo $* = $($*)

.PHONY: clean

clean:
	@echo "Cleaning " $(BUILD_DIR) "and" $(TARGET_EXEC)
	@$(RM) -r $(BUILD_DIR)
	@$(RM) $(TARGET_EXEC)

-include $(DEPS)

MKDIR_P ?= mkdir -p
