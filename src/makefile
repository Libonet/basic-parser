parser: main.o trie.o cola.o
	gcc main.o trie.o cola.o -o parser

main.o: main.c lib/main.h lib/trie.h lib/cola.h
	gcc -Ilib -c main.c

cola.o: cola.c lib/cola.h lib/trie.h
	gcc -Ilib -c cola.c

trie.o: trie.c lib/trie.h
	gcc -Ilib -c trie.c

clean:
	mv main.o ./bin
	mv trie.o ./bin
	mv cola.o ./bin

unclean:
	mv ./bin/main.o .
	mv ./bin/trie.o .