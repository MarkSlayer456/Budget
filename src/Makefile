IDIR = ../include
CC=gcc
CFLAGS=-I$(IDIR) -Wall

ODIR=obj
LDIR=../lib

_DEPS = budget.h
DEPS = $(patsubst %,$(ODIR)/%,$(_OBJ))

_OBJ = main.o budget.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

budget: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
