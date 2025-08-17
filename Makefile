# User config
# ===========

# ROM config
# ----------

NAME		:= first
GAME_TITLE	:= "FIRST"
GAME_CODE	:= "00"

# include paths
INCLUDES	:= # A list of paths

# tools
CC			:= arm-none-eabi-gcc
OBJCOPY		:= arm-none-eabi-objcopy
MKDIR		:= mkdir

# directories
SOURCEDIR	:= source
BUILDDIR	:= build

# build artifacts
ELF		:= $(NAME).elf
ROM		:= $(NAME).gba
MAP		:= $(NAME).map

# source files
SOURCES_S	:= $(wildcard $(SOURCEDIR)/*.s $(SOURCEDIR)/**/*.s)
SOURCES_C	:= $(wildcard $(SOURCEDIR)/*.c $(SOURCEDIR)/**/*.c)

# compiler and linker flags
DEFINES		:= -D__GBA__

ARCH		:= -mcpu=arm7tdmi -mtune=arm7tdmi

ASFLAGS		+= -x assembler-with-cpp $(DEFINES) $(ARCH) \
		   -mthumb -mthumb-interwork \
		   -ffunction-sections -fdata-sections

CFLAGS		+= -std=gnu11 -Wall $(DEFINES) $(ARCH) \
		   -mthumb -mthumb-interwork -O3 \
		   -ffunction-sections -fdata-sections

LDFLAGS		:= -mthumb -mthumb-interwork \
		   -Wl,-Map,$(MAP) -Wl,--gc-sections \
		   -specs=nano.specs -T source/sys/gba_cart.ld \
		   -Wl,--start-group -lc -Wl,--end-group

# tntermediate build files
OBJS		:= \
	$(patsubst $(SOURCEDIR)/%.s,$(BUILDDIR)/%.s.o,$(SOURCES_S)) \
	$(patsubst $(SOURCEDIR)/%.c,$(BUILDDIR)/%.c.o,$(SOURCES_C)) \
	$(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.cpp.o,$(SOURCES_CPP))

DEPS		:= $(OBJS:.o=.d)

# tules
$(BUILDDIR)/%.s.o : $(SOURCEDIR)/%.s
	@echo "  AS      $<"
	@$(MKDIR) -p $(@D) # Build target's directory if it doesn't exist
	@$(CC) $(ASFLAGS) -MMD -MP -c -o $@ $<

$(BUILDDIR)/%.c.o : $(SOURCEDIR)/%.c
	@echo "  CC      $<"
	@$(MKDIR) -p $(@D) # Build target's directory if it doesn't exist
	@$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<


# targets
.PHONY: all clean

all: $(ROM)

$(ELF): $(OBJS)
	@echo "  LD      $@"
	@$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(ROM): $(ELF)
	@echo "  OBJCOPY $<"
	@$(OBJCOPY) -O binary $< $@

clean:
	@echo "  CLEAN"
	@rm -rf $(ROM) $(ELF) $(MAP) $(BUILDDIR)