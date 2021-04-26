all:		exclusiu

exclusiu:	cache.cc exclusiu.cc replacement_state.cpp replacement_state.h trace.h neural_module.cpp cache_state.cpp
		g++ -std=c++11 -O3 -Wall -g -o exclusiu cache.cc exclusiu.cc replacement_state.cpp neural_module.cpp cache_state.cpp -lz

clean:
	 	rm -f exclusiu
