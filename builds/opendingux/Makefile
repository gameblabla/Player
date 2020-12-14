##################################################################
#         Makefile for OpenDingux (Dingoo A320/GCW-Zero)         #
#                            By Shin-NiL                         #
#                     - Based on Rikku2000's -                   #
##################################################################

# Compiler Device option
BUILD_TARGET ?= funkey

# Host toolchain directory and extra flags
ifeq ($(BUILD_TARGET), dingoo)
	TARGET = dingoo/EasyRPG
	TOOLCHAINDIR ?= /opt/opendingux-toolchain
	SYSROOT = $(TOOLCHAINDIR)
	BINDIR = $(TOOLCHAINDIR)/usr/bin
	CFLAGS = -O2 -fomit-frame-pointer -ffunction-sections -ffast-math -fno-math-errno -G0 -mbranch-likely -std=gnu++14
	LDFLAGS = -Wl,--gc-sections
	PNG_LIBS = -lpng12
	PIXMAN_CFLAGS := -I$(SYSROOT)/usr/include/pixman-1
	PIXMAN_LIBS := $(SYSROOT)/usr/lib/libpixman-1.a
	FREETYPE_CFLAGS := -I$(SYSROOT)/usr/include/freetype2
	ICU_CFLAGS := 
	ICU_LIBS := 
	LIBMPG123_CFLAGS := 
	LIBMPG123_LIBS := 
	LIBSNDFILE_CFLAGS :=
	LIBSNDFILE_LIBS :=
	CXXFLAGS = 
	SDL_CONFIG = $(SYSROOT)/usr/bin/sdl-config
	SDL_TARGET = 1
	LIBSDLMIXER_LIBS = -lSDL_mixer
	HUGE_PAGES = 0
	HOST = $(BINDIR)/mipsel-linux-
else ifeq ($(BUILD_TARGET), bittboy)
	TARGET = bittboy/EasyRPG
	TOOLCHAINDIR ?= /opt/bittboy-toolchain
	SYSROOT = $(TOOLCHAINDIR)/usr/arm-buildroot-linux-musleabi/sysroot
	BINDIR = $(TOOLCHAINDIR)/usr/bin
	CFLAGS = -Ofast -fno-math-errno -fomit-frame-pointer -fdata-sections -ffunction-sections -march=armv5te -mtune=arm926ej-s -std=gnu++14 -DLCF_SUPPORT_ICU -flto
	CFLAGS += -DGCW_ZERO -DHAVE_FREETYPE=1 -DHAVE_LIBSAMPLERATE=1 -DHAVE_LIBSNDFILE=1 -DHAVE_MPG123=1 -DSUPPORT_AUDIO=1 -DHAVE_FLUIDLITE=1
	LDFLAGS = -flto -Wl,--as-needed,--gc-sections -s
	PKG_CONFIG := $(BINDIR)/pkg-config
	PNG_LIBS := $(shell $(PKG_CONFIG) --libs libpng)
	PIXMAN_CFLAGS := $(shell $(PKG_CONFIG) --static --cflags pixman-1)
	PIXMAN_LIBS := -Wl,-Bstatic $(shell $(PKG_CONFIG) --static --libs pixman-1)
	FREETYPE_CFLAGS := $(shell $(PKG_CONFIG) --cflags freetype2)
	ICU_CFLAGS := $(shell $(PKG_CONFIG) --cflags icu-uc icu-i18n)
	ICU_LIBS := $(shell $(PKG_CONFIG) --libs icu-uc icu-i18n) -Wl,-Bdynamic
	LIBMPG123_CFLAGS := $(shell $(PKG_CONFIG) --cflags libmpg123)
	LIBMPG123_LIBS :=  $(shell $(PKG_CONFIG) --static --libs libmpg123)
	LIBSNDFILE_CFLAGS := $(shell $(PKG_CONFIG) --cflags sndfile)
	LIBSNDFILE_LIBS := $(shell $(PKG_CONFIG) --libs sndfile)
	LIBSAMPLERATE_CFLAGS := $(shell $(PKG_CONFIG) --cflags samplerate)
	LIBSAMPLERATE_LIBS := $(shell $(PKG_CONFIG) --static --libs samplerate)
	FLUIDLITE_CFLAGS := $(shell $(PKG_CONFIG) --cflags fluidlite)
	FLUIDLITE_LIBS := $(shell $(PKG_CONFIG) --static --libs fluidlite)
	CXXFLAGS = 
	SDL_CONFIG = $(SYSROOT)/usr/bin/sdl-config
	SDL_TARGET = 1
	HUGE_PAGES = 0
	HOST = $(BINDIR)/arm-linux-
