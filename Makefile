CC = gcc
CFLAGS = -Wall
LDFLAGS = -lpthread -lm

EXE_ALLOCATE = allocate
SRCS = allocate.c sjf.c roundrobin.c memory_allocation.c
OBJS = $(SRCS:.c=.o)

all: $(EXE_ALLOCATE)

$(EXE_ALLOCATE): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f *.o $(EXE_ALLOCATE)