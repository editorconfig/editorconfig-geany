# Copyright (c) 2011-2012 EditorConfig Team
# All rights reserved.
#
# This file is part of EditorConfig-geany.

# EditorConfig-geany is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 2 of the License, or (at your option) any
# later version.

# EditorConfig-geany is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
# details.

# You should have received a copy of the GNU General Public License along with
# EditorConfig-geany. If not, see <http://www.gnu.org/licenses/>.

# Makefile for editorconfig-geany

EDITORCONFIG_PREFIX = /usr/local

CC = cc
CFLAGS = -Wall -Wextra -O2 -g -fPIC `pkg-config --cflags geany` -I${EDITORCONFIG_PREFIX}/include
LDFLAGS = -shared `pkg-config --libs geany` -L ${EDITORCONFIG_PREFIX}/lib -leditorconfig
RM = rm -f
TARGET_LIB = editorconfig-geany.so

SRCS = editorconfig-geany.c
OBJS = $(SRCS:.c=.o)

.PHONY: all
all: ${TARGET_LIB}

$(TARGET_LIB): $(OBJS)
	$(CC) -o $@ $^ ${LDFLAGS}

$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

include $(SRCS:.c=.d)

.PHONY: clean
clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d)

