SHELL=/bin/bash

CMP = g++

FLGS = $(shell root-config --cflags) $(shell root-config --glibs)

OBJS = BCO_Ana.o main.o

main: $(OBJS)
	$(CMP) $(FLGS) -o $@ $(OBJS)

%.o: %.cc
	$(CMP) $(FLGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf main $(OBJS)
