CC := g++
CFLAGS := -Wall -Wextra -Werror -pedantic -Iinclude

TARGET := project
TARGET_TEST := test
OBJECTS := obj/parser.o

$(TARGET): $(OBJECTS) obj/main.o
	$(CC) -o $@ $^

$(TARGET_TEST): $(OBJECTS) obj/test.o
	$(CC) -o $@ $^ -lgtest_main -lgtest -lpthread

obj/%.o: src/%.cpp | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj:
	mkdir -p obj

clean:
	rm -rf obj
	rm -f $(TARGET)
	rm -f $(TARGET_TEST)

.PHONY: clean
