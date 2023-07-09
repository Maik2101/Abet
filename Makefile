SOURCES = $(wildcard *.c)
OUTPUT = main

all:
	gcc -Wall $(SOURCES) -o $(OUTPUT)

clean:
	rm -f $(OUTPUT) $(SOURCES:.c=.o)

debug:
	gcc -g $(SOURCES) -o $(OUTPUT)
