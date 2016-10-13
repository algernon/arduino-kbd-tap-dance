CXX ?= g++
CXXFLAGS = -Wall -O0 -ggdb3 -std=c++11 -Wextra

SOURCES = src/TimeOut.cpp src/TapDance.cpp src/OneShot.cpp src/Leader.cpp
HEADERS = ${SOURCES:.cpp=.h}

TEST_SOURCES = src/test/test-timer.cpp src/test/test-tap-dance.cpp \
               src/test/test-oneshot.cpp src/test/test-leader.cpp \
               src/test/testsuite.cpp

OBJECTS = ${SOURCES:.cpp=.o}

all: libtapdance.a

check: test/testsuite
	$^

clean:
	rm -rf test libtapdance.a ${OBJECTS}

test:
	install -d test

test/testsuite: ${TEST_SOURCES} test libtapdance.a
	${CXX} ${CXXFLAGS} -Isrc -L. -o $@ ${TEST_SOURCES} -ltapdance

libtapdance.a: ${OBJECTS}
	${AR} cr $@ ${OBJECTS}
	ranlib $@

%.o: %.cpp %.h
	${CXX} ${CXXFLAGS} -Isrc -L. -o $@ -c $<

.PHONY: check clean all
