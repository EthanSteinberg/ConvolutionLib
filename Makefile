ifeq (1, 1)
	CC = x86_64-w64-mingw32-gcc
	JNI_INCLUDE = /usr/lib/jvm/java-8-oracle/include/win32
	LIB_FOLDER = lib/win32
	LIB_PREFIX = 
	LIB_EXTENSION = .dll
	EXE_EXTENSION = .exe
else
	CC = gcc
	JNI_INCLUDE = /usr/lib/jvm/java-8-oracle/include/linux
	LIB_FOLDER = lib/linux
	LIB_EXTENSION = .so
	LIB_PREFIX = lib
	EXE_EXTENSION = 
endif


SRCS = src/convolution.c src/main.c src/JNIConvolution.c
DEPS = $(SRCS:.c=.d)
OBJECTS = $(SRCS:.c=.o)

CFLAGS = -std=c99 -Wall -fPIC -O2 -ffast-math -march=native
CPPFLAGS := -I /usr/lib/jvm/java-8-oracle/include -I ${JNI_INCLUDE} -I ./include

LFLAGS = -L ${LIB_FOLDER}

LIBS = -lfftw3-3 -lm

EXECUTABLE := program${EXE_EXTENSION}
LIBRARY := ${LIB_PREFIX}JNIConvolution${LIB_EXTENSION}

default: ${LIBRARY} ${EXECUTABLE} 

include $(DEPS)

%.d : %.c
	$(CC) ${CPPFLAGS} $(CCFLAGS) -MF"$@" -MM -MT"$@" -MT"$(<:.c=.o)" "$<"

${LIBRARY}: $(OBJECTS)
		$(CC) -shared ${OBJECTS} ${LFLAGS} ${LIBS} -o $@ 

${EXECUTABLE}: $(OBJECTS)
		$(CC) ${OBJECTS} ${LFLAGS} ${LIBS} -o $@

clean:
	-rm  $(OBJECTS)
	-rm  $(DEPS)
	-rm  $(LIBRARY)
	-rm  $(EXECUTABLE)
	
