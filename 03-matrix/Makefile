CXX := g++-12
CFLAGS := -std=c++20 -Iinclude -Wextra -Werror -pedantic

TARGET := project

OBJDIR := obj
OBJECTS := $(OBJDIR)/matrix.o $(OBJDIR)/main.o

$(TARGET): $(OBJECTS)
	$(CXX) $(CFLAGS) -o $@ $^ -lgtest_main -lgtest -lpthread

$(OBJDIR)/%.o: src/%.cc | $(OBJDIR)
	$(CXX) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)
	rm -f $(TARGET)

.PHONY: clean
