# alaasalah142@gmail.com
# --------------------------------------------------------------------
# Toolchain & flags
# --------------------------------------------------------------------
CXX       := clang++
CXXFLAGS  := -std=c++23 -Wall -Wextra -Iinclude

# --------------------------------------------------------------------
# Source file discovery
# --------------------------------------------------------------------
SRC_DIR   := source
SRCS      := $(wildcard $(SRC_DIR)/*.cpp)

# Engine (core) sources
CORE_SRCS := $(filter-out $(SRC_DIR)/GUI.cpp, $(wildcard $(SRC_DIR)/*.cpp))
CORE_OBJS := $(patsubst %.cpp,%.o,$(CORE_SRCS))

# GUI-specific sources
GUI_SRCS := $(SRC_DIR)/GUI.cpp
GUI_OBJS := $(GUI_SRCS:.cpp=.o)

TEST_DIR  := tests
TESTS     := test_player test_roles test_game_core test_blocks

# --------------------------------------------------------------------
# Default target: build tests and demo
# --------------------------------------------------------------------
all: $(TESTS) demo

# --------------------------------------------------------------------
# Build test executables
# --------------------------------------------------------------------
tests: $(TESTS)

# --------------------------------------------------------------------
# Build engine object files
# --------------------------------------------------------------------
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build main.o
main.o: main.cpp include/GUI.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

# --------------------------------------------------------------------
# Test executables
# --------------------------------------------------------------------
$(TESTS): %: $(TEST_DIR)/%.cpp $(CORE_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# --------------------------------------------------------------------
# GUI demo
# --------------------------------------------------------------------
demo: main.o $(CORE_OBJS) $(GUI_OBJS)
	$(CXX) $(CXXFLAGS) main.o $(CORE_OBJS) $(GUI_OBJS) -o demo \
	        -lsfml-graphics -lsfml-window -lsfml-system

# --------------------------------------------------------------------
# Memory‐check targets with Valgrind
# --------------------------------------------------------------------
VG       := valgrind
VGFLAGS  := --leak-check=full --show-leak-kinds=all --track-origins=yes

# Run the demo under Valgrind
memcheck-demo: demo
	$(VG) $(VGFLAGS) ./demo

# Run all test suites under Valgrind
memcheck-tests: $(TESTS)
	for t in $(TESTS); do \
		echo "=== memcheck $$t ==="; \
		$(VG) $(VGFLAGS) ./$$t; \
	done

clean:
	rm -f main.o $(CORE_OBJS) $(GUI_OBJS) $(TESTS) demo

.PHONY: all demo tests clean