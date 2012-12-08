CC=${CROSS_COMPILE}gcc
CFLAGS=-c -g -Wall -I../include
LDFLAGS=-L../lib -lqip -lpthread
SOURCES=${wildcard *.c}
OBJECTS=${SOURCES:%.c=%.o}
OUT=test

all: ${OUT}

${OUT}: ${OBJECTS}
	$(CC) -o $(OUT) $(OBJECTS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f ${OBJECTS} ${OUT}

