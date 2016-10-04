CXX ?= g++
CXXFLAGS = -Wall -O0 -ggdb3 -std=c++11

check: test/tap-dance test/oneshot test/leader
	test/tap-dance
	test/oneshot
	test/leader

clean:
	rm -rf test/

test/tap-dance: src/TapDance.cpp src/TapDance.h src/test/test-tap-dance.cpp
	install -d test
	${CXX} ${CXXFLAGS} -Isrc src/TapDance.cpp src/test/test-tap-dance.cpp -o $@

test/oneshot: src/OneShot.cpp src/OneShot.h src/test/test-oneshot.cpp
	install -d test
	${CXX} ${CXXFLAGS} -Isrc src/OneShot.cpp src/test/test-oneshot.cpp -o $@

test/leader: src/Leader.cpp src/Leader.h src/test/test-leader.cpp
	install -d test
	${CXX} ${CXXFLAGS} -Isrc src/Leader.cpp src/test/test-leader.cpp -o $@

.PHONY: check clean
