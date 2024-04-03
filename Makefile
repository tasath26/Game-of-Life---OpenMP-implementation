CC = gcc
CFLAGS = -fopenmp -g -O3
CFLAGS_NO_MP = -g -O3
SRC =   life.c life.h
SRC_NO_MP = life_seq.c
TARGET =  life
OBJS = $(SRC:.c=.o)

all: $(TARGET)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

no_openmp: $(OBJS)
	$(CC) $(CFLAGS_NO_MP) life_seq.c -o life_seq

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

