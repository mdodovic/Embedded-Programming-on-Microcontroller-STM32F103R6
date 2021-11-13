PROGRAM = program
BUILD_DIR = build
INCLUDE_DEBUG_FLAGS = 1

SOURCE_C_LIST = \
src/handlers.c \
src/main.c


SOURCE_S_LIST = \
src/startup_code.s

INCLUDE_H_FLAG = \
-Iinc


OBJECT_LIST =
OBJECT_LIST += $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCE_C_LIST:.c=.o)))
OBJECT_LIST += $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCE_S_LIST:.s=.o)))

vpath %.c $(sort $(dir $(SOURCE_C_LIST)))
vpath %.s $(sort $(dir $(SOURCE_S_LIST)))


HX = arm-none-eabi-objcopy.exe --output-target ihex
LINKER_SCRIPT = -T linker_script.ld
LD = arm-none-eabi-ld.exe

DEBUG_FLAGS = -g -gdwarf-2 -fdebug-prefix-map==../
MCU = -mcpu=cortex-m3 -mthumb

CC_FLAGS = $(MCU)
ifeq ($(INCLUDE_DEBUG_FLAGS),1)
	CC_FLAGS += $(DEBUG_FLAGS)
endif
CC_FLAGS += -MMD -MP
CC_FLAGS += $(INCLUDE_H_FLAG)
CC_FLAGS += -mlong-calls

AS_FLAGS = $(MCU)
ifeq ($(INCLUDE_DEBUG_FLAGS),1)
	AS_FLAGS += $(DEBUG_FLAGS)
endif

CC = arm-none-eabi-gcc.exe -c
AS = arm-none-eabi-gcc.exe -c -x assembler


all: $(BUILD_DIR)/$(PROGRAM).hex $(BUILD_DIR)/$(PROGRAM).elf
	

$(BUILD_DIR)/$(PROGRAM).hex: $(BUILD_DIR)/$(PROGRAM).elf
	$(HX) $(<) $(@)
	
$(BUILD_DIR)/$(PROGRAM).elf: $(OBJECT_LIST)
	$(LD) $(LINKER_SCRIPT) -o $(@) $(OBJECT_LIST)

$(BUILD_DIR)/%.o: %.c makefile | $(BUILD_DIR)
	$(CC) $(CC_FLAGS) -o $(@) $(<)

$(BUILD_DIR)/%.o: %.s makefile | $(BUILD_DIR)
	$(AS) $(AS_FLAGS) -o $(@) $(<)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)
		
clean: 
	rm -rf $(BUILD_DIR)	
	
-include $(wildcard $(BUILD_DIR)/*.d)	