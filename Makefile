CXX ?= g++

check: kbd-test
	./kbd-test

clean:
	rm -f kbd-test

kbd-test: src/TapDance.cpp src/TapDance.h src/test.cpp
	${CXX} -I. src/TapDance.cpp src/test.cpp -o kbd-test

.PHONY: check clean
