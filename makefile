CFLAGS = -lsqlite3

build:
	@echo "compiling the main file..."
	gcc -o c-creds main.c $(CFLAGS)

clean:
	@echo "removing everything but the source files..."
	rm c-creds
