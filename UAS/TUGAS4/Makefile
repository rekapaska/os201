# Thu Jul  2 12:49:22 WIB 2020
# Fri Jun 12 12:04:27 WIB 2020
# Thu Jun 11 17:35:49 WIB 2020

CC      = gcc
SOURCES = $(wildcard p*.c)
EXECS   = $(SOURCES:%.c=%)
SHAREMEM= SharedMemoryFile.bin
CFLAGS  = -std=gnu11 -pthread

all: $(EXECS)

$(EXECS): share.c share.h

clean: ; rm -f $(EXECS) $(SHAREMEM)

.PHONY: all clean