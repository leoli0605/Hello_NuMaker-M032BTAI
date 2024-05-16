# ------------------------------------------------
# Generic Makefile (based on gcc)
# ------------------------------------------------

$(info make version is $(MAKE_VERSION))

######################################
# functions
######################################
fixpath = $(if $(findstring ",$(1)"),$(subst \,\\,$(1)),$(1))

define find_includes
$(if $(filter $(OS),Windows_NT),\
$(shell powershell -Command "$$dirs = Get-ChildItem -Path '$1' -Directory -Recurse | ForEach-Object { '-I' + (((Resolve-Path -Path $$_.FullName -Relative) -replace '\\', '/') -replace '^\./', '') }; $$dirs -join ' '"),\
$(shell find $1 -type d -exec echo -I{} \;))
endef

define find_sources
$(if $(filter $(OS),Windows_NT),\
$(shell powershell -Command "$$dirs = Get-ChildItem '$1' -Recurse -Filter '*.c' | ForEach-Object { ((Resolve-Path -Path $$_.FullName -Relative) -replace '\\', '/') -replace '^\./', '' }; $$dirs -join ' '"),\
$(shell find $1 -type f -name '*.c' -exec echo {} \;))
endef

######################################
# version / date
######################################

ifeq ($(OS),Windows_NT)
DATE = $(shell powershell -Command "(Get-Date).ToString('yyyy-MM-dd')")
else
DATE = $(shell date +%Y-%m-%d)
endif

$(info Date: $(DATE))

######################################
# target
######################################
TARGET = TRSP_UART_Central

######################################
# building variables
######################################
# debug build? 0: for release build (no debug info), 1: for debug build (with debug info)
DEBUG = 1
# optimization: -O0, -O1, -O2, -O3, -Os, -Ofast or -Og for debug build (default)
ifeq ($(DEBUG), 1)
OPT = -Og
else
OPT = -O2
endif

#######################################
# paths
#######################################
# Build path
ifeq ($(OS),Windows_NT)
# windows path: your\build\path
BUILD_DIR := $(call fixpath,Source\build)
OUT_DIR := $(call fixpath,Source\build\out)
else
# linux path: your/build/path
BUILD_DIR := Source/build
OUT_DIR := Source/build/out
endif