else ifeq ($(BUILD_TARGET), funkey)
	TARGET = funkey/EasyRPG
	TOOLCHAINDIR ?= /opt/funkey-toolchain
	SYSROOT = $(TOOLCHAINDIR)/usr/arm-buildroot-linux-musleabihf/sysroot
	BINDIR = $(TOOLCHAINDIR)/usr/bin
	CFLAGS = -Ofast -fno-math-errno -fomit-frame-pointer -fdata-sections -ffunction-sections  -march=armv7-a+neon-vfpv4 -mtune=cortex-a7 -mfpu=neon-vfpv4 -std=gnu++14 -DLCF_SUPPORT_ICU -flto
	CFLAGS += -DGCW_ZERO -DHAVE_FREETYPE=1 -DHAVE_LIBSAMPLERATE=1 -DHAVE_LIBSNDFILE=1 -DHAVE_MPG123=1 -DSUPPORT_AUDIO=1 -DHAVE_FLUIDLITE=1 -DFUNKEY
	LDFLAGS = -flto -Wl,--as-needed,--gc-sections -s
	PKG_CONFIG := $(BINDIR)/pkg-config
	PNG_LIBS := $(shell $(PKG_CONFIG) --libs libpng)
	PIXMAN_CFLAGS := $(shell $(PKG_CONFIG) --static --cflags pixman-1)
	PIXMAN_LIBS := -Wl,-Bstatic $(shell $(PKG_CONFIG) --static --libs pixman-1)
	FREETYPE_CFLAGS := $(shell $(PKG_CONFIG) --cflags freetype2)
	ICU_CFLAGS := $(shell $(PKG_CONFIG) --cflags icu-uc icu-i18n)
	ICU_LIBS := $(shell $(PKG_CONFIG) --libs icu-uc icu-i18n) -Wl,-Bdynamic
	LIBMPG123_CFLAGS := $(shell $(PKG_CONFIG) --cflags libmpg123)
	LIBMPG123_LIBS :=  $(shell $(PKG_CONFIG) --static --libs libmpg123)
	LIBSNDFILE_CFLAGS := $(shell $(PKG_CONFIG) --cflags sndfile)
	LIBSNDFILE_LIBS := $(shell $(PKG_CONFIG) --libs sndfile)
	LIBSAMPLERATE_CFLAGS := $(shell $(PKG_CONFIG) --cflags samplerate)
	LIBSAMPLERATE_LIBS := $(shell $(PKG_CONFIG) --static --libs samplerate)
	FLUIDLITE_CFLAGS := $(shell $(PKG_CONFIG) --cflags fluidlite)
	FLUIDLITE_LIBS := $(shell $(PKG_CONFIG) --static --libs fluidlite)
	CXXFLAGS = 
	SDL_CONFIG = $(SYSROOT)/usr/bin/sdl-config
	SDL_TARGET = 1
	HUGE_PAGES = 0
	HOST = $(BINDIR)/arm-linux-
