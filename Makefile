# See LICENSE file for copyright and license details
.POSIX:

include config.mk

all: shrt

shrt: ${OBJ}
	${CC} ${LDFLAGS} -o shrt ${OBJ}

shrt.o shrtfile.o: shrtfile.h

shrt.o: config.h

.c.o:
	${CC} ${CFLAGS} ${CPPFLAGS} -o $@ $<

clean:
	rm -f shrt *.o

install: shrt
	install -Dm755 shrt ${DESTDIR}${PREFIX}/bin/shrt
	install -Dm644 shrt.1 ${DESTDIR}${MANPATH}/man1/shrt.1
	install -Dm644 shrt.1 ${DESTDIR}${MANPATH}/man5/shrtfile.5

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/shrt
	rm -f ${DESTDIR}${MANPATH}/man1/shrt.1
	rm -f ${DESTDIR}${MANPATH}/man5/shrtfile.5

dist:
	rm -rf shrt-${VERSION}
	mkdir shrt-${VERSION}
	cp -r ${DIST_SRC} shrt-${VERSION}
	tar -cf - shrt-${VERSION} | gzip > shrt-${VERSION}.tar.gz

test: shrt
	sh test/do_test.sh

distclean:
	rm -rf shrt-*

.PHONY: all clean install uninstall dist distclean test
