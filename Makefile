CC=clang
CFLAGS= -g -Wall -lX11
SOURCES=$(TARGET).c

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)
clean:
	rm $(TARGET)
