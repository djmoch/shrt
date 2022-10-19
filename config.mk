# See LICENSE file for copyright and license details
PREFIX := /usr/local
MANPATH := ${PREFIX}/man

SRC = \
	main.c \
	shrtfile.c
OBJ = ${SRC:c=o}
DIST_SRC = ${SRC} Makefile config.mk shrt.1 shrtfile.5 README LICENSE
VERSION = 0.2.3-dev0

CFLAGS = -c -g -O0 -std=c99
CPPFLAGS = -DVERSION=\"${VERSION}\" -D_BSD_SOURCE
LDFLAGS = -static
CC = cc
