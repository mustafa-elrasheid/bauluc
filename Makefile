compile:
	g++ -g3 src/main.cpp src/frontend/lexer.cpp src/frontend/parser.cpp src/frontend/grammer.cpp src/bytecode.cpp src/vm.cpp src/program.cpp src/utils.cpp src/program.labels.cpp src/program.offsets.cpp -o bin/baulu
run:
	./bin/baulu examples/hello.bul
test:
	./bin/baulu examples/hello.bul -log
compile-test:
	make compile
	make test