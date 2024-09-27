CC = gcc
CXX = g++
BINS = vector_add vector_bench
CFLAGS = -O2 -march=native
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


vector_add : vector_add.c
	$(CC) -o $@ $< -march=native -g

vector_bench : vector_bench.cc
	$(CXX) -o $@ $< $(CFLAGS) $(CELEROFLAGS)

.PHONY : clean
clean :
	rm -rf Celero CeleroBuild $(BINS)
