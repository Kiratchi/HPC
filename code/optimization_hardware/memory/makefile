CC = gcc
CXX = g++
BINS = locality locality_block padding indirect_addressing aliasing_example aliasing memory_vs_file allocation_bad array_access array_access_native
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

locality : locality.cc
	$(CXX) -o $@ $< $(CFLAGS) $(CELEROFLAGS)

locality_block_auxiliary.o : locality_block_auxiliary.c
	$(CC) -c -o $@ $< $(CFLAGS) $(CELEROFLAGS)

locality_block.o : locality_block.cc
	$(CXX) -c -o $@ $< $(CFLAGS) $(CELEROFLAGS)

locality_block : locality_block.o locality_block_auxiliary.o
	$(CXX) -o $@ $^ $(CFLAGS) $(CELEROFLAGS)

indirect_addressing : indirect_addressing.cc
	$(CXX) -o $@ $< $(CFLAGS) $(CELEROFLAGS)

aliasing_example : aliasing_example.c
	$(CC) -o $@ $<

aliasing.o : aliasing.cc
	$(CXX) -c -o $@ $< $(CFLAGS) $(CELEROFLAGS)

aliasing_auxiliary.o : aliasing_auxiliary.c
	$(CC) -c -o $@ $< $(CFLAGS)

aliasing : aliasing.o aliasing_auxiliary.o
	$(CXX) -o $@ $^ $(CFLAGS) $(CELEROFLAGS)

file_access : file_access.cc
	$(CXX) -o $@ $^ $(CFLAGS) $(CELEROFLAGS)

padding : padding.c
	$(CC) -o $@ $<

allocation_bad.o : allocation_bad.c
	$(CC) -c -o $@ $< $(CFLAGS)

allocation_bad_auxiliary.o : allocation_bad_auxiliary.c
	$(CC) -c -o $@ $< $(CFLAGS)

allocation_bad : allocation_bad.o allocation_bad_auxiliary.o
	$(CC) -o $@ $^ $(CFLAGS)

array_access : array_access.cc
	$(CXX) -o $@ $< $(CFLAGS) $(CELEROFLAGS)

array_access_native : array_access.cc
	$(CXX) -march=native -o $@ $< $(CFLAGS) $(CELEROFLAGS)


.PHONY : clean
clean :
	rm -rf Celero CeleroBuild bench.dat locality_block.o locality_block_auxiliary.o aliasing.o aliasing_auxiliary.o allocation_bad.o allocation_bad_auxiliary.o $(BINS)
