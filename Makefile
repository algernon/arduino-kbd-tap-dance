CXX ?= g++
CXXFLAGS = -Wall -O0 -ggdb3 -std=c++11 -Wextra -IKbdBasicKey/src -IKbdTapDance/src

SOURCES = KbdBasicKey/src/BasicKey.cpp \
          KbdTapDance/src/Timer.cpp \
          KbdTapDance/src/TapDance.cpp \
          KbdTapDance/src/OneShot.cpp \
          KbdTapDance/src/Leader.cpp
HEADERS = ${SOURCES:.cpp=.h}

TEST_SOURCES = t/test-timer.cpp t/test-tap-dance.cpp \
               t/test-oneshot.cpp t/test-leader.cpp \
               t/testsuite.cpp

OBJECTS = ${SOURCES:.cpp=.o}

all: libKbdHacks.a

check: t/testsuite
	$^ -v

clean:
	rm -rf t/testsuite libKbdHacks.a ${OBJECTS} arduino

t/testsuite: ${TEST_SOURCES} libKbdHacks.a
	${CXX} ${CXXFLAGS} -Isrc -L. -o $@ ${TEST_SOURCES} -lKbdHacks -lCppUTest

libKbdHacks.a: ${OBJECTS}
	${AR} cr $@ ${OBJECTS}
	ranlib $@

%.o: %.cpp %.h
	${CXX} ${CXXFLAGS} -o $@ -c $<

arduino:
	make -C src -f arduino.mk all

.PHONY: check clean all arduino
