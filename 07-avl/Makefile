CXX := g++
CFLAGS := -std=c++20 -Iinclude -Wall -Werror -Wextra -pedantic

TARGET := test
OBJDIR := obj

OBJECTS := $(OBJDIR)/test.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ -lgtest_main -lgtest -lpthread

$(OBJDIR)/%.o: src/%.cpp | $(OBJDIR)
	$(CXX) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)
	rm -f $(TARGET)

.PHONY: clean all