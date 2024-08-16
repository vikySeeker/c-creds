LFLAGS = -lsqlite3

default: build

build: mystrings.o creds.o modes.o db.o main.o
	@echo "building c-creds executable file"
	gcc -o c-creds main.o db.o creds.o modes.o mystrings.o $(LFLAGS)
	@echo "\nc-creds built successfully!!"
main.o: main.c
	@echo "building main object file"
	gcc -c main.c
mystrings.o: mystrings/mystrings.c includes/mystrings.h
	@echo "building mystrings object file"
	gcc -c mystrings/mystrings.c

creds.o: creds/creds.c includes/creds.h
	@echo "building creds object file"
	gcc -c creds/creds.c

modes.o: modes/modes.c includes/modes.h
	@echo "building modes object file"
	gcc -c modes/modes.c

db.o: db/db.c includes/db.h
	@echo "building db object file"
	gcc -c db/db.c
clean:
	@echo "removing everything but the source files..."
	rm c-creds *.o