else ifeq ($(BUILD_TARGET), rs97)
	TARGET = rs97/EasyRPG
	TOOLCHAINDIR ?= /opt/rs97-toolchain
	SYSROOT = $(TOOLCHAINDIR)/usr/mipsel-buildroot-linux-musl/sysroot
	BINDIR = $(TOOLCHAINDIR)/usr/bin
	CFLAGS = -Ofast -fdata-sections -ffunction-sections -march=mips32 -mtune=mips32 -mno-mips16 -std=gnu++14 -DLCF_SUPPORT_ICU -flto
	CFLAGS += -mno-fp-exceptions -mno-check-zero-division -mframe-header-opt -fno-common -mno-micromips -mno-interlink-compressed -mno-abicalls -fno-PIC
	CFLAGS += -DGCW_ZERO -DHAVE_FREETYPE=1 -DHAVE_LIBSAMPLERATE=1 -DHAVE_LIBSNDFILE=1 -DHAVE_MPG123=1 -DSUPPORT_AUDIO=1 -DHAVE_FLUIDLITE=1
	LDFLAGS = -flto -Wl,--as-needed,--gc-sections  -s -flto
	PKG_CONFIG := $(BINDIR)/pkg-config
	PNG_LIBS := $(shell $(PKG_CONFIG) --libs libpng)
	PIXMAN_CFLAGS := $(shell $(PKG_CONFIG) --static --cflags pixman-1)
	PIXMAN_LIBS := -Wl,-Bstatic $(shell $(PKG_CONFIG) --static --libs pixman-1)
	FREETYPE_CFLAGS := $(shell $(PKG_CONFIG) --cflags freetype2)
	ICU_CFLAGS := $(shell $(PKG_CONFIG) --cflags icu-uc icu-i18n)
	ICU_LIBS := $(shell $(PKG_CONFIG) --libs icu-uc icu-i18n) -Wl,-Bdynamic
	LIBMPG123_CFLAGS := $(shell $(PKG_CONFIG) --cflags libmpg123)
	LIBMPG123_LIBS :=  $(shell $(PKG_CONFIG) --static --libs libmpg123)
	LIBSNDFILE_CFLAGS := $(shell $(PKG_CONFIG) --cflags sndfile)
	LIBSNDFILE_LIBS := $(shell $(PKG_CONFIG) --libs sndfile)
	LIBSAMPLERATE_CFLAGS := $(shell $(PKG_CONFIG) --cflags samplerate)
	LIBSAMPLERATE_LIBS := $(shell $(PKG_CONFIG) --static --libs samplerate)
	FLUIDLITE_CFLAGS := $(shell $(PKG_CONFIG) --cflags fluidlite)
	FLUIDLITE_LIBS := $(shell $(PKG_CONFIG) --static --libs fluidlite)
	CXXFLAGS = 
	SDL_CONFIG = $(SYSROOT)/usr/bin/sdl-config
	SDL_TARGET = 1
	HUGE_PAGES = 0
	HOST = $(BINDIR)/mipsel-linux-
else ifeq ($(BUILD_TARGET), 350h)
	TARGET = gcw-zero/EasyRPG
	TOOLCHAINDIR ?= /opt/gcw0-toolchain-static
	SYSROOT = $(TOOLCHAINDIR)/usr/mipsel-buildroot-linux-musl/sysroot
	BINDIR = $(TOOLCHAINDIR)/usr/bin
	CFLAGS = -Ofast -mno-abicalls -mno-shared -mno-memcpy -fno-builtin -fno-math-errno -fomit-frame-pointer -fdata-sections -ffunction-sections -march=mips32r2 -mtune=mips32r2 -std=gnu++14 -DLCF_SUPPORT_ICU -flto
	CFLAGS += -mno-fp-exceptions -mno-check-zero-division -mframe-header-opt -fno-common -mno-micromips -mno-interlink-compressed
	CFLAGS += -DGCW_ZERO -DHAVE_LIBSAMPLERATE=1 -DHAVE_LIBSNDFILE=1 -DHAVE_MPG123=1 -DSUPPORT_AUDIO=1 -DHAVE_FLUIDLITE=1
	LDFLAGS = -flto -Wl,--as-needed,--gc-sections -s
	PKG_CONFIG := $(BINDIR)/pkg-config
	PNG_LIBS := $(shell $(PKG_CONFIG) --libs libpng)
	PIXMAN_CFLAGS := $(shell $(PKG_CONFIG) --static --cflags pixman-1)
	PIXMAN_LIBS := -Wl,-Bstatic $(shell $(PKG_CONFIG) --static --libs pixman-1)
	FREETYPE_CFLAGS := $(shell $(PKG_CONFIG) --cflags freetype2)
	ICU_CFLAGS := $(shell $(PKG_CONFIG) --cflags icu-uc icu-i18n)
	ICU_LIBS := $(shell $(PKG_CONFIG) --libs icu-uc icu-i18n) -Wl,-Bdynamic
	LIBMPG123_CFLAGS := $(shell $(PKG_CONFIG) --cflags libmpg123)
	LIBMPG123_LIBS :=  $(shell $(PKG_CONFIG) --static --libs libmpg123)
	LIBSNDFILE_CFLAGS := $(shell $(PKG_CONFIG) --cflags sndfile)
	LIBSNDFILE_LIBS := $(shell $(PKG_CONFIG) --libs sndfile)
	LIBSAMPLERATE_CFLAGS := $(shell $(PKG_CONFIG) --cflags samplerate)
	LIBSAMPLERATE_LIBS := $(shell $(PKG_CONFIG) --static --libs samplerate)
	FLUIDLITE_CFLAGS := $(shell $(PKG_CONFIG) --cflags fluidlite)
	FLUIDLITE_LIBS := $(shell $(PKG_CONFIG) --static --libs fluidlite)
	CXXFLAGS = 
	SDL_CONFIG = $(SYSROOT)/usr/bin/sdl-config
	SDL_TARGET = 1
	HUGE_PAGES = 0
	HOST = $(BINDIR)/mipsel-linux-
else ifeq ($(BUILD_TARGET), gcwzero)
	TARGET = gcw-zero/EasyRPG
	TOOLCHAINDIR ?= /opt/gcw0-toolchain
	SYSROOT = $(TOOLCHAINDIR)/usr/mipsel-gcw0-linux-uclibc/sysroot
	BINDIR = $(TOOLCHAINDIR)/usr/bin
	CFLAGS = -Ofast -mplt -mno-shared -mno-memcpy -fno-builtin -fno-math-errno -fomit-frame-pointer -fdata-sections -ffunction-sections -march=mips32r2 -mtune=mips32r2 -std=gnu++14 -DLCF_SUPPORT_ICU -flto
	CFLAGS += -mno-fp-exceptions -mno-check-zero-division -mframe-header-opt -fno-common -mno-micromips -mno-interlink-compressed
	CFLAGS += -DGCW_ZERO -DHAVE_LIBSAMPLERATE=1 -DHAVE_LIBSNDFILE=1 -DHAVE_MPG123=1 -DSUPPORT_AUDIO=1 -DHAVE_FLUIDLITE=1
	LDFLAGS = -flto -Wl,--as-needed,--gc-sections -s
	PKG_CONFIG := $(BINDIR)/pkg-config
	PNG_LIBS := $(shell $(PKG_CONFIG) --libs libpng)
	PIXMAN_CFLAGS := $(shell $(PKG_CONFIG) --static --cflags pixman-1)
	PIXMAN_LIBS := -Wl,-Bstatic $(shell $(PKG_CONFIG) --static --libs pixman-1)
	FREETYPE_CFLAGS := $(shell $(PKG_CONFIG) --cflags freetype2)
	ICU_CFLAGS := $(shell $(PKG_CONFIG) --cflags icu-uc icu-i18n)
	ICU_LIBS := $(shell $(PKG_CONFIG) --libs icu-uc icu-i18n) -Wl,-Bdynamic
	LIBMPG123_CFLAGS := $(shell $(PKG_CONFIG) --cflags libmpg123)
	LIBMPG123_LIBS :=  $(shell $(PKG_CONFIG) --static --libs libmpg123)
	LIBSNDFILE_CFLAGS := $(shell $(PKG_CONFIG) --cflags sndfile)
	LIBSNDFILE_LIBS := $(shell $(PKG_CONFIG) --libs sndfile)
	LIBSAMPLERATE_CFLAGS := $(shell $(PKG_CONFIG) --cflags samplerate)
	LIBSAMPLERATE_LIBS := $(shell $(PKG_CONFIG) --static --libs samplerate)
	FLUIDLITE_CFLAGS := $(shell $(PKG_CONFIG) --cflags fluidlite)
	FLUIDLITE_LIBS := $(shell $(PKG_CONFIG) --static --libs fluidlite)
	CXXFLAGS = 
	SDL_CONFIG = $(SYSROOT)/usr/bin/sdl-config
	SDL_TARGET = 1
	HUGE_PAGES = 1
	HOST = $(BINDIR)/mipsel-linux-
else ifeq ($(BUILD_TARGET), pc)
	TARGET = pc/EasyRPG
	TOOLCHAINDIR ?= 
	SYSROOT = 
	BINDIR = $(TOOLCHAINDIR)/usr/bin
	CFLAGS = -O0 -g3
	CFLAGS += -DGCW_ZERO -DHAVE_FREETYPE=1 -DHAVE_LIBSAMPLERATE=1 -DHAVE_LIBSNDFILE=1 -DHAVE_MPG123=1 -DSUPPORT_AUDIO=1
	LDFLAGS = 
	PKG_CONFIG := $(BINDIR)/pkg-config
	PNG_LIBS := $(shell $(PKG_CONFIG) --libs libpng)
	PIXMAN_CFLAGS := $(shell $(PKG_CONFIG) --static --cflags pixman-1)
	PIXMAN_LIBS := -Wl,-Bstatic $(shell $(PKG_CONFIG) --static --libs pixman-1)
	FREETYPE_CFLAGS := $(shell $(PKG_CONFIG) --cflags freetype2)
	ICU_CFLAGS := $(shell $(PKG_CONFIG) --cflags icu-uc icu-i18n)
	ICU_LIBS := $(shell $(PKG_CONFIG) --libs icu-uc icu-i18n) -Wl,-Bdynamic
	LIBMPG123_CFLAGS := $(shell $(PKG_CONFIG) --cflags libmpg123)
	LIBMPG123_LIBS :=  $(shell $(PKG_CONFIG) --static --libs libmpg123)
	LIBSNDFILE_CFLAGS := $(shell $(PKG_CONFIG) --cflags sndfile)
	LIBSNDFILE_LIBS := $(shell $(PKG_CONFIG) --libs sndfile)
	LIBSAMPLERATE_CFLAGS := $(shell $(PKG_CONFIG) --cflags samplerate)
	LIBSAMPLERATE_LIBS := $(shell $(PKG_CONFIG) --static --libs samplerate)
	FLUIDLITE_CFLAGS := $(shell $(PKG_CONFIG) --cflags fluidlite)
	FLUIDLITE_LIBS := $(shell $(PKG_CONFIG) --static --libs fluidlite)
	CXXFLAGS = 
	SDL_CONFIG = $(SYSROOT)/usr/bin/sdl-config
	SDL_TARGET = 1
	HUGE_PAGES = 0
	HOST = 
else
$(error Unknown device $(BUILD_TARGET)! Valid choices: dingoo, gcwzero)
endif

# Compiler headers
INCLUDES = ../../src ../../lib/liblcf/src ../../lib/liblcf/src/generated
INCDIR = $(foreach dir, $(INCLUDES), -I$(wildcard $(dir)))

# Compiler source
SOURCES = ../../src ../../src/platform/opendingux ../../lib/liblcf/src ../../lib/liblcf/src/generated
CPPFILES = $(foreach dir, $(SOURCES), $(wildcard $(dir)/*.cpp))
OBJS = $(CPPFILES:.cpp=.o)

# Compiler flags
CFLAGS += -Wall -DNDEBUG -DUSE_SDL=$(SDL_TARGET) `$(SDL_CONFIG) --cflags` $(PIXMAN_CFLAGS) $(FREETYPE_CFLAGS) $(ICU_CFLAGS) $(LIBMPG123_CFLAGS) $(LIBSNDFILE_CFLAGS) $(LIBSAMPLERATE_CFLAGS) $(FLUIDLITE_CFLAGS)
CXXFLAGS += $(CFLAGS) -fno-rtti -fno-exceptions -ftree-vectorize
LDFLAGS += -static-libgcc -lc -lgcc -lm -ldl -lstdc++ -lz -lfmt \
            `$(SDL_CONFIG) --libs` $(PIXMAN_LIBS) $(ICU_LIBS) $(PNG_LIBS) $(LIBMPG123_LIBS) $(LIBSNDFILE_LIBS) $(LIBSAMPLERATE_LIBS) $(FLUIDLITE_LIBS) -lvorbisidec -logg

ifeq ($(HUGE_PAGES), 1)
LDFLAGS		+= -Wl,-zcommon-page-size=2097152 -Wl,-zmax-page-size=2097152 -lhugetlbfs
endif

# Host compiler and extra flags
CFLAGS += -DOPENDINGUX

# Start compile
$(TARGET): $(OBJS)
	$(HOST)gcc $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)
ifeq ($(HUGE_PAGES), 1)
	hugeedit --text --data $(TARGET)
endif

.cpp.o:
	$(HOST)gcc $(INCDIR) $(CXXFLAGS) -c $< -o $@

# Clean Project
clean:
	rm -f $(OBJS) $(TARGET) 
