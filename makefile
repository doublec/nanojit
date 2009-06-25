CFLAGS=-g -include mozilla-config.h

all: libnanojit.a

libnanojit.a: Assembler.o Fragmento.o LIR.o Nativei386.o RegAlloc.o avmplus.o
	ar r libnanojit.a Assembler.o Fragmento.o LIR.o Nativei386.o RegAlloc.o avmplus.o

Assembler.o: Assembler.cpp
	g++ -c $(CFLAGS) -o Assembler.o Assembler.cpp

Fragmento.o: Fragmento.cpp
	g++ -c $(CFLAGS) -o Fragmento.o Fragmento.cpp

LIR.o: LIR.cpp
	g++ -c $(CFLAGS) -o LIR.o LIR.cpp

NativeARM.o: NativeARM.cpp
	g++ -c $(CFLAGS) -o NativeARM.o NativeARM.cpp

NativeSparc.o: NativeSparc.cpp
	g++ -c $(CFLAGS) -o NativeSparc.o NativeSparc.cpp

Nativei386.o: Nativei386.cpp
	g++ -c $(CFLAGS) -o Nativei386.o Nativei386.cpp

RegAlloc.o: RegAlloc.cpp
	g++ -c $(CFLAGS) -o RegAlloc.o RegAlloc.cpp

TraceTreeDrawer.o: TraceTreeDrawer.cpp
	g++ -c $(CFLAGS) -o TraceTreeDrawer.o TraceTreeDrawer.cpp

avmplus.o: avmplus.cpp
	g++ -c $(CFLAGS) -o avmplus.o avmplus.cpp


clean:
	rm *.o
	rm *.a
