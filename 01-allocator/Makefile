CC = g++
CFLAGS := -std=c++20 -Wall -Werror -Wextra -pedantic -Iinclude

TARGET := test
OBJECTS := obj/allocator.o obj/test.o

$(TARGET): $(OBJECTS)
	$(CC) -g -o $@ $^ -lgtest_main -lgtest -lpthread

obj/%.o: src/%.cpp | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj:
	mkdir -p obj

clean:
	rm -rf obj
	rm -f $(TARGET)

.PHONY: clean
