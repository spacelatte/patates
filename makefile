#!/usr/bin/env make -f

BINDIR  := ./bin/
LIBDIR  := ./lib/
OBJDIR  := ./obj/
INCDIR  := ./inc/
SRCDIR  := ./src/
TESTDIR := ./test/
TARGET  := patates
SOURCE  := args http list log srv type

RM      := rm
ARGS    := port=8080 root=/tmp
ARCH    := -arch x86_64 -platform_version macos \
           10.9 $(shell sw_vers -productVersion)
MKDIR   := mkdir

DBG     := lldb
DBGARGS := --batch -o run -K .lldb
AR      := ar
ARFLAGS := -rcs
CC      := cc
CFLAGS  := -g -Wall -fPIC $(addprefix -I, $(INCDIR)) -DNDEBUG \
           #-Wno-incompatible-pointer-types-discards-qualifiers
LD      := ld
LDFLAGS := $(addprefix -L, $(LIBDIR))
LIBS    := -lm -lc -lpthread

TESTBIN := $(addprefix $(BINDIR), $(addsuffix .test, $(SOURCE)))

all: $(BINDIR)$(TARGET) $(TESTBIN)
	tree

run: $(BINDIR)$(TARGET)
	./$^ $(ARGS)

dbg: $(BINDIR)$(TARGET)
	$(DBG) $(DBGARGS) -- ./$^

tests.dbg: $(TESTBIN)
	for test in $^; do $(DBG) $(DBGARGS) -- ./$${test}; done

tests.exec: $(TESTBIN)
	for test in $^; do ./$${test}; done

tests: tests.dbg # default

clean:
	$(RM) -rf \
	$(addprefix $(LIBDIR), $(addsuffix .a, $(SOURCE))) \
	$(addprefix $(OBJDIR), $(addsuffix .o, $(SOURCE))) \
	$(addprefix $(BINDIR), $(addsuffix .test, $(SOURCE))) \
	$(addprefix $(BINDIR), $(TARGET)) $(BINDIR)*.dSYM

mkfile: $(SRCDIR) $(INCDIR) $(TESTDIR)
	test -n "$(NAME)" # NAME= is required!
	touch $(addprefix $(INCDIR),  $(addsuffix .h, $(NAME)))
	touch $(addprefix $(SRCDIR),  $(addsuffix .c, $(NAME)))
	touch $(addprefix $(TESTDIR), $(addsuffix .c, $(NAME)))

$(BINDIR) $(LIBDIR) $(OBJDIR) $(INCDIR) $(SRCDIR) $(TESTDIR) %/:
	$(MKDIR) -p $@

$(OBJDIR)%.o: $(OBJDIR)
$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) -o $@ -c $^

$(LIBDIR)%.a: $(LIBDIR)
$(LIBDIR)%.a: $(OBJDIR)%.o
	$(AR) $(ARFLAGS) $@ $^

$(BINDIR)%: $(BINDIR)
$(BINDIR)$(TARGET): $(addprefix $(LIBDIR), $(addsuffix .a, $(SOURCE) main))
	$(LD) $(LDFLAGS) $(LIBS) $(ARCH) -o $@ $^

$(OBJDIR)%_test.o: $(TESTDIR)%.c
	$(CC) $(CFLAGS) -o $@ -c $^

$(BINDIR)%.test: \
	$(addprefix $(LIBDIR), $(addsuffix .a, %)) \
	$(addprefix $(OBJDIR), $(addsuffix _test.o, %))
	$(LD) $(LDFLAGS) $(LIBS) $(ARCH) -o $@ $^

#TESTPRE := test_
#$(OBJDIR)$(TESTPRE)%.o: $(TESTDIR)$(TESTPRE)%.c
#	$(CC) $(CFLAGS) -o $@ -c $^
#$(LIBDIR)$(TESTPRE)%.a: $(OBJDIR)$(TESTPRE)%.o
#	$(AR) $(ARFLAGS) $@ $^
