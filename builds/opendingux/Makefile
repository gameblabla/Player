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

CFLAGS = -std=gnu++23
# -DHAVE_FLUIDLITE=1
CFLAGS +=  -DLOW_MEMORY_DEVICES=1 -DDREAMCAST=1 -DWANT_DRWAV=1 -DSUPPORT_AUDIO=1 -DWANT_FMMIDI=1 -DSUPPORT_JOYSTICK=1 -DSUPPORT_JOYSTICK_AXIS=1 -DLIBPNG_DEP=1
CFLAGS += -I/opt/toolchains/dc/kos/addons/include -I/opt/toolchains/dc/kos/kernel/arch/dreamcast/include -I/opt/toolchains/dc/kos/include -I/opt/toolchains/dc/sh-elf/sh-elf/include
CFLAGS += -I/opt/toolchains/dc/kos-ports/include/zlib/ -I/opt/toolchains/dc/kos-ports/include/png/ -I/opt/toolchains/dc/kos-ports/include/SDL/ -I. -Isounddc -Iinclude

PKG_CONFIG :=
PNG_LIBS := 
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
SOURCES = ../../src ../../src/platform/dreamcast ../../lib/liblcf/src ../../lib/liblcf/src/generated
#./sounddc

CPPFILES = $(foreach dir, $(SOURCES), $(wildcard $(dir)/*.cpp))
CFILES = $(foreach dir, $(SOURCES), $(wildcard $(dir)/*.c))

OBJS = $(CPPFILES:.cpp=.o)
OBJS += $(CFILES:.c=.o)

# Compiler flags
CFLAGS += -Wall -DNDEBUG $(PIXMAN_CFLAGS) $(FREETYPE_CFLAGS) $(ICU_CFLAGS) $(LIBMPG123_CFLAGS) $(LIBSNDFILE_CFLAGS) $(LIBSAMPLERATE_CFLAGS) $(FLUIDLITE_CFLAGS) $(SPEEX_CFLAGS)
CXXFLAGS += $(CFLAGS) -fno-rtti -fno-exceptions 

# Host compiler and extra flags
CFLAGS += -DOPENDINGUX

# Start compile
$(TARGET): $(OBJS)
	kos-cc $(CFLAGS) $(OBJS) -o $@  -lstdc++ -lSDLaudio  -lz -lpixman-1 -lpng -Wl,--as-needed -Wl,--gc-sections -s -flto -Wl,-Map=output.map

.cpp.o:
	kos-c++ $(INCDIR) $(CXXFLAGS) -c $< -o $@
	
.c.o:
	kos-cc $(INCDIR) $(CFLAGS) -c $< -o $@
	
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
