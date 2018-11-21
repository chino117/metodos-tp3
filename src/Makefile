CPP = g++
FLAGS = -std=c++11 -Wall -Wfloat-equal -O3 -g 
DEPS = generadorRayos.hpp matriz_aux.hpp normas_aux.hpp ppmloader/ppmloader.h IOUtils.hpp eigen.hpp
OBJ = tp3.o ppmloader/ppmloader.o

all: tp3

tp3: $(OBJ)
	$(CPP) $(FLAGS) -o $@ $^

%.o: %.cpp $(DEPS)
	$(CPP) $(FLAGS) -c -o $@ $<

clean:
	rm -f *.o
	rm -f tp3
