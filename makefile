VERSION = 1.6

# -o ./nkorp



MAIN = main.cpp
#use long double or "just" double?
#MAIN = main-nolong.cpp

DFLAGS = -g
WFLAGS = -Wall

CFLAGS = -O2 -fopenmp -march=native -mmmx -mavx -mfpmath=sse -malign-double -mpc80 # 
CFLAGS += -msse -msse2 -msse3 -march=native -mtune=native -mcpu=native
CFLAGS += $(WFLAGS)
CFLAGS += $(DFLAGS) 

CC = g++

nkorp: nkorp.o
	@echo
	@echo "nkorp"
	$(CC) $(CFLAGS) -o nkorp nkorp.o 
	@echo "nkorp gebaut"
	
nkorp.o: $(MAIN) Vektor.h mapu.h treeclear3d.h
	@echo	
	@echo "nkorp.o"
	$(CC) -c $(CFLAGS) -o nkorp.o $(MAIN)
	@echo "nkorp.o gebaut"	
	

#mapu.h.gch: mapu.h  

#treeclear3d.h.gch: treeclear3d.h

#Vektor.h.gch: Vektor.h 
#	$(CXX) -c 

clean:
	@echo
	@echo "clean!"
	-rm *.o
	@echo "bereit für neue builds"

removedata:
	@echo
	@echo "rmdata"
	-rm  version/* data/*   data-per/*  plotme.dat 
	
	@#mkdir version data plotme
	
	@echo "bereit für neue daten"
all: clean nkorp clean

remake: all removedata

