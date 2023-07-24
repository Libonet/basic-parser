

parser: main.o trie.o tablahash.o
	gcc main.c trie.o -o parser

main.o: main.c lib/trie.h
	gcc -c -Ilib main.c

trie.o: trie.c lib/trie.h lib/tablahash.h
	gcc -c -Ilib trie.c

tablahash.o: tablahash.c lib/tablahash.h
	gcc -c -Ilib tablahash.c

clean:
	mv main.o ./bin
	mv trie.o ./bin