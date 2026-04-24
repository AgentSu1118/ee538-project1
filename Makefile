# Build configuration
CXX := g++
SRC := opinion-startercode.cpp
MODE ?= list
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic

ifeq ($(MODE),matrix)
CXXFLAGS += -DMATRIX
TARGET := opinion_matrix.exe
else ifeq ($(MODE),list)
TARGET := opinion_list.exe
else
$(error Unsupported MODE='$(MODE)'. Use MODE=matrix or MODE=list)
endif

.PHONY: all build run compare clean help

all: build

build:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: build
	./$(TARGET)

compare:
	$(MAKE) clean
	$(MAKE) run MODE=matrix > matrix_run_output.txt
	$(MAKE) run MODE=list > list_run_output.txt
	@echo "Saved outputs to matrix_run_output.txt and list_run_output.txt"

clean:
	powershell -NoProfile -Command "Remove-Item 'opinion_matrix.exe','opinion_list.exe','matrix_run_output.txt','list_run_output.txt' -ErrorAction SilentlyContinue; exit 0"

help:
	@echo "Usage:"
	@echo "  make build MODE=matrix   # build matrix version"
	@echo "  make build MODE=list     # build list version"
	@echo "  make run MODE=matrix     # build and run matrix version"
	@echo "  make run MODE=list       # build and run list version"
	@echo "  make compare             # run both and save outputs"
	@echo "  make clean               # remove generated binaries and outputs"
