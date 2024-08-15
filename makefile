LFLAGS = -lsqlite3

default: build

build:
	@echo "building db object file"
	gcc -c db/db.c
	@echo "building main object file"
	gcc -c main.c
	@echo "building c-creds executable file"
	gcc -o c-creds main.o db.o $(LFLAGS)
	@echo "\nc-creds built successfully!!"

clean:
	@echo "removing everything but the source files..."
	rm c-creds *.o
