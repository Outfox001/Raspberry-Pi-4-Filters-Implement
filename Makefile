CC = gcc
CFLAGS = -O3 -march=armv8-a
LDFLAGS = -lm
OBJS = main.o lms_filter.o lms_neon_opt.o lms_neon_opt2.o
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

main.o: main.c lms_filter.h lms_neon_opt.h lms_neon_opt2.h
	$(CC) $(CFLAGS) -c main.c

lms_filter.o: lms_filter.c lms_filter.h
	$(CC) $(CFLAGS) -c lms_filter.c

lms_neon_opt.o: lms_neon_opt.c lms_neon_opt.h
	$(CC) $(CFLAGS) -c lms_neon_opt.c

lms_neon_opt2.o: lms_neon_opt2.c lms_neon_opt2.h
	$(CC) $(CFLAGS) -c lms_neon_opt2.c

clean:
	rm -f *.o $(TARGET)
