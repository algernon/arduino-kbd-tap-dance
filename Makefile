CXX ?= g++

check: test/tap-dance
	test/tap-dance

clean:
	rm -rf test/

test/tap-dance: src/TapDance.cpp src/TapDance.h src/test/test-tap-dance.cpp
	install -d test
	${CXX} -Isrc src/TapDance.cpp src/test/test-tap-dance.cpp -o $@

.PHONY: check clean
