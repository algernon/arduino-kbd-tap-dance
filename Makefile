CXX ?= g++
CXXFLAGS = -Wall -O0 -ggdb3 -std=c++11 -Wextra -IkbagBasicKey/src -IkbagTapDance/src -IkbagLayout/src

SOURCES = kbagBasicKey/src/BasicKey.cpp \
          kbagTapDance/src/Timer.cpp \
          kbagTapDance/src/TapDance.cpp \
          kbagTapDance/src/OneShot.cpp \
          kbagTapDance/src/Leader.cpp \
          kbagLayout/src/kbagLayout.cpp
HEADERS = ${SOURCES:.cpp=.h}

TEST_SOURCES = t/test-timer.cpp t/test-tap-dance.cpp \
               t/test-oneshot.cpp t/test-leader.cpp \
               t/test-layout.cpp \
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
