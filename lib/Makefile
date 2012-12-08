AR=${CROSS_COMPILE}ar
AFLAGS=rcs
CFLAGS=-c -g -Wall -I../include
SOURCES=${wildcard *.c}
OBJECTS=${SOURCES:%.c=%.o}
OUT=libqip.a

all: ${OUT}

${OUT}: ${OBJECTS}
	$(AR) $(AFLAGS) $(OUT) $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f ${OBJECTS} ${OUT}

