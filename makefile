#!/usr/bin/env make -f

PROJECT := patates
BINDIR  := ./bin/
LIBDIR  := ./lib/
OBJDIR  := ./obj/
INCDIR  := ./inc/
SRCDIR  := ./src/
TESTDIR := ./test/
TARGET  := $(PROJECT).exe
MODULES := file map args srv http list log type signal

RM      := rm
MKDIR   := mkdir

PORT    := 8000
ARGS    := port=$(PORT) root=/tmp

DBG     := lldb
DBGARGS := --batch -o run -K .$(DBG)
AR      := ar
ARFLAGS := -rcs
CC      := cc
CFLAGS  := -O0 -g -Wall -fPIC $(addprefix -I, $(INCDIR)) -DNDEBUG
LD      := ld
LDFLAGS := $(addprefix -L, $(LIBDIR))
LIBS    := $(addprefix -l, m c dl pthread)

TESTBIN := $(addprefix $(BINDIR), $(addsuffix .test, $(MODULES)))
SYSTEM  := $(shell uname)

ifeq ($(SYSTEM),Darwin)
ARCH = -arch x86_64 -platform_version macos 10.9 $(shell sw_vers -productVersion)
endif
#export ARCH

ifeq ($(SYSTEM),Linux)
LD      := gcc
DBG     := gdb
DBGARGS := --batch -ex run -x .$(DBG)
endif
#export LD

default: $(BINDIR)$(TARGET)
	-tree $(BINDIR)

all: default $(TESTBIN)
	-tree

help:
	-grep -oE '^(\w|\.)+:[^#]+' makefile | column -t

edit:
	find . \( -iname '*.h' -or -iname '*.c' \) -exec subl {} +

docker:
	docker build --compress -t $(PROJECT) -f dockerfile $$(pwd)
	docker run --name=$(PROJECT) --rm -i $(PROJECT) \
		sh -c 'for i in *.test; do ./$$i || break; done'

run: $(BINDIR)$(TARGET)
	./$< $(ARGS)

fn: $(BINDIR)$(TARGET)
	sleep 1 && curl -vskL 0:$(PORT) &
	./$< $(ARGS)

dbg: $(BINDIR)$(TARGET)
	$(DBG) $(DBGARGS) -- ./$<

tests.bin: $(TESTBIN)
	-tree $(TESTDIR)

tests.dbg: $(TESTBIN)
	for test in $^; do $(DBG) $(DBGARGS) -- ./$${test}; done

tests.run: $(TESTBIN)
	for test in $^; do ./$${test}; done

tests: $(TESTBIN) # tests.dbg # default

watch:
	watchexec -n -c -e 'h,c,makefile' $(MAKE) clean all tests.run fn

clean:
	$(RM) -rf DerivedData \
	$(addprefix $(OBJDIR), $(addsuffix .o, main)) \
	$(addprefix $(LIBDIR), $(addsuffix .a, main)) \
	$(addprefix $(LIBDIR), $(addsuffix .a, $(MODULES))) \
	$(addprefix $(OBJDIR), $(addsuffix .o, $(MODULES))) \
	$(addprefix $(BINDIR), $(addsuffix .test, $(MODULES))) \
	$(addprefix $(OBJDIR), $(addsuffix _test.o, $(MODULES))) \
	$(addprefix $(BINDIR), $(TARGET)) $(BINDIR)*.dSYM

mkfile: $(SRCDIR) $(INCDIR) $(TESTDIR)
	test -n "$(NAME)" # NAME= is required!
	touch $(addprefix $(INCDIR),  $(addsuffix .h, $(NAME)))
	touch $(addprefix $(SRCDIR),  $(addsuffix .c, $(NAME)))
	touch $(addprefix $(TESTDIR), $(addsuffix .c, $(NAME)))

$(BINDIR) $(LIBDIR) $(OBJDIR) $(INCDIR) $(SRCDIR) $(TESTDIR):
	$(MKDIR) -p $@

$(OBJDIR)%.o: $(OBJDIR)
$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) -o $@ -c $^

$(LIBDIR)%.a: $(LIBDIR)
$(LIBDIR)%.a: $(OBJDIR)%.o
	$(AR) $(ARFLAGS) $@ $^

$(BINDIR)%: $(BINDIR)
$(BINDIR)$(TARGET): \
	$(addprefix $(LIBDIR), $(addsuffix .a, main)) \
	$(addprefix $(LIBDIR), $(addsuffix .a, $(MODULES)))
	$(LD) $(LDFLAGS) $(LIBS) $(ARCH) -o $@ $^

$(OBJDIR)%_test.o: $(TESTDIR)%.c
	$(CC) $(CFLAGS) -o $@ -c $^

$(BINDIR)%.test: \
	$(addprefix $(OBJDIR), $(addsuffix _test.o, %)) \
	$(addprefix $(LIBDIR), $(addsuffix .a, %))
	$(LD)  $(LDFLAGS) $(LIBS) $(ARCH) -o $@ $^
	# $(DBG) $(DBGARGS) -- ./$@

$(BINDIR)srv.test:  $(addprefix $(LIBDIR), $(addsuffix .a, list))
$(BINDIR)http.test: $(addprefix $(LIBDIR), $(addsuffix .a, list))

#TESTPRE := test_
#$(OBJDIR)$(TESTPRE)%.o: $(TESTDIR)$(TESTPRE)%.c
#	$(CC) $(CFLAGS) -o $@ -c $^
#$(LIBDIR)$(TESTPRE)%.a: $(OBJDIR)$(TESTPRE)%.o
#	$(AR) $(ARFLAGS) $@ $^
