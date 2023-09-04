CC=g++ --std=c++17 -Wall
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=src/main.cpp src/morse.cpp
OBJECTS=$(SOURCES:.cpp=.o)
BUILDDIR=build/
TARGET=$(BUILDDIR)/morse-cpp

all: $(SOURCES) $(TARGET)
	
$(TARGET): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf *.o $(TARGET)

rebuild: clean all