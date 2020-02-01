#!/usr/bin/env make -f

PROJECT := main
MODULES := module
TARGET  := $(PROJECT).exe
SYSTEM  := $(shell uname)

CC   := cc
LD   := ld
AR   := ar
DBG  := lldb
LIBS := $(addprefix -l, c m dl pthread)

CFLAGS   := -g -O0 -fPIC
LDFLAGS  := -v
ARFLAGS  := -rcs
DBGFLAGS := -o run

LIBRARY  := $(addsuffix .so, module)

ifeq ($(SYSTEM),Linux)
LD       := ld
LDFLAGS  += -tt -E $(LIBS)
endif

ifeq ($(SYSTEM),Darwin)
LDFLAGS  += -arch x86_64 $(LIBS)
#LIBS     += $(addprefix -l, System)
endif

#$(LIBRARY): $(addsuffix .a, $(MODULES))

all: $(TARGET) $(LIBRARY)

auto:
	watchexec -v -n -c -e 'h,c' $(MAKE) clean all test

test: $(TARGET) $(LIBRARY)
	ldd $<
	./$^

debug: $(TARGET) $(LIBRARY)
	$(DBG) $(DBGFLAGS) -- $^

clean:
	rm -rf \
	$(addsuffix .so, $(MODULES)) \
	$(addsuffix .a,  $(MODULES)) \
	$(addsuffix .o,  $(MODULES)) \
	$(TARGET)

%.exe: %.a
	$(LD) $(LDFLAGS) -pie -o $@ $^ # $(LIBS)

%.so: %.a
	$(LD) $(LDFLAGS) -pie -o $@ $^ # $(LIBS)

%.a: %.o
	$(AR) $(ARFLAGS)  $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^
