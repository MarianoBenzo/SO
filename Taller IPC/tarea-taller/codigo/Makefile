.POSIX:
.SUFFIXES:
.SUFFIXES: .c

DEFINES = -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE

.c:
	$(CC) $(DEFINES) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

BIN = \
	mini-shell         \
	mini-telnet-client \
	mini-telnet-server

SRC = $(BIN:=.c)

all: $(BIN)

$(SRC): config.h

clean:
	rm -f $(BIN)
