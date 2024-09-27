CXX = g++
BINS = algebraic_expressions \
       performed_operation \
       operand_type operand_type_sqrtf operand_type_sqrtff operand_type_sqrt \
       operand_value
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

algebraic_expressions : algebraic_expressions.cc
	$(CXX) -o $@ $< $(CFLAGS) $(CELEROFLAGS)

performed_operation : performed_operation.cc
	$(CXX) -o $@ $< $(CFLAGS) $(CELEROFLAGS)

operand_type : operand_type.cc
	$(CXX) -o $@ $< $(CFLAGS) $(CELEROFLAGS)

operand_type_sqrtf : operand_type_sqrtf.cc
	$(CXX) -g -o $@ $< $(CFLAGS) $(CELEROFLAGS)

operand_type_sqrtff : operand_type_sqrtff.cc
	$(CXX) -g -o $@ $< $(CFLAGS) $(CELEROFLAGS)

operand_type_sqrt : operand_type_sqrt.cc
	$(CXX) -g -o $@ $< $(CFLAGS) $(CELEROFLAGS)

operand_value : operand_value.cc
	$(CXX) -o $@ $< $(CFLAGS) $(CELEROFLAGS)

.PHONY : clean
clean :
	rm -rf Celero CeleroBuild $(BINS)
