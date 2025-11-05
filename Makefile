compile:
	gcc -g3 src/main.c src/lexer.c src/parser.c -o bin/baulo
run:
	./bin/baulo
test:
	./bin/baulo examples/hello.bul
compile-test:
	make compile
	make test