CC = gcc
CFLAGS = -Wall -Wextra
TARGET = main
SRCS = main.c utils.c inventory.c combat.c world.c save.c

# По умолчанию: компилирует проект
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Собрать и сразу запустить игру
run: $(TARGET)
	./$(TARGET)

# Очистить скомпилированный бинарник
clean:
	rm -f $(TARGET)