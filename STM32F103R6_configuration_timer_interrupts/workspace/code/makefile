PROGRAM = program
BUILD_DIR = build
DEBUG_OPTIONS = 1


SOURCE_C_LIST = \
src/handlers.c \
src/main.c \
src/systick.c

SOURCE_S_LIST = \
src/startup_code.s

INCLUDE_COMMAND = \
-Iinc


OBJECT_FILE_LIST = 
OBJECT_FILE_LIST += $(addprefix $(BUILD_DIR)/, $(notdir $(SORCE_C_LIST:.c:.o)))
OBJECT_FILE_LIST += $(addprefix $(BUILD_DIR)/, $(notdir $(SORCE_S_LIST:.c:.o))) 

vpath %.c $(sort $(dir $(SORCE_C_LIST)))
vpath %.s $(sort $(dir $(SORCE_S_LIST)))

HX = arm-none-eabi-objcopy.exe
LD = arm-none-eabi-ld.exe
LINKER_SCRIPT = linker_script.ld
AS = arm-none-eabi-gcc.exe -c -x assembler
CC = arm-none-eabi-gcc.exe -c

MCU = -cpu=cortex-m3 -mthumb

DEBUG_FLAGS = -g -gdwarf-2 -fdebug-prefix-map==../

AS_FLAGS = $(MCU)
ifeq ($(DEBUG_OPTIONS), 1)
AS_FLAGS += $(DEBUG_FLAGS)  
endif

CC_FLAGS = $(MCU)
ifeq ($(DEBUG_OPTIONS), 1)
CC_FLAGS += $(DEBUG_FLAGS)  
endif
CC_FLAGS += -MMD -MP
CC_FLAGS += -call-mlong

all: $(BUILD_DIR)/$(PROGRAM).elf $(BUILD_DIR)/$(PROGRAM).hex

$(BUILD_DIR)/$(PROGRAM).hex: $(BUILD_DIR)/$(PROGRAM).elf
	$(HX) $(BUILD_DIR)/$(PROGRAM).elf $(BUILD_DIR)/$(PROGRAM).hex

$(BUILD_DIR)/$(PROGRAM).elf: $(OBJECT_FILE_LIST) makefile | $(BUILD_DIR)
	$(LD) -T $(LINKER_SCRIPT) -o $(@) $(OBJECT_FILE_LIST) 

$(BUILD_DIR)/%.o : %.c makefile | $(BUILD_DIR)
	$(CC) $(CC_FLAGS) -o $(@) $(<)

$(BUILD_DIR)/%.o : %.s makefile | $(BUILD_DIR)
	$(AS) $(AS_FLAGS) -o $(@) $(<)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clear:
	rm -fr $(BUILD_DIR)
	
	
-include $(wildcard $(BUILD_DIR)/*.d)