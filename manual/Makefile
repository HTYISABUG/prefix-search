CPP = g++
CPPFLAGS = -Wall -Werror -g -std=c++0x

GENFILE= data.txt \
		 ldata.txt \
		 len.txt \
		 high.txt

EXEC = append

.PHONY: all clean
all: append

append: append.cpp append.txt
	$(CPP) $(CPPFLAGS) $@.cpp -o $@

plot: append
	./append > /dev/null
	gnuplot append.gp
	gnuplot len.gp
	gnuplot high.gp

clean:
	$(RM) $(EXEC) $(GENFILE) *.png
