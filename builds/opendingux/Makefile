##################################################################
#         Makefile for OpenDingux (Dingoo A320/GCW-Zero)         #
#                            By Shin-NiL                         #
#                     - Based on Rikku2000's -                   #
##################################################################

# Compiler Device option
BUILD_TARGET ?= dreamcast
PROFILE ?= 0

TARGET = dc/EasyRPG
TOOLCHAINDIR ?= /opt/toolchains/dc/sh-elf
SYSROOT =
BINDIR = $(TOOLCHAINDIR)/bin

CFLAGS = -std=gnu++17
CFLAGS +=  -DLOW_MEMORY_DEVICES=1 -DDREAMCAST -DWANT_DRWAV=1 -DSUPPORT_AUDIO=1 -DWANT_FMMIDI=1
CFLAGS += -I/opt/toolchains/dc/kos/addons/include -I/opt/toolchains/dc/kos/kernel/arch/dreamcast/include -I/opt/toolchains/dc/kos/include -I/opt/toolchains/dc/sh-elf/sh-elf/include
CFLAGS += -I/opt/toolchains/dc/kos-ports/include/SDL/ -I/opt/toolchains/dc/kos-ports/include/zlib/ -I/opt/toolchains/dc/kos-ports/include/png/

PKG_CONFIG :=
PNG_LIBS := -lpng 
PIXMAN_CFLAGS := -I/opt/toolchains/dc/kos-ports/include/pixman-1 -I/opt/toolchains/dc/kos-ports/include
PIXMAN_LIBS := -lpixman-1
FREETYPE_CFLAGS := 
ICU_CFLAGS := 
ICU_LIBS := 
LIBMPG123_CFLAGS := 
LIBMPG123_LIBS :=  
LIBSNDFILE_CFLAGS := 
LIBSNDFILE_LIBS := 
LIBSAMPLERATE_CFLAGS := 
LIBSAMPLERATE_LIBS := 
FLUIDLITE_CFLAGS :=
FLUIDLITE_LIBS := 
SPEEX_CFLAGS := 
SPEEX_LIBS := 
CXXFLAGS = 
SDL_CONFIG = 
SDL_TARGET = 1
HUGE_PAGES = 0
HOST = sh-elf-

# Compiler headers
INCLUDES = ../../src ../../lib/liblcf/src ../../lib/liblcf/src/generated
INCDIR = $(foreach dir, $(INCLUDES), -I$(wildcard $(dir)))

# Compiler source
SOURCES = ../../src ../../src/platform/sdl ../../src/platform/opendingux ../../lib/liblcf/src ../../lib/liblcf/src/generated
CPPFILES = $(foreach dir, $(SOURCES), $(wildcard $(dir)/*.cpp))
OBJS = $(CPPFILES:.cpp=.o)

# Compiler flags
CFLAGS += -Wall -DNDEBUG -DUSE_SDL=$(SDL_TARGET) $(PIXMAN_CFLAGS) $(FREETYPE_CFLAGS) $(ICU_CFLAGS) $(LIBMPG123_CFLAGS) $(LIBSNDFILE_CFLAGS) $(LIBSAMPLERATE_CFLAGS) $(FLUIDLITE_CFLAGS) $(SPEEX_CFLAGS)
CXXFLAGS += $(CFLAGS) -fno-rtti -fno-exceptions

# Host compiler and extra flags
CFLAGS += -DOPENDINGUX

# Start compile
$(TARGET): $(OBJS)
	kos-cc $(CFLAGS) $(OBJS) -o $@  -lSDL -lstdc++ -lpng -lz -lpixman-1 -lfmt -Wl,--as-needed -Wl,--gc-sections -s

.cpp.o:
	kos-c++ $(INCDIR) $(CXXFLAGS) -c $< -o $@

pack:
	rm -f IP.BIN
	sh-elf-objcopy -R .stack -O binary dc/EasyRPG main.bin
	${KOS_BASE}/utils/scramble/scramble main.bin ./dc/cd/1ST_READ.BIN
	makeip ip.txt IP.BIN
	mkisofs -V disc -G IP.BIN -joliet -rock -l -x cd -o "disc.iso" ./dc/cd
	cdi4dc disc.iso disc.cdi -d

# Clean Project
clean:
	rm -f $(OBJS) $(TARGET) 
