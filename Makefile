# Build configuration
CXX := g++
SRC := opinion-startercode.cpp
MODE ?= list
DATA ?= short
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic

ifeq ($(MODE),matrix)
CXXFLAGS += -DMATRIX
TARGET := opinion_matrix.exe
else ifeq ($(MODE),list)
TARGET := opinion_list.exe
else
$(error Unsupported MODE='$(MODE)'. Use MODE=matrix or MODE=list)
endif

ifeq ($(DATA),large)
CXXFLAGS += -DUSE_LARGE_DATA
else ifeq ($(DATA),short)
# default short dataset, no extra macro needed
else
$(error Unsupported DATA='$(DATA)'. Use DATA=short or DATA=large)
endif

.PHONY: all build run compare clean help

all: build

build:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: build
	./$(TARGET)

compare:
	$(MAKE) clean
	$(MAKE) run MODE=matrix DATA=large > matrix_run_output.txt
	$(MAKE) run MODE=list DATA=large > list_run_output.txt
	@echo "Saved outputs to matrix_run_output.txt and list_run_output.txt"

clean:
	powershell -NoProfile -Command "Remove-Item 'opinion_matrix.exe','opinion_list.exe','matrix_run_output.txt','list_run_output.txt' -ErrorAction SilentlyContinue; exit 0"

help:
	@echo "Usage:"
	@echo "  make build MODE=matrix   # build matrix version"
	@echo "  make build MODE=list     # build list version"
	@echo "  make run MODE=matrix DATA=short   # default short dataset"
	@echo "  make run MODE=list DATA=short     # default short dataset"
	@echo "  make run MODE=list DATA=large     # explicitly run large dataset"
	@echo "  make compare                       # runs both modes on large dataset"
	@echo "  make clean               # remove generated binaries and outputs"
