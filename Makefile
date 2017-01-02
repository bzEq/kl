OBJECTS := testkit.o epoll.o logger.o
TESTS := $(patsubst %.cc, %, $(wildcard *_test.cc))
STATIC_LIB := libkl.a
CXX := clang++
CXXFLAGS := -std=c++14 -g -Wall -O2
LDFLAGS := -L. -lkl -lpthread

.PHONY : all clean test

all: $(STATIC_LIB)

$(TESTS): $(STATIC_LIB)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

$(TESTS): %_test: %_test.o
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

$(STATIC_LIB): $(OBJECTS)
	ar rcsv $@ $^

test: $(TESTS)
	@for test in $^; do ./$$test || exit 1; done
	@echo "==== CONG! ALL TESTS PASSED."

clean:
	@rm -rvf *.o *.a
	@rm -rvf $(TESTS)
