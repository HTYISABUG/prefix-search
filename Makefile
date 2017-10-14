TESTS = \
    test_cpy \
    test_ref

CPP = g++
CFLAGS = -Wall -Werror -g
CPPFLAGS = -Wall -Werror -g -std=c++0x

GENFILE= append.txt \
		 data.txt \
		 ldata.txt \
		 len.txt \
		 high.txt \
		 prefix_search.txt \
		 prefix_search_avg.txt

EXEC = append \
	   prefix_search

# Control the build verbosity                                                   
ifeq ("$(VERBOSE)","1")
    Q :=
    VECHO = @true
else
    Q := @
    VECHO = @printf
endif

GIT_HOOKS := .git/hooks/applied

.PHONY: all clean

all: $(GIT_HOOKS) $(TESTS)

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

OBJS_LIB = \
    tst.o

OBJS := \
    $(OBJS_LIB) \
    test_cpy.o \
    test_ref.o

deps := $(OBJS:%.o=.%.o.d)

test_%: test_%.o $(OBJS_LIB)
	$(VECHO) "  LD\t$@\n"
	$(Q)$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(VECHO) "  CC\t$@\n"
	$(Q)$(CC) -o $@ $(CFLAGS) -c -MMD -MF .$@.d $<

append: append.cpp
	$(CPP) $(CPPFLAGS) $@.cpp -o $@

prefix_search: prefix_search.cpp
	$(CPP) $(CPPFLAGS) $@.cpp -o $@

bench: $(TESTS) $(EXEC)
	sudo chrt -f 99 taskset -c 1 ./test_ref --bench
	./append
	./prefix_search

plot: bench
	gnuplot scripts/*.gp

clean:
	$(RM) $(TESTS) $(OBJS) $(EXEC)
	$(RM) $(deps)
	$(RM) *.png $(GENFILE)

-include $(deps)
