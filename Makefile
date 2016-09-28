CXX ?= g++
CXXFLAGS = -Wall

check: test/tap-dance test/oneshot
	test/tap-dance
	test/oneshot

clean:
	rm -rf test/

test/tap-dance: src/TapDance.cpp src/TapDance.h src/test/test-tap-dance.cpp
	install -d test
	${CXX} ${CXXFLAGS} -Isrc src/TapDance.cpp src/test/test-tap-dance.cpp -o $@

test/oneshot: src/OneShot.cpp src/OneShot.h src/test/test-oneshot.cpp
	install -d test
	${CXX} ${CXXFLAGS} -Isrc src/OneShot.cpp src/test/test-oneshot.cpp -o $@

.PHONY: check clean
