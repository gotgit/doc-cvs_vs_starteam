CC       = gcc
#CWARN    = -Wstrict-prototypes -Wall -Wunused 
CWARN    = -Wunused 
CDEFS    = -D_DEBUG
CINCS    = -I/usr/local/include 
CFLAGS     = $(CWARN) $(CDEFS) $(CINCS) -g -O2
CXXFLAGS = $(CFLAGS)
LDFLAGS = -pthread -L/usr/local/lib
PROGRAM = steam2cvs


# Donot change below here. 
SRCS     = $(wildcard *.cpp) $(wildcard *.c)
OBJS    = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SRCS)))
DEPEND    = makedepend -- $(CFLAGS) --
CTAGS    = ctags

all: $(PROGRAM) 

install:
	-cp -f $(PROGRAM)  /usr/local/bin/

$(PROGRAM):$(OBJS)


depend:$(SRCS)
	$(DEPEND) $(SRCS)


tags:$(SRCS)
	$(CTAGS) $(SRCS)


clean:
	-rm -f $(PROGRAM) *.o tags core shar
 
