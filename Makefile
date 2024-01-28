CFLAGS ?=
LDFLAGS ?=
CC ?= gcc

CFLAGS += $(shell pkg-config --cflags dbus-1)
LDFLAGS += $(shell pkg-config --libs dbus-1)

ifdef USE_CJSON
CFLAGS += -DUSE_CJSON $(shell pkg-config --cflags libcjson)
LDFLAGS += $(shell pkg-config --libs libcjson)
endif

ifdef DWLB_STRLEN
CFLAGS += -DDWLB_STRLEN=${DWLB_STRLEN}
endif

.PHONY:

all: noticat

noticat: cnoti.c noticat.c
	$(CC) cnoti.c noticat.c $(CFLAGS) $(LDFLAGS) -o $@

clean:
	rm -f noticat
