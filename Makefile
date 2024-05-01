CFLAGS :=
LDFLAGS :=
DEFS += -DDEBUG
DEFS += -DMEMORY_EFFICIENT
DEFS += -DDOUBLE_HASHING
DEFS += -DINTS
DEFINES = $(DEFS)

CC = gcc
SRCDIR=src
OUTDIR=out
LDDIR=$(SRCDIR)/lib

SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(patsubst $(SRCDIR)/%.c, $(OUTDIR)/%.o, $(SRCS))
LDLIBS=$(patsubst $(LDDIR)%.c, $(OUTDIR)/%.so, $(SRCS))

INCLUDE=-Iinclude

$(OUTDIR)/%.o : $(SRCDIR)/%.c Makefile
	$(CC) -c $< $(INCLUDE) $(CFLAGS) -o $@ $(DEFINES) $(LDFLAGS)

PROJ_NAME=hash_table

all:
	make configure objects
	$(CC) $(OBJS) $(CFLAGS) -o  $(PROJ_NAME) $(LDFLAGS)

configure:
	mkdir -p out
	mkdir -p shared

objects: $(OBJS)

clean:
	rm -rf out/* && rm -f $(PROJ_NAME)
	rmdir out shared


