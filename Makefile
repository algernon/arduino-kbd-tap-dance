CXX ?= g++
CXXFLAGS = -Wall -O0 -ggdb3 -std=c++11 -Wextra

SOURCES = src/TapDance.cpp src/OneShot.cpp src/Leader.cpp
HEADERS = ${SOURCES:.cpp=.h}

OBJECTS = ${SOURCES:.cpp=.o}

all: libtapdance.a

check: test/tap-dance test/oneshot test/leader
	test/tap-dance
	test/oneshot
	test/leader

clean:
	rm -rf test libtapdance.a ${OBJECTS}

test:
	install -d test

test/%: src/test/test-%.cpp test libtapdance.a
	${CXX} ${CXXFLAGS} -Isrc -L. -o $@ $< -ltapdance

libtapdance.a: ${OBJECTS}
	${AR} cr $@ ${OBJECTS}
	ranlib $@

%.o: %.cpp %.h
	${CXX} ${CXXFLAGS} -Isrc -L. -o $@ -c $<

.PHONY: check clean all
