
CC = gcc

CPPFLAGS += -D _GNU_SOURCE
CPPFLAGS += -I .

CFLAGS += -Wall -Werror -Wfatal-errors

LDLIBS += -lgmp
#LDLIBS += -lstdc++
LDLIBS += -lGL -lglfw -lm
LDLIBS += -ldl

buildtype ?= release

ifeq ($(buildtype), release)
CPPFLAGS += -D RELEASE

CFLAGS += -O2
CFLAGS += -flto

LDFLAGS += -flto=auto
#LDFLAGS += -static

else ifeq ($(buildtype), test)
CPPFLAGS += -D TESTING

CFLAGS += -g
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-unused-function
CFLAGS += -Wno-unused-but-set-variable
CFLAGS += -Wno-unused-label

else ifeq ($(buildtype), debug)
CPPFLAGS += -D DEBUGGING
CPPFLAGS += -D ZEBU_DEBUG

CFLAGS += -g
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-unused-function
CFLAGS += -Wno-unused-but-set-variable
CFLAGS += -Wno-unused-label

else
$(error "invalid buildtype!");
endif

verbose ?= yes
ifeq ($(verbose), yes)
CPPFLAGS += -D VERBOSE
else ifeq ($(verbose), no)
else
$(error "invalid verbose!");
endif

on_error ?= do_nothing
ifeq ($(on_error), do_nothing)
ON_ERROR =
else ifeq ($(on_error), open_editor)
ON_ERROR += || ($$EDITOR $<; false)
else
$(error "invalid on_error option!");
endif

buildprefix = bin/$(buildtype)-build/$(verbose)-verbose
depprefix   = dep/$(buildtype)-build/$(verbose)-verbose

default: $(buildprefix)/money

ARGS += -v

ARGS += -p

#ARGS += -i ./examples/test.money
ARGS += -i ~/money/arbor-checking.money

run: $(buildprefix)/money
	$< $(ARGS)

valrun: $(buildprefix)/money
	valgrind $< $(ARGS)

valrun-stop: $(buildprefix)/money
	valgrind --gen-suppressions=yes -- $< ${ARGS}

valrun-leak: $(buildprefix)/money
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 -- $< ${ARGS}

tracerun: $(buildprefix)/money
	strace $< $(ARGS)

install: $(buildprefix)/money
	@ mkdir -vp ~/bin/
	@ cp -vau $(buildprefix)/money ~/bin/money

.PRECIOUS: %/

%/:
	@ mkdir -p $@

srclist.mk:
	@ echo "searching for source files..."
	@ find -name '*.c' -! -path '*/-*' | sort -V | sed 's/^/srcs += /' > $@

ifneq "$(MAKECMDGOALS)" "clean"
include srclist.mk
srcs += ./zebu.c
srcs += ./glad/glad.c
srcs += ./shaders/fragment.c
srcs += ./shaders/vertex.c
endif

objs := $(patsubst %.c,$(buildprefix)/%.o,$(srcs))
objs := $(patsubst %.cpp,$(buildprefix)/%.o,$(objs))

deps := $(patsubst %.c,$(depprefix)/%.d,$(srcs))
deps := $(patsubst %.cpp,$(depprefix)/%.d,$(deps))

zebu.c zebu.h dep/zebu.d: zebu.zb | dep/
	zebu -v -m --template=fileio -i $< -o zebu -MF dep/zebu.d

glad/glad.c glad/glad.h:
	glad --out-path glad --generator c --local-files

bin/escape: shaders/-escape.c | bin/
	gcc -Wall -Werror $< -o $@

shaders/%.c shaders/%.h: bin/escape shaders/%.glsl
	$^ -o shaders/$*.c -v $*_source

$(buildprefix)/%.o $(depprefix)/%.d: %.cpp | $(buildprefix)/%/ $(depprefix)/%/
	@ echo "compiling $<"
	@ $(CC) -c $(CPPFLAGS) $(CFLAGS) $< -MMD -o $(buildprefix)/$*.o -MF $(depprefix)/$*.d $(ON_ERROR)

$(buildprefix)/%.o $(depprefix)/%.d: %.c | $(buildprefix)/%/ $(depprefix)/%/
	@ echo "compiling $<"
	@ $(CC) -c $(CPPFLAGS) $(CFLAGS) $< -MMD -o $(buildprefix)/$*.o -MF $(depprefix)/$*.d $(ON_ERROR)

$(buildprefix)/money: $(objs)
	@ echo "linking $@"
	@ $(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	for l in $$(cat .gitignore); do rm -rvf $$l; done

ifneq "$(MAKECMDGOALS)" "clean"
include $(deps) dep/zebu.d
endif
















