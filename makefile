#!/usr/bin/env make -f

BINDIR  := ./bin/
LIBDIR  := ./lib/
OBJDIR  := ./obj/
INCDIR  := ./inc/
SRCDIR  := ./src/
TESTDIR := ./test/
TARGET  := patates
SOURCE  := file args srv http list log type

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
LIBS    := -lm -lc -ldl -lpthread

TESTBIN := $(addprefix $(BINDIR), $(addsuffix .test, $(SOURCE)))
SYSTEM  := $(shell uname)

ifeq ($(SYSTEM),Darwin)
ARCH = -arch x86_64 -platform_version macos \
           10.9 $(shell sw_vers -productVersion)
endif
export ARCH

ifeq ($(SYSTEM),Linux)
LD      := gcc
DBG     := gdb
DBGARGS := --batch -ex run -x .$(DBG)
endif
export LD

default: $(BINDIR)$(TARGET)
	-tree $(BINDIR)

all: default $(TESTBIN)
	-tree

edit:
	find . \( -iname '*.h' -or -iname '*.c' \) -exec subl {} +

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

clean:
	$(RM) -rf DerivedData \
	$(addprefix $(OBJDIR), $(addsuffix .o, main)) \
	$(addprefix $(LIBDIR), $(addsuffix .a, main)) \
	$(addprefix $(LIBDIR), $(addsuffix .a, $(SOURCE))) \
	$(addprefix $(OBJDIR), $(addsuffix .o, $(SOURCE))) \
	$(addprefix $(BINDIR), $(addsuffix .test, $(SOURCE))) \
	$(addprefix $(OBJDIR), $(addsuffix _test.o, $(SOURCE))) \
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
	$(addprefix $(LIBDIR), $(addsuffix .a, $(SOURCE)))
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
