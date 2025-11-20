compile:
	g++ -g3 src/main.cpp src/lexer.cpp src/parser.cpp -o bin/baulo
run:
	./bin/baulo
test:
	./bin/baulo examples/hello.bul
compile-test:
	make compile
	make test