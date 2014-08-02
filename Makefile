CC=gcc
CFLAGS= -g -Wall -L/opt/X11/lib -I/opt/X11/include -lX11
SOURCES=$(TARGET).c

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)
clean:
	rm $(TARGET)
