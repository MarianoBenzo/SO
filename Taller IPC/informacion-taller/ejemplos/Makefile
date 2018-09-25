.PHONY: clean all

BIN = \
	pipe                     \
	udp-client               \
	udp-server

SRC = $(BIN:=.c)

.c:
	$(CC) -D_POSIX_C_SOURCE=200809L $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

all: $(BIN)

clean:
	rm -f $(BIN)

socket: udp-client udp-server
