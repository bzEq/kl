OBJECTS := testkit.o epoll.o logger.o scheduler.o lex_buffer.o \
	utf8.o buffer.o
LLVM_ASM := $(patsubst %.cc, %.ll, $(wildcard *.cc))
TESTS := $(patsubst %.cc, %, $(wildcard *_test.cc))
STATIC_LIB := libkl.a
CXX := g++
CLANGXX := clang++
CXXFLAGS := -std=c++14 -g -Wall -Werror
LDFLAGS := -L. -lkl -lpthread

.PHONY : all clean test

all: $(STATIC_LIB) $(TESTS)

$(TESTS): $(STATIC_LIB)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

llvm-ir: $(LLVM_ASM)

%.ll: %.cc
	$(CLANGXX) $(CXXFLAGS) -S -emit-llvm $<

$(TESTS): %_test: %_test.o
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

$(STATIC_LIB): $(OBJECTS)
	ar rcsv $@ $^

utf8_echo: utf8_echo.o utf8.o
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

test: $(TESTS)
	@for test in $^; do ./$$test || exit 1; done
	@echo "==== CONG! ALL TESTS PASSED."

clean:
	@rm -rvf *.o *.a *.ll *.bc
	@rm -rvf $(TESTS)
