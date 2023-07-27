parser: main.o trie.o
	gcc main.o trie.o -o parser

main.o: main.c lib/main.h lib/trie.h
	gcc -Ilib -c main.c

trie.o: trie.c lib/trie.h
	gcc -Ilib -c trie.c

clean:
	mv main.o ./bin
	mv trie.o ./bin

unclean:
	mv ./bin/main.o .
	mv ./bin/trie.o .