DESTDIR?=/
PREFIX?=/usr/local
BINDIR=$(DESTDIR)/$(PREFIX)/bin

.PHONY: install
install: 
    install -Dm755 -p lincut -t $(BINDIR)

.PHONY: uninstall
uninstall:
    rm -f  $(BINDIR)/lincut
