BINS = naive_bench celero_bench_O1 celero_bench_O2 hyperfine_bench
CELEROFLAGS = -ICelero/include -LCeleroBuild -lcelero -Wl,-rpath -Wl,CeleroBuild

.PHONY : all
all : $(BINS)

naive_bench : naive_bench.c
	gcc -o $@ $< -O2 -march=native -lm

.PHONY : celero
celero : CeleroBuild/libcelero.so

Celero/CMakeLists.txt :
	rm -rf Celero
	git clone --depth=1  https://github.com/DigitalInBlue/Celero.git

CeleroBuild/libcelero.so : Celero/CMakeLists.txt
	cmake -S Celero -B CeleroBuild
	$(MAKE) -C CeleroBuild


celero_bench_O1 : celero_bench.cc
	g++ -o $@ $< -O1 -march=native $(CELEROFLAGS)

celero_bench_O2 : celero_bench.cc
	g++ -o $@ $< -O2 -march=native $(CELEROFLAGS)

hyperfine_bench : hyperfine_bench.c
	gcc -o $@ $< -O2 -lm

.PHONY : clean
clean :
	rm -rf Celero CeleroBuild $(BINS)
