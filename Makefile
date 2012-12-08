CC=${CROSS_COMPILE}gcc

DIRS=lib qip-manager tests
DIRSCLEAN=${DIRS:%=%-clean}

all: ${DIRS}

${DIRS}:
	${MAKE} -C $@

${DIRSCLEAN}:
	${MAKE} -C ${@:%-clean=%} clean

clean: ${DIRSCLEAN}

.PHONY: subdirs ${DIRS}
