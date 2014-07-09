ifeq (1, 1)
	CC = x86_64-w64-mingw32-gcc
	JNI_INCLUDE = /usr/lib/jvm/java-8-oracle/include/win32
	LIB_FOLDER = lib/win32
	LIB_EXTENSION = .dll
	EXE_EXTENSION = .exe
else
	CC = gcc
	JNI_INCLUDE = /usr/lib/jvm/java-8-oracle/include/linux
	LIB_FOLDER = lib/linux
	LIB_EXTENSION = .so
	EXE_EXTENSION = 
endif



HEADERS = convolution.h convolutionlib_JNIConvolution.h
OBJECTS = src/convolution.o src/main.o src/JNIConvolution.o

CFLAGS = -std=c99 -Wall -fPIC -O2 -ffast-math -march=native
CPPFLAGS := -I /usr/lib/jvm/java-8-oracle/include -I ${JNI_INCLUDE} -I ./include

LFLAGS = -L ${LIB_FOLDER}



LIBS = -lfftw3-3 -lm

EXECUTABLE := program${EXE_EXTENSION}
LIBRARY := libJNIConvolution${LIB_EXTENSION}

default: ${LIBRARY} ${EXECUTABLE} 


${LIBRARY}: $(OBJECTS)
	    $(CC) -shared ${OBJECTS} ${LFLAGS} ${LIBS} -o $@ 

${EXECUTABLE}: $(OBJECTS)
		$(CC) ${OBJECTS} ${LFLAGS} ${LIBS} -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f $(LIBRARY)
	-rm -r $(EXECUTABLE)
