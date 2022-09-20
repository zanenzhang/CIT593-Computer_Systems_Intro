#########################################################
#  file name   : Makefile                               #
#  author      : Zan Zhang                              #
#  description : Makefile to automate compiling         #
#                and linking                            #
#########################################################
#
#

all: assembler 

asm_parser.o: asm_parser.c asm_parser.h
	clang -c asm_parser.c

assembler.o: assembler.c
	clang -c assembler.c

assembler: assembler.o asm_parser.o
	clang assembler.o asm_parser.c -o assembler

clean:
	rm -rf *.o

clobber: clean
	rm -rf assembler
	


