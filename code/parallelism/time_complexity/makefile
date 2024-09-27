CC = gcc
CXX = g++
OBJS = speedup.o speedup_auxiliary.o
BINS = speedup
CFLAGS = -O2 -fopenmp
CELEROFLAGS = -ICelero/include -LCeleroBuild -lcelero -Wl,-rpath -Wl,CeleroBuild

.PHONY : all
all : $(BINS)

.PHONY : celero
celero : CeleroBuild/libcelero.so

Celero/CMakeLists.txt :
	rm -rf Celero
	git clone --depth=1  https://github.com/DigitalInBlue/Celero.git

CeleroBuild/libcelero.so : Celero/CMakeLists.txt
	cmake -S Celero -B CeleroBuild
	$(MAKE) -C CeleroBuild


speedup_auxiliary.o : speedup_auxiliary.c
	$(CC) -c -o $@ $< $(CFLAGS) $(CELEROFLAGS)

speedup.o : speedup.cc
	$(CXX) -c -o $@ $< $(CFLAGS) $(CELEROFLAGS)

speedup : $(OBJS)
	$(CXX) -o $@ $^ $(CFLAGS) $(CELEROFLAGS)

.PHONY : clean
clean :
	rm -rf Celero CeleroBuild $(BINS) $(OBJS)
