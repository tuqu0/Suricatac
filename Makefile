CC=gcc
EXEC=Suricatac
FILES= ./src/main.c ./src/utils.c ./src/client.c
CFLAGS= -ggdb -O0 -Wall
LIBS=
OBJS=$(FILES:.c=.o)
DIST= puydoyeux_vincent-Suricatac

Suricatac: $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o ./bin/$(EXEC) $(OBJS) $(LIBS)
	cp -f ./bin/$(EXEC) ./tests/

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf ./src/*.o
	rm -f ./bin/$(EXEC)
	rm -f ./tests/$(EXEC)
	rm -rf ./tests/output/*.log
	rm -rf ./tests/output/unified2.alert.*
	
dist: clean
	mkdir ../$(DIST)
	cp -r ../$(EXEC)/*  ../$(DIST)
	tar cvfj ../$(DIST).tar.gz ../$(DIST) 
	rm -rf ../$(DIST)
