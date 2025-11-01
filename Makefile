compile:
	gcc main.c -o baulo
run:
	./baulo
test:
	./baulo hello.bul
compile-test:
	make compile
	make test