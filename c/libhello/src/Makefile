
ifeq ($(V),1)
	Q =
	msg =
else
	Q = @
	msg = @printf '  %-8s %s%s\n' "$(1)" "$(2)" "$(if $(3), $(3))";
endif

LIBHELLO_MAJOR_VERSION := 0
LIBHELLO_MINOR_VERSION := 2
LIBHELLO_PATCH_VERSION := 1
LIBHELLO_VERSION := $(LIBHELLO_MAJOR_VERSION).$(LIBHELLO_MINOR_VERSION).$(LIBHELLO_PATCH_VERSION)
LIBHELLO_MAJMIN_VERSION := $(LIBHELLO_MAJOR_VERSION).$(LIBHELLO_MINOR_VERSION).0
LIBHELLO_MAP_VERSION := $(shell grep -oE '^LIBHELLO_([0-9.]+)' libhello.map | sort -rV | head -n1 | cut -d'_' -f2)

ifneq ($(LIBHELLO_MAJMIN_VERSION), $(LIBHELLO_MAP_VERSION))
$(error Libhello release ($(LIBHELLO_VERSION)) and map ($(LIBHELLO_MAP_VERSION)) versions are out of sync!)
endif

define allow-override
  $(if $(or $(findstring environment,$(origin $(1))),\
            $(findstring command line,$(origin $(1)))),,\
    $(eval $(1) = $(2)))
endef

$(call allow-override,CC,$(CROSS_COMPILE)cc)
$(call allow-override,LD,$(CROSS_COMPILE)ld)


TOPDIR = ..

INCLUDES := -I. -I$(TOPDIR)/include
ALL_CFLAGS := $(INCLUDES)

SHARED_CFLAGS += -fPIC -fvisibility=hidden -DSHARED

CFLAGS ?= -g -O2 -Werror -Wall -std=c11
ALL_CFLAGS += $(CFLAGS) $(EXTRA_CFLAGS)
ALL_LDFLAGS += $(LDFLAGS) $(EXTRA_LDFLAGS)

OBJDIR ?= .
SHARED_OBJDIR := $(OBJDIR)/sharedobjs
STATIC_OBJDIR := $(OBJDIR)/staticobjs
OBJS := hello.o
SHARED_OBJS := $(addprefix $(SHARED_OBJDIR)/,$(OBJS))
STATIC_OBJS := $(addprefix $(STATIC_OBJDIR)/,$(OBJS))

STATIC_LIBS := $(OBJDIR)/libhello.a
ifndef BUILD_STATIC_ONLY
	SHARED_LIBS := $(OBJDIR)/libhello.so \
		       $(OBJDIR)/libhello.so.$(LIBHELLO_MAJOR_VERSION) \
		       $(OBJDIR)/libhello.so.$(LIBHELLO_VERSION)
	
	VERSION_SCRIPT := libhello.map
endif

HEADERS := hello.h

PC_FILE := $(OBJDIR)/libhello.pc

INSTALL = install

DESTDIR ?=

HOSTARCH = $(firstword $(subst -, ,$(shell $(CC) -dumpmachine)))
ifeq ($(filter-out %64 %64be %64eb %64le %64el s390x, $(HOSTARCH)),)
	LIBSUBDIR := lib64
else
	LIBSUBDIR := lib
endif

ifndef LIBDIR
	LIBDIR_PC := $$\{prefix\}/$(LIBSUBDIR)
else
	LIBDIR_PC := $(LIBDIR)
endif
PREFIX ?= /usr
LIBDIR ?= $(PREFIX)/$(LIBSUBDIR)
INCLUDEDIR ?= $(PREFIX)/include

TAGS_PROG := $(if $(shell which etags 2>/dev/null),etags,ctags)

all: $(STATIC_LIBS) $(SHARED_LIBS) $(PC_FILE)

$(OBJDIR)/libhello.a: $(STATIC_OBJS)
	$(call msg,AR,$@)
	$(Q)$(AR) rcs $@ $^

$(OBJDIR)/libhello.so: $(OBJDIR)/libhello.so.$(LIBHELLO_MAJOR_VERSION)
	$(Q)ln -sf $(^F) $@

$(OBJDIR)/libhello.so.$(LIBHELLO_MAJOR_VERSION): $(OBJDIR)/libhello.so.$(LIBHELLO_VERSION)
	$(Q)ln -sf $(^F) $@

$(OBJDIR)/libhello.so.$(LIBHELLO_VERSION): $(SHARED_OBJS)
	$(call msg,CC,$@)
	$(Q)$(CC) -shared -Wl,--version-script=$(VERSION_SCRIPT) \
		  -Wl,-soname,libhello.so.$(LIBHELLO_MAJOR_VERSION) \
		  $^ $(ALL_LDFLAGS) -o $@

$(OBJDIR)/libhello.pc: force
	$(Q)sed -e "s|@PREFIX@|$(PREFIX)|" \
		-e "s|@LIBDIR@|$(LIBDIR_PC)|" \
		-e "s|@VERSION@|$(LIBHELLO_VERSION)|" \
		< libhello.pc.template > $@
	  
$(STATIC_OBJDIR) $(SHARED_OBJDIR):
	$(call msg,MKDIR,$@)
	$(Q)mkdir -p $@

$(STATIC_OBJDIR)/%.o: %.c | $(STATIC_OBJDIR)
	$(call msg,CC,$@)
	$(Q)$(CC) $(ALL_CFLAGS) $(CPPFLAGS) -c $< -o $@

$(SHARED_OBJDIR)/%.o: %.c | $(SHARED_OBJDIR)
	$(call msg,CC,$@)
	$(Q)$(CC) $(ALL_CFLAGS) $(SHARED_CFLAGS) $(CPPFLAGS) -c $< -o $@

define do_install
	$(call msg,INSTALL,$1)
	$(Q)if [ ! -d '$(DESTDIR)$2' ]; then		\
		$(INSTALL) -d -m 755 '$(DESTDIR)$2';	\
	fi;
	$(Q)$(INSTALL) $(if $3,-m $3,) $1 '$(DESTDIR)$2'
endef


define do_install
	$(call msg,INSTALL,$1)
	$(Q)if [ ! -d '$(DESTDIR)$2' ]; then		\
		$(INSTALL) -d -m 755 '$(DESTDIR)$2';	\
	fi;
	$(Q)$(INSTALL) $(if $3,-m $3,) $1 '$(DESTDIR)$2'
endef

# Preserve symlinks at installation.
define do_s_install
	$(call msg,INSTALL,$1)
	$(Q)if [ ! -d '$(DESTDIR)$2' ]; then		\
		$(INSTALL) -d -m 755 '$(DESTDIR)$2';	\
	fi;
	$(Q)cp -fR $1 '$(DESTDIR)$2'
endef


install: all install_headers install_pkgconfig
	$(call do_s_install,$(STATIC_LIBS) $(SHARED_LIBS),$(LIBDIR))

install_headers:
	$(call do_install,$(HEADERS),$(INCLUDEDIR)/hello,644)

install_pkgconfig: $(PC_FILE)
	$(call do_install,$(PC_FILE),$(LIBDIR)/pkgconfig,644)

clean:
	$(call msg,CLEAN)
	$(Q)rm -rf *.o *.a *.so *.so.* *.pc $(SHARED_OBJDIR) $(STATIC_OBJDIR)

.PHONY: cscope tags force
cscope:
	$(call msg,CSCOPE)
	$(Q)ls *.c *.h > cscope.files
	$(Q)cscope -b -q -f cscope.out

tags:
	$(call msg,CTAGS)
	$(Q)rm -f TAGS tags
	$(Q)ls *.c *.h | xargs $(TAGS_PROG) -a

force:
