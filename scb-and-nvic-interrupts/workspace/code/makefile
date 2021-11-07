PROGRAM = program
BUILD_DIR = build
DEBUG_OPTIONS = 1

SOURCE_C_LIST = \
src/main.c \
src/handlers.c \
src/systick.c

SOURCE_S_LIST = \
src/startup_code.s

INCLUDE_OPTION = \
-Iinc

OBJECT_LIST = 
OBJECT_LIST += $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCE_C_LIST:.c=.o)))
OBJECT_LIST += $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCE_S_LIST:.s=.o)))


vpath %.s $(sort $(dir $(SOURCE_S_LIST)))
vpath %.c $(sort $(dir $(SOURCE_C_LIST)))


HX = arm-none-eabi-objcopy.exe -O ihex
LD = arm-none-eabi-ld.exe
LINKER_SCRIPT = -T linker_script.ld

CC = arm-none-eabi-gcc.exe -c
AS = arm-none-eabi-gcc.exe -c -x assembler

MCU = -mcpu=cortex-m3 -mthumb
DEBUG_FLAGS = -g -gdwarf-2 -fdebug-prefix-map==../

CC_FLAGS = 
CC_FLAGS += $(MCU)
ifeq ($(DEBUG_OPTIONS),1)
CC_FLAGS += $(DEBUG_FLAGS)
endif
CC_FLAGS += -MMD -MP
CC_FLAGS += $(INCLUDE_OPTION)
CC_FLAGS += -mlong-calls


AS_FLAGS = 
AS_FLAGS += $(MCU)
ifeq ($(DEBUG_OPTIONS),1)
AS_FLAGS += $(DEBUG_FLAGS)
endif


all : $(BUILD_DIR)/$(PROGRAM).hex $(BUILD_DIR)/$(PROGRAM).elf

$(BUILD_DIR)/$(PROGRAM).hex : $(BUILD_DIR)/$(PROGRAM).elf
	$(HX) $(<) $(@)

$(BUILD_DIR)/$(PROGRAM).elf : $(OBJECT_LIST)
	$(LD) $(LINKER_SCRIPT) -o $(@) $(OBJECT_LIST)

$(BUILD_DIR)/%.o : %.c makefile | $(BUILD_DIR)
	$(CC) $(CC_FLAGS) -o $(@) $(<)
	
$(BUILD_DIR)/%.o : %.s makefile | $(BUILD_DIR)
	$(AS) $(AS_FLAGS) -o $(@) $(<)
		
$(BUILD_DIR): 
	mkdir $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
	
	
-include $(wildcard $(BUILD_DIR)/*.d)