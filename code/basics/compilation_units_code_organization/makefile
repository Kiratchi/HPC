CC = gcc
CFLAGS = -O2 -march=native

.PHONY : all
all : \
  included_constants_definition \
  included_constants_static \
  included_declaration \
  included_inline


included_constants_definition : \
  included_constants_definition.c \
  included_constants_definition.h
	$(CC) $(CFLAGS) -o $@ $<


included_constants_static.o : \
  included_constants_static.c \
  included_constants_static.h

included_constants_static_print.o : \
  included_constants_static_print.c \
  included_constants_static.h

included_constants_static : \
  included_constants_static.o \
  included_constants_static_print.o
	$(CC) $(CFLAGS) -o $@ $^


included_inline : included_inline.c included_inline.h
	$(CC) $(CFLAGS) -o $@ $<


included_declaration.o : \
	included_declaration.c included_declaration.h

included_declaration_definition.o : \
	included_declaration_definition.c included_declaration.h

included_declaration.h : included_declaration_auxiliary.h

OBJS_DECLARATION = \
  included_declaration.o \
  included_declaration_definition.o

included_declaration : $(OBJS_DECLARATION)
	$(CC) $(CFLAGS) -o $@ $^


.PHONY : clean
clean :
	rm -rf \
	  included_constants_definition \
	  included_constants_static \
	  included_constants_static.o \
  	  included_constants_static_print.o \
	  included_inline \
	  included_declaration \
	  $(OBJS_DECLARATION)
