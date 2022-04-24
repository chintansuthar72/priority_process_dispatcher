# you can put include files in this directory
IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

# directory where object files will be created
ODIR=obj
# you can put library files in this directory
LDIR =../lib
# looks for library libm.so or libm.a
LIBS=-pthread

# create string with ../include/<header filename> in our case ../include/myheader.h
_DEPS = Process_control_block.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

# create string with obj/<object filename> in our case obj/add.o and obj/sub.o  (obj folder will be created under current i.e. source directory)
_LIBOBJ = Process_Queue.o
LIBOBJ = $(patsubst %,$(ODIR)/%,$(_LIBOBJ))
LIBSRC = Process_Queue.c
_APPOBJ = Feed_back_dispatcher.o
APPOBJ = $(patsubst %,$(ODIR)/%,$(_APPOBJ))

$(ODIR)/Process_Queue.o: Process_Queue.c
	$(CC) -c -fPIC -o $@ $< $(CFLAGS)


$(LDIR)/libmypcb.so:$(LIBOBJ)
	$(CC) -shared -o $@ $^

$(APPOBJ): Feed_back_dispatcher.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
Feed_back_dispatcher.out: $(APPOBJ) $(LDIR)/libmypcb.so
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(LDIR)/libmypcb.so
	
.PHONY: clean

# removes object files and temporary files created with ~ at the end of file name in current i.e. source and include directory
clean: 
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
	rm -f Feed_back_dispatcher.out $(LDIR)/libmypcb.so
	
.PHONY: all
all: $(LDIR)/libmypcb.so Feed_back_dispatcher.out

