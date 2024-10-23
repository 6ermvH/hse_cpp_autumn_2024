CC := g++
CFLAGS := -Wall -Wextra -Werror -pedantic -Iinclude

TARGET := project
TARGET_TEST := test
LIB := lib/libparser.a

$(TARGET): obj/main.o $(LIB)
	$(CC) -o $@ $< -Llib -lparser

$(TARGET_TEST): obj/test.o $(LIB)
	$(CC) -o $@ $< -Llib -lgtest_main -lgtest -lpthread -lparser

$(LIB): obj/parser.o | lib
	ar rc $@ $^

obj/%.o: src/%.cpp | obj
	$(CC) $(CFLAGS) -c $< -o $@

lib:
	mkdir -p lib

obj:
	mkdir -p obj

clean:
	rm -rf obj
	rm -rf lib
	rm -f $(TARGET)
	rm -f $(TARGET_TEST)

.PHONY: clean
