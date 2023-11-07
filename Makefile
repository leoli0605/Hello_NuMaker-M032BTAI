# ------------------------------------------------
# Generic Makefile (based on gcc)
# ------------------------------------------------

######################################
# target
######################################
TARGET = TRSP_UART_Central


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -O3 # -O0, -O1, -O2, -O3, -Os, -Ofast or -Og for debug build (default)


#######################################
# paths
#######################################
# Build path
BUILD_DIR = "Source/build"

######################################
# source
######################################
# C sources
C_SOURCES =  \
Source/BleAppProfile/ble_profile_app.c \
Source/BleAppProfile/ble_profile_def.c \
Source/Lib/ble_bonding_storage.c \
Source/Lib/ble_LL_ref.c \
Source/Lib/ble_msgblock.c \
Source/Library/Device/Nuvoton/M031/Source/GCC/_syscalls.c \
Source/Library/Device/Nuvoton/M031/Source/system_M031Series.c \
Source/Library/StdDriver/src/clk.c \
Source/Library/StdDriver/src/fmc.c \
Source/Library/StdDriver/src/gpio.c \
Source/Library/StdDriver/src/pdma.c \
Source/Library/StdDriver/src/retarget.c \
Source/Library/StdDriver/src/spi.c \
Source/Library/StdDriver/src/sys.c \
Source/Library/StdDriver/src/timer.c \
Source/Library/StdDriver/src/uart.c \
Source/Library/StdDriver/src/wdt.c \
Source/main.c \
Source/Porting/dev_addr.c \
Source/Porting/porting_flash.c \
Source/Porting/porting_misc.c \
Source/Porting/porting_rfpower.c \
Source/Porting/porting_spi.c \
Source/Service/BleServiceBase/ble_service_base.c \
Source/Service/DIS/ble_service_dis.c \
Source/Service/GAP/ble_service_gap.c \
Source/Service/GATT/ble_service_gatt.c \
Source/Service/UDF01S/ble_service_udf01s.c \
Source/user.c

# ASM sources
ASM_SOURCES =  \
Source/Library/Device/Nuvoton/M031/Source/GCC/startup_M031Series.S


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
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
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m0

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS =

# C defines
C_DEFS =


# AS includes
AS_INCLUDES =

# C includes
C_INCLUDES =  \
-ISource/BleAppProfile \
-ISource/Library/CMSIS/Include \
-ISource/Library/StdDriver/inc \
-ISource/Library/Device/Nuvoton/M031/Include \
-ISource/Include \
-ISource/Porting \
-ISource/Service/BleServiceBase \
-ISource/Service/DIS \
-ISource/Service/GAP \
-ISource/Service/GATT \
-ISource/Service/UDF01S \
-ISource


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = Source/Toolchain/GCC/M031_128K.ld

# libraries
LIBS = -lc -lm -lnosys -lBLELIB_GCC_C
LIBDIR = -LSource/Lib
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
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
# clean up
#######################################
clean:
ifeq ($(OS),Windows_NT)
	del /Q /F $(BUILD_DIR)
else
	rm -rf $(BUILD_DIR)
endif

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***