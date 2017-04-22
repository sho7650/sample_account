#
#
#
COMPILER = cc
CFLAGS   = -MMD -MP -Wall -Wextra -Winit-self -Wno-missing-field-initializers
TARGET   = ./sample_account
SOURCE   = ./sample_account.c

$(TARGET): $(SOURCE)
	$(COMPILER) -o $@ $^

all: clean $(TARGET)

clean:
	-rm -f $(TARGET)
