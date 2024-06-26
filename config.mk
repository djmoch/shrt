# See LICENSE file for copyright and license details
VERSION = 1.0.5-dev0
PREFIX := /usr/local
MANPATH := ${PREFIX}/man

SRC = \
	shrt.c \
	shrtfile.c
OBJ = ${SRC:c=o}
HDRS = \
	shrtfile.h \
	config.h
DIST_SRC = \
	${SRC} \
	${HDRS} \
	test \
	Makefile \
	config.mk \
	shrt.1 \
	shrtfile.5 \
	README \
	LICENSE

CFLAGS = -c -g -O0 -std=c99 -Wall -Wextra -Werror -pedantic-errors
CPPFLAGS = -DVERSION=\"${VERSION}\" -D_BSD_SOURCE
LDFLAGS = -pie -static
CC = cc
