######################################
# RISC-V Example Project by A_D
# https://adelectronics.ru/
######################################

######################################
# User sources
######################################
C_USER_SOURCES = \
Src/main.c \
Src/user_config.c \
Src/handlers.c \
Src/systick.c \
DriversExt/ST7735.c \
DriversExt/fonts.c \
Drivers/NMSIS/DSP/Source/BasicMathFunctions/BasicMathFunctions.c \
Drivers/NMSIS/DSP/Source/FastMathFunctions/FastMathFunctions.c \
Drivers/NMSIS/DSP/Source/TransformFunctions/TransformFunctions.c \
Drivers/NMSIS/DSP/Source/FilteringFunctions/FilteringFunctions.c \
Drivers/NMSIS/DSP/Source/ComplexMathFunctions/ComplexMathFunctions.c \
Drivers/NMSIS/DSP/Source/CommonTables/CommonTables.c \
Drivers/NMSIS/DSP/Source/CommonTables/riscv_twiddlecoef_tables.c

######################################
# User includes
######################################
C_USER_INCLUDES = \
-IInc \
-ISrc \
-IDriversExt \
-IDrivers/NMSIS/Core/Include \
-IDrivers/NMSIS/DSP/Include \
-IDrivers/NMSIS/NN/Include

######################################
# Driver sources
######################################
C_SOURCES = \
Drivers/GD32VF103_standard_peripheral/system_gd32vf103.c \
Drivers/GD32VF103_standard_peripheral/Source/gd32vf103_adc.c \
Drivers/GD32VF103_standard_peripheral/Source/gd32vf103_bkp.c \
Drivers/GD32VF103_standard_peripheral/Source/gd32vf103_can.c \
Drivers/GD32VF103_standard_peripheral/Source/gd32vf103_crc.c \
Drivers/GD32VF103_standard_peripheral/Source/gd32vf103_dac.c \
Drivers/GD32VF103_standard_peripheral/Source/gd32vf103_dbg.c \
Drivers/GD32VF103_standard_peripheral/Source/gd32vf103_dma.c \
Drivers/GD32VF103_standard_peripheral/Source/gd32vf103_eclic.c \
Drivers/GD32VF103_standard_peripheral/Source/gd32vf103_exmc.c \
Drivers/GD32VF103_standard_peripheral/Source/gd32vf103_exti.c \
Drivers/GD32VF103_standard_peripheral/Source/gd32vf103_fmc.c \
Drivers/GD32VF103_standard_peripheral/Source/gd32vf103_fwdgt.c \
Drivers/GD32VF103_standard_peripheral/Source/gd32vf103_gpio.c \
Drivers/GD32VF103_standard_peripheral/Source/gd32vf103_i2c.c \
Drivers/GD32VF103_standard_peripheral/Source/gd32vf103_pmu.c \
Drivers/GD32VF103_standard_peripheral/Source/gd32vf103_rcu.c \
Drivers/GD32VF103_standard_peripheral/Source/gd32vf103_rtc.c \
Drivers/GD32VF103_standard_peripheral/Source/gd32vf103_spi.c \
Drivers/GD32VF103_standard_peripheral/Source/gd32vf103_timer.c \
Drivers/GD32VF103_standard_peripheral/Source/gd32vf103_usart.c \
Drivers/GD32VF103_standard_peripheral/Source/gd32vf103_wwdgt.c \
Drivers/RISCV/drivers/n200_func.c \
Drivers/RISCV/stubs/_exit.c \
Drivers/RISCV/stubs/close.c \
Drivers/RISCV/stubs/fstat.c \
Drivers/RISCV/stubs/isatty.c \
Drivers/RISCV/stubs/lseek.c \
Drivers/RISCV/stubs/read.c \
Drivers/RISCV/stubs/sbrk.c \
Drivers/RISCV/stubs/write.c \
Drivers/RISCV/stubs/write_hex.c

ASM_SOURCES = \
entry.S \
start.S

######################################
# Driver includes
######################################

C_INCLUDES = \
-IDrivers/GD32VF103_standard_peripheral/Include \
-IDrivers/GD32VF103_standard_peripheral \
-IDrivers/RISCV/drivers \
-IDrivers/RISCV/stubs

######################################
# Build dir, name
######################################
TARGET = Firmware
BUILD_DIR = build

#######################################
# Binaries
#######################################
PREFIX = riscv-none-embed-

ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc -std=gnu11
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# Definitions
#######################################
ASM_DEFS =

C_DEFS = -DHXTAL_VALUE=25000000 -D__SYSTEM_CLOCK_108M_PLL_HXTAL=108000000 -DRISCV_ALL_FFT_TABLES
# -DUSE_STDPERIPH_DRIVER=1

#######################################
# Flags
#######################################

ARCH = rv32imac
ABI = ilp32

COMP_OPT = -Og -flto -g
MCU = -march=$(ARCH) -mabi=$(ABI) -fno-pie
DEPENDS = -MMD -MP
WARNINGS = -Wall -Wextra -Wshadow -Wformat=2 -Wformat-truncation=2 -Wundef
WARNINGS += -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-undef -Wno-sign-compare

ASFLAGS = $(COMP_OPT) $(MCU) -ggdb -pipe $(DEPENDS) $(WARNINGS) $(ASM_DEFS) $(C_INCLUDES) $(C_USER_INCLUDES)
CFLAGS = $(COMP_OPT) $(MCU) -ggdb -pipe $(DEPENDS) $(WARNINGS) $(C_DEFS) $(C_INCLUDES) $(C_USER_INCLUDES)
CFLAGS += -fno-common -ffunction-sections -fdata-sections
CFLAGS += -ffreestanding -ftls-model=local-exec

LDSCRIPT = GD32VF103xB.lds

LIBS = -lgcc -lc

LDFLAGS = $(COMP_OPT) $(MCU) $(LIBS) -T$(LDSCRIPT) -static -Wl,-O1,--gc-sections,--relax,--build-id=none -Wl,-Map=$(BUILD_DIR)/$(TARGET).map
LDFLAGS  += -nostdlib

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

#######################################
# Build
#######################################

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(C_USER_SOURCES:.c=.o)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.S=.o)))

vpath %.c $(sort $(dir $(C_SOURCES)))
vpath %.c $(sort $(dir $(C_USER_SOURCES)))
vpath %.S $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo $<
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	@echo $<
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@
	
$(BUILD_DIR):
	mkdir $@

#######################################
# Clean
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# Dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)
