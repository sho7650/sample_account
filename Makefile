#
#
#
COMPILER = c++
CFLAGS   = -g -MMD -MP -Wall -Wextra -Winit-self -Wno-missing-field-initializers
# CFLAGS   = -Wall
LIBS     =
LDFLAGS  = -L/usr/local/opt/openssl/lib -lcrypto
INCLUDE  = -I./include -I/usr/local/opt/openssl/include
TARGET   = ./sample_account
SRCDIR   = ./source
SOURCES  = $(wildcard $(SRCDIR)/*.cpp)
OBJDIR   = ./obj
OBJECTS  = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cpp=.o)))
DEPENDS  = $(OBJECTS:.o=.d)

$(TARGET): $(OBJECTS) $(LIBS)
	$(COMPILER) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	-mkdir -p $(OBJDIR)
	$(COMPILER) $(CFLAGS) $(INCLUDE) -o $@ -c $<

all: clean $(TARGET)

clean:
	-rm -f $(OBJECTS) $(DEPENDS) $(TARGET)

-include $(DEPENDS)
