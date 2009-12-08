DESTDIR=
PREFIX=/usr/local
DOCDIR=$(PREFIX)/share/doc/rarcrack
DOCUMENTATION=ChangeLog README COPYING
CFLAGS+=-Wall `pkg-config gthread-2.0 gobject-2.0 glib-2.0 --cflags`
LDFLAGS+=`pkg-config gthread-2.0 gobject-2.0 glib-2.0 --libs`
SOURCES=src/main.c src/rarcrack.c src/state.c \
        src/rarcrack.h src/state.h src/file.h \
		src/file.c

all:
	@echo -n "Compiling sources ... "
	@$(CC) $(SOURCES) $(CFLAGS) $(LDFLAGS) -o rarcrack
	@echo "Done"

clean:
	@echo -n "Cleaning up ... "
	-@rm -f *.o rarcrack tags debian/files debian/*.log
	-@rm -rf debian/tmp debian/rarcrack *stamp*
	@echo "Done"

install:
	@echo -n "Installing rarcrack ... "
	-@mkdir -p $(DESTDIR)$(PREFIX)/bin
	@install -s rarcrack $(DESTDIR)$(PREFIX)/bin
	-@mkdir -p $(DESTDIR)$(DOCDIR)
	@chmod 755 $(DESTDIR)$(DOCDIR)
	@install -m 644 -t $(DESTDIR)$(DOCDIR) $(DOCUMENTATION) 
	@install -m 644 -t $(DESTDIR)$(DOCDIR) test.zip test.7z test.rar
	@echo "Done"

uninstall:
	@echo -n "Uninstalling rarcrack from $(DESTDIR) ... "
	-@rm $(DESTDIR)$(PREFIX)/bin/rarcrack
	-@rm -rf $(DESTDIR)$(DOCDIR)
	@echo "Done"

dist:
	@echo -n "Creating distribution package: rarcrack-0.3.tar.bz2 ... "
	@tar -cjpf rarcrack-0.3.tar.bz2 $(SOURCES) gentoo/rarcrack-0.3.ebuild \
	          debian/control debian/rules debian/changelog debian/compat \
	          debian/copyright debian/install $(DOCUMENTATION) \
	          Makefile Makefile.win test.zip test.7z test.rar
	@echo "Done"

distclean: clean
	-@rm -f rarcrack-0.3.tar.bz2

	
