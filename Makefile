#Compiler
CC = g++

ODIR = build
SDIR = src
INC = -Iinc

#Debug or Release
DEBUG = -g
RELEASE = -O2
EXEC_TYPE = ${RELEASE}

#Compiler options
CFLAGS = -Wall -std=c++11 -Wno-deprecated -I headers ${EXEC_TYPE}

_OBJSG = generator.o shapes.o

OBJSG = $(patsubst %,$(ODIR)/%,$(_OBJSG))


$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) -c $(INC) -o $@ $< $(CFLAGS)

generator: $(OBJSG)
	$(CC) $(OBJSG) $(CFLAGS) -o generator

clean:
	rm -f build/*.o
	rm -f generator