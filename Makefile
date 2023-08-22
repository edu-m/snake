# Specifica il compilatore da utilizzare
CC = g++

# Opzioni per il compilatore
CFLAGS = -O2 -std=c++17 -Wall

# Librerie SFML da linkare
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Nome dell'eseguibile
EXECUTABLE = snake.out

# Nome del file sorgente
SOURCE = main.cpp

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCE) $(SFML_LIBS)

clean:
	rm -f $(EXECUTABLE)
