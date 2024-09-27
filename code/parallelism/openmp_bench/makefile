CC = gcc
CXX = g++
OBJS = $(OBJS_FOR) $(OBJS_SCHEDULERS)
OBJS_FOR = for.o for_auxiliary.o
OBJS_SCHEDULERS = schedulers.o schedulers_auxiliary.o
BINS = for schedulers
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


for_auxiliary.o : for_auxiliary.c
	$(CC) -c -o $@ $< $(CFLAGS) $(CELEROFLAGS)

for.o : for.cc
	$(CXX) -c -o $@ $< $(CFLAGS) $(CELEROFLAGS)

for : $(OBJS_FOR)
	$(CXX) -o $@ $^ $(CFLAGS) $(CELEROFLAGS)

schedulers_auxiliary.o : schedulers_auxiliary.c
	$(CC) -c -o $@ $< $(CFLAGS) $(CELEROFLAGS)

schedulers.o : schedulers.cc
	$(CXX) -c -o $@ $< $(CFLAGS) $(CELEROFLAGS)

schedulers : $(OBJS_SCHEDULERS)
	$(CXX) -o $@ $^ $(CFLAGS) $(CELEROFLAGS)


.PHONY : clean
clean :
	rm -rf Celero CeleroBuild $(BINS) $(OBJS)