######################################
# source
######################################
# C sources
C_SOURCES =  \
$(call find_sources,Source/BleAppProfile) \
$(call find_sources,Source/Lib) \
$(call find_sources,Source/Library/StdDriver) \
$(wildcard Source/*.c) \
$(wildcard Source/Library/Device/Nuvoton/M031/Source/GCC/_syscalls.c) \
$(wildcard Source/Library/Device/Nuvoton/M031/Source/system_M031Series.c) \
$(wildcard Source/Porting/dev_addr.c) \
$(wildcard Source/Porting/porting_flash.c) \
$(wildcard Source/Porting/porting_misc.c) \
$(wildcard Source/Porting/porting_rfpower.c) \
$(wildcard Source/Porting/porting_spi.c) \
$(wildcard Source/Service/BleServiceBase/*.c) \
$(wildcard Source/Service/DIS/*.c) \
$(wildcard Source/Service/GAP/*.c) \
$(wildcard Source/Service/GATT/*.c) \
$(wildcard Source/Service/UDF01S/*.c) \

# $(info C_SOURCES: $(C_SOURCES))

# ASM sources
ASM_SOURCES =  \
Source/Library/Device/Nuvoton/M031/Source/GCC/startup_M031Series.S

# List of assembly source file names without directory paths and without extension
ASM_BASENAMES = $(basename $(notdir $(ASM_SOURCES)))

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
ifeq ($(CPU), -mcpu=cortex-m4)
FPU = -mfpu=fpv4-sp-d16
else ifeq ($(CPU), -mcpu=cortex-m7)
FPU = -mfpu=fpv5-sp-d16
else
$(info Cortex-M0/M0+/M3 is not supported by FPU)
FPU =
endif

# float-abi
ifeq ($(FPU),)
FLOAT-ABI = -mfloat-abi=soft
else
FLOAT-ABI = -mfloat-abi=hard
endif

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS =

# C defines
C_DEFS = \

ifeq ($(DEBUG), 1)
C_DEFS += -DDEBUG
else
C_DEFS += -DDISABLE_UART
endif

# AS includes
AS_INCLUDES =

# C includes
C_INCLUDES =  \
-ISource \
-ISource/BleAppProfile \
-ISource/Include \
-ISource/Porting \
$(call find_includes,Source/Library) \
$(call find_includes,Source/Service) \

# $(info C_INCLUDES: $(C_INCLUDES))

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections -Wno-unknown-pragmas

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections -Wno-unknown-pragmas

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
# CFLAGS += -Werror
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = Source/Toolchain/GCC/M032_512K.ld

# libraries
LIBS = -lc -lm -lnosys \
-lBLELIB_GCC_C
LIBDIR = \
-LSource/Lib
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -u _printf_float

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(OUT_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(OUT_DIR)/,$(notdir $(ASM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASM_SOURCES)))

$(OUT_DIR)/%.o: %.c Makefile | $(OUT_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(OUT_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(OUT_DIR)/%.o: %.S Makefile | $(OUT_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf | $(BUILD_DIR)
	$(BIN) $< $@

$(OUT_DIR):
ifeq ($(OS),Windows_NT)
	if not exist "$(OUT_DIR)" mkdir $(OUT_DIR)
else
	mkdir -p $(OUT_DIR)
endif

#######################################
# docker build
#######################################
docker:
ifeq ($(OS),Windows_NT)
	$(eval CURRENT_DIR := $(shell cd))
else
	$(eval CURRENT_DIR := $(shell pwd))
endif
	$(eval IMAGE_NAME := jafee201153/arm-none-eabi-gcc:13.2.Rel1-ubuntu-20.04)
	$(eval DOCKER_CMD := apt-get update && apt-get install -y jq zip && export TZ=Asia/Taipei && ln -snf /usr/share/zoneinfo/$$TZ /etc/localtime && echo $$TZ > /etc/timezone && DEBIAN_FRONTEND="noninteractive" apt-get install -y tzdata && bash)
	$(eval DOCKER_RUN := docker run -it --rm -v "$(CURRENT_DIR)":/share -w /share $(IMAGE_NAME))
ifeq ($(OS),Windows_NT)
	@powershell -Command "$$IMAGE_NAME='$(IMAGE_NAME)'; \
	$$DOCKER_RUN='$(DOCKER_RUN)'; \
	$$imageExists = docker images -q $$IMAGE_NAME; \
	if (-not $$imageExists) { \
		Write-Host 'Image $$IMAGE_NAME not found locally, pulling...'; \
		docker pull $$IMAGE_NAME; \
	}; \
	$$DOCKER_CMD = $$DOCKER_RUN + ' sh -c \"$(DOCKER_CMD)\"'; \
	Invoke-Expression $$DOCKER_CMD"
else
	@if [ -z "$$(docker images -q $(IMAGE_NAME))" ]; then \
		echo "Image $(IMAGE_NAME) not found locally, pulling..."; \
		docker pull $(IMAGE_NAME); \
	fi; \
	$(DOCKER_RUN) sh -c "$(DOCKER_CMD)"
endif

#######################################
# clean up
#######################################
clean:
ifeq ($(OS),Windows_NT)
	@if exist "$(OUT_DIR)" rmdir /s /q $(OUT_DIR)
	@if exist "$(BUILD_DIR)" rmdir /s /q $(BUILD_DIR)
	@$(foreach asmfile,$(ASM_BASENAMES),del /Q /F "$(asmfile).s" &)
else
	@rm -rf $(OUT_DIR)
	@rm -rf $(BUILD_DIR)
	@$(foreach asmfile,$(ASM_BASENAMES),rm -f "$(asmfile).s";)
endif

#######################################
# install
#######################################
install:
	git submodule update --init --recursive
ifeq ($(OS),Windows_NT)
	powershell -Command "Start-Process PowerShell -ArgumentList '-NoProfile -ExecutionPolicy Bypass -Command \"python -m pip install -U pyocd ; python -m pip install -U libusb ; pyocd pack install M032BTAIAAN\"' -Verb RunAs"
else ifeq ($(shell uname -s),Linux)
	@echo "\033[34mIf you are running in WSL, please refer to the following URL:\033[0m"
	@echo "\033[32mhttps://learn.microsoft.com/zh-tw/windows/wsl/connect-usb\033[0m"
	@if ! python -m pip list | grep pyocd; then \
		rm -rf ~/pyOCD && \
		git clone https://github.com/pyocd/pyOCD.git ~/pyOCD && \
		cd ~/pyOCD && \
		git checkout v0.36.0 && \
		python -m pip install . --user && \
		cd -; \
	fi
	@echo "SUBSYSTEM==\"usb\", ATTR{idVendor}==\"0416\", ATTR{idProduct}==\"2004\", MODE:=\"666\"" > ./99-pyocd.rules
	@sudo mv ./*.rules /etc/udev/rules.d
	@sudo udevadm control --reload
	@sudo udevadm trigger
	@pyocd pack install M032BTAIAAN
endif

#######################################
# flash
#######################################
flash:
	pyocd flash --erase chip --target M032BTAIAAN ./$(BUILD_DIR)/$(TARGET).bin --base-address 0x00000000

#######################################
# rebuild
#######################################
rebuild: clean all

#######################################
# upgrade
#######################################
upgrade:
	make clean
	make all
	make flash

#######################################
# documentation
#######################################
docs:
	pandoc -f markdown -t gfm -o README.md README_.md
	pandoc -f markdown -t pdf -o README.pdf README_.md --pdf-engine=xelatex --toc-depth=3 --number-sections --template=eisvogel.latex --listings --variable date=$(DATE) --lua-filter=diagram.lua

docs-install:
	@tlmgr install adjustbox babel-german background bidi collectbox csquotes everypage filehook footmisc footnotebackref framed fvextra letltxmacro ly1 mdframed mweights needspace pagecolor sourcecodepro sourcesanspro titling ucharcat ulem unicode-math upquote xecjk xurl zref
ifeq ($(OS),Windows_NT)
	@echo "Fetching the latest Eisvogel release URL..."
	@powershell -Command "$$LATEST_RELEASE_URL = $$(Invoke-RestMethod -Uri 'https://api.github.com/repos/Wandmalfarbe/pandoc-latex-template/releases/latest' | Select-Object -ExpandProperty assets | Where-Object { $$_.name -like '*eisvogel*.zip' } | Select-Object -First 1 -ExpandProperty browser_download_url); \
	if ([string]::IsNullOrEmpty($$LATEST_RELEASE_URL)) { \
		Write-Error 'Error: Unable to retrieve the release URL.'; \
		exit 1; \
	}; \
	Write-Host 'Downloading Eisvogel template...'; \
	Invoke-WebRequest -Uri $$LATEST_RELEASE_URL -OutFile 'Eisvogel.zip'; \
	Write-Host 'Unzipping the template...'; \
	Expand-Archive -Path 'Eisvogel.zip' -DestinationPath $$env:USERPROFILE'\AppData\Roaming\pandoc\templates' -Force; \
	Remove-Item 'Eisvogel.zip';"
	@echo "Fetching the latest diagram.lua release URL..."
	@powershell -Command "$$filtersPath = \"$$(Join-Path $$env:USERPROFILE 'AppData\Roaming\pandoc\filters')\"; \
	if (!(Test-Path -Path $$filtersPath)) { \
		New-Item -ItemType Directory -Path $$filtersPath; \
	} \
	Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/pandoc-ext/diagram/main/_extensions/diagram/diagram.lua' -OutFile \"$$(Join-Path $$filtersPath 'diagram.lua')\";"
else
	# Unix-like commands (macOS and Linux)
	@echo "Fetching the latest Eisvogel release URL..."
	@LATEST_RELEASE_URL=$$(curl -s https://api.github.com/repos/Wandmalfarbe/pandoc-latex-template/releases/latest | jq -r '.assets[] | select(.name | endswith(".zip")) | .browser_download_url' | head -n 1); \
	if [ -z "$$LATEST_RELEASE_URL" ]; then \
		echo "Error: Unable to retrieve the release URL."; \
		exit 1; \
	fi; \
	echo "Downloading Eisvogel template..."; \
	curl -L -o Eisvogel.zip "$$LATEST_RELEASE_URL"; \
	echo "Unzipping the template to $$HOME/.pandoc/templates..."; \
	mkdir -p $$HOME/.pandoc/templates; \
	unzip Eisvogel.zip -d $$HOME/.pandoc/templates; \
	rm Eisvogel.zip
	@echo "Fetching the latest diagram.lua release URL..."
	mkdir -p ~/.pandoc/filters
	curl -L https://raw.githubusercontent.com/pandoc-ext/diagram/main/_extensions/diagram/diagram.lua -o $$HOME/.pandoc/filters/diagram.lua
endif

#######################################
# help
#######################################
help:
	@echo "make              - build the application"
	@echo "make clean        - remove all build files"
	@echo "make install      - install the submodules"
	@echo "make flash        - flash the application"
	@echo "make rebuild      - remove all build files and build the application"
	@echo "make upgrade      - update the submodules, remove all build files, build the application, and flash the application"
	@echo "make docs         - generate the documentation"
	@echo "make docs-install - install the Eisvogel template and diagram.lua filter"

#######################################
# dependencies
#######################################
-include $(wildcard $(OUT_DIR)/*.d)

.PHONY: all clean install flash rebuild upgrade docs docs-install docker help

# *** EOF ***
