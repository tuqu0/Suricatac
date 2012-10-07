CC=gcc
EXEC=Suricatac
FILES= ./src/main.c ./src/utils.c ./src/client.c
CFLAGS= -ggdb -O0 -Wall
INCLUDES=
LIBS=
OBJS=$(FILES:.c=.o)
DIST= puydoyeux_vincent-Suricatac

Suricatac: $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o ./bin/$(EXEC) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf ./src/*.o
	rm -f ./bin/$(EXEC)

dist:
	tar cvfj ../$(DIST).tar.gz ../$(EXEC) 
