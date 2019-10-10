# object files
OBJ=fir.o svf.o svf-vst.o audioeffect.o audioeffectx.o vstplugmain.o

# compilers
CC=g++

# compiler options
OPTS=-Wall
CFLAGS=-fPIC -I../vstsdk2.4 -D__cdecl=""

svf.so: $(OBJ)
	$(CC) -shared -Wl,-soname,svf.so -o $@ $+

fir.o:	fir.cpp
	$(CC) $(OPTS) $(CFLAGS) -c $<

svf.o:	svf.cpp
	$(CC) $(OPTS) $(CFLAGS) -c $<

svf-vst.o:	svf-vst.cpp
	$(CC) $(OPTS) $(CFLAGS) -c $<

audioeffect.o:	audioeffect.cpp
	$(CC) $(OPTS) $(CFLAGS) -c $<

audioeffectx.o:	audioeffectx.cpp
	$(CC) $(OPTS) $(CFLAGS) -c $<

vstplugmain.o:	vstplugmain.cpp
	$(CC) $(OPTS) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm *.o *.so

