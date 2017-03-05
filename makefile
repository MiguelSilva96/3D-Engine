CC=g++ -std=c++11 -Wno-deprecated -I headers


ODIR = build
SDIR = src
INC = -Iinc

all : generator engine

_OBJSG = generator.o shapes.o
_OBJS += $(_OBJSG)

OBJSG = $(patsubst %,$(ODIR)/%,$(_OBJSG))
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) -c $(INC) -o $@ $< $(CFLAGS)

generator: $(OBJSG)
	$(CC) $(OBJSG) -o generator

clean:
	rm -f build/*
	rm -f generator
