# tools
CC			:= arm-none-eabi-gcc
OBJCOPY		:= arm-none-eabi-objcopy
MKDIR		:= mkdir

# preprocessing
RES			:= src/res.c

# input
SOURCES_S	:= src/gba_crt0.s
SOURCES_C	:= src/main.c src/syscalls.c

# output
ELF			:= build/game.elf
ROM			:= build/game.gba
MAP			:= build/game.map

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
		   -specs=nano.specs -T src/gba_cart.ld \
		   -Wl,--start-group -lc -Wl,--end-group

# intermediary object files
OBJS		:= \
	$(patsubst src/%.s,build/%_s.o,$(SOURCES_S)) \
	$(patsubst src/%.c,build/%_c.o,$(SOURCES_C))

build/%_s.o : src/%.s
	@echo "  COMPILE $<"
	@$(MKDIR) -p $(@D)
	@$(CC) $(ASFLAGS) -MMD -MP -c -o $@ $<

build/%_c.o : src/%.c
	@echo "  COMPILE $<"
	@$(MKDIR) -p $(@D)
	@$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

# targets
.PHONY: all clean

all: $(ROM)

$(RES): $(wildcard res/*)
	@echo "  CREATE  $@"
	@gcc -o res/bmp_to_rom res/bmp_to_rom.c
	@./res/bmp_to_rom $(RES)
	@rm res/bmp_to_rom

$(ELF): $(RES) $(OBJS)
	@echo "  LINK    $@"
	@$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(ROM): $(ELF)
	@echo "  CREATE  $@"
	@$(OBJCOPY) -O binary $< $@

clean:
	@echo "  CLEAN"
	@rm -rf build
	@rm $(RES)