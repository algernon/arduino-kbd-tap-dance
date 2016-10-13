CXX ?= g++
CXXFLAGS = -Wall -O0 -ggdb3 -std=c++11 -Wextra

SOURCES = src/TimeOut.cpp src/TapDance.cpp src/OneShot.cpp src/Leader.cpp
HEADERS = ${SOURCES:.cpp=.h}

TEST_SOURCES = t/test-timer.cpp t/test-tap-dance.cpp \
               t/test-oneshot.cpp t/test-leader.cpp \
               t/testsuite.cpp

OBJECTS = ${SOURCES:.cpp=.o}

all: libtapdance.a

check: t/testsuite
	$^

clean:
	rm -rf t/testsuite libtapdance.a ${OBJECTS}

t/testsuite: ${TEST_SOURCES} libtapdance.a
	${CXX} ${CXXFLAGS} -Isrc -L. -o $@ ${TEST_SOURCES} -ltapdance

libtapdance.a: ${OBJECTS}
	${AR} cr $@ ${OBJECTS}
	ranlib $@

%.o: %.cpp %.h
	${CXX} ${CXXFLAGS} -Isrc -L. -o $@ -c $<

arduino:
	make -C src -f arduino.mk all

.PHONY: check clean all arduino
