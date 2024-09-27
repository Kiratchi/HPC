CC = gcc
CXX = g++
BINS = aliasing_example aliasing branching branchless data_blocking
CFLAGS = -O2
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

aliasing_example : aliasing_example.c
	$(CC) -o $@ $<

aliasing.o : aliasing.cc
	$(CXX) -c -o $@ $< $(CFLAGS) $(CELEROFLAGS)

aliasing_auxiliary.o : aliasing_auxiliary.c
	$(CC) -c -o $@ $< $(CFLAGS)

aliasing : aliasing.o aliasing_auxiliary.o
	$(CXX) -o $@ $^ $(CFLAGS) $(CELEROFLAGS)

branching : branching.cc
	$(CXX) -o $@ $< $(CFLAGS) $(CELEROFLAGS)

branchless : branchless.cc
	$(CXX) -o $@ $< $(CFLAGS) $(CELEROFLAGS)

data_blocking : data_blocking.cc
	$(CXX) -o $@ $< $(CFLAGS) $(CELEROFLAGS)

.PHONY : clean
clean :
	rm -rf Celero CeleroBuild aliasing.o aliasing_auxiliary.o $(BINS)
