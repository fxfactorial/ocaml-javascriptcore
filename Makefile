# -*- makefile -*-

objects := dump_info.o
exec := program
flags := -std=c++11 -framework Foundation -stdlib=libc++

all:${objects}
	clang++ ${objects} ${flags} -o ${exec}

%.o: %.mm
	clang++ ${flags} -c $<

.PHONY: clean

clean:
	@rm -rf *.o ${exec}
