#
#
#
COMPILER = c++
# Override on the command line: `make CXXFLAGS_OPT=-O2` for an optimized build.
CXXFLAGS_OPT ?= -O0
CXXFLAGS = -g -std=c++17 $(CXXFLAGS_OPT) -MMD -MP -Wall -Wextra -Winit-self -Wno-missing-field-initializers
LIBS     =
LDFLAGS  =
INCLUDE  = -I./include
TARGET   = ./sample_account
SRCDIR   = ./source
SOURCES  = $(wildcard $(SRCDIR)/*.cpp)
OBJDIR   = ./obj
OBJECTS  = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cpp=.o)))
DEPENDS  = $(OBJECTS:.o=.d)

# Sources excluding the executable's main(); used to link unit-test binaries
# against the production code without dragging in main.
LIB_SOURCES = $(filter-out $(SRCDIR)/sample_account.cpp $(SRCDIR)/main.cpp, $(SOURCES))
LIB_OBJECTS = $(addprefix $(OBJDIR)/, $(notdir $(LIB_SOURCES:.cpp=.o)))

TESTDIR     = ./tests
TEST_BINS   = $(OBJDIR)/test_repos

$(TARGET): $(OBJECTS) $(LIBS)
	$(COMPILER) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	-mkdir -p $(OBJDIR)
	$(COMPILER) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(OBJDIR)/test_repos: $(TESTDIR)/test_repos.cpp $(LIB_OBJECTS)
	-mkdir -p $(OBJDIR)
	$(COMPILER) $(CXXFLAGS) $(INCLUDE) -o $@ $^ $(LDFLAGS)

all: clean $(TARGET)

clean:
	-rm -f $(OBJECTS) $(DEPENDS) $(TARGET) $(TEST_BINS)

unit-test: $(TEST_BINS)
	@echo "--- unit tests ---"
	@$(OBJDIR)/test_repos

snapshot-test: $(TARGET)
	@echo "--- snapshot tests ---"
	@./tests/snapshot.sh

test: unit-test snapshot-test

.PHONY: all clean test unit-test snapshot-test

-include $(DEPENDS)
