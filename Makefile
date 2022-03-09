TARGET		:= librio.so

CC			:= clang
LINK_CMD	:= clang

CFLAGS		:= -Wall -Wextra -Werror -pedantic -std=c89
LFLAGS		:= -fPIC -shared

SOURCES		:= rio.c
OBJECTS		:= rio.o

.PHONY: all clean re

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINK_CMD) -o $@ $(OBJECTS) $(LFLAGS)

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)

re: clean all
