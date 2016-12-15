CC = gcc
CFLAGS = -mavx -O3 -g -Wall -std=c99

TARGETS = \
	  matrixmul \
	  matrixvecadd \
		matrixmulLoopBlocking \

all:	$(TARGETS)

%:	%.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
