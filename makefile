########################################
#
#  IUPUI CSCI 503 -- Operating Systems
#
########################################
## Compiler, tools and options
CC      = gcc
LINK    = gcc
OPT     = -g

CCFLAGS = $(OPT)
LDFLAGS = $(OPT)

## Libraries
LIBS = 
INC  = -I.

## FILES
OBJECTS = lab2.o
TARGET  = lab2


## Implicit rules
.SUFFIXES: .c
.c.o:
	$(CC) -c $(CCFLAGS) $(INC) $<

## Build rules
all: $(TARGET) 

$(TARGET): $(OBJECTS)
	$(LINK) -o $@ $(OBJECTS) $(LDFLAGS) $(LIBS)

clean:
	rm -f $(OBJECTS) $(TARGET) 
	rm -f *~ core
