PROGRAM = program
BUILD_DIR = build
SOURCE_DIR = source
INCLUDE_DIR = include
DEBUG_ENABLED = 1

SOURCE_S_LIST = \
$(SOURCE_DIR)/startup_code.s 

SOURCE_C_LIST = \
$(SOURCE_DIR)/main.c \
$(SOURCE_DIR)/systick.c 

OBJECTS_LIST = 
OBJECTS_LIST += $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCE_S_LIST:.s=.o)))
OBJECTS_LIST += $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCE_C_LIST:.c=.o)))

vpath %.c $(sort $(dir $(SOURCE_C_LIST)))
vpath %.s $(sort $(dir $(SOURCE_S_LIST)))

CC = arm-none-eabi-gcc.exe -c
AS = $(CC) -x assembler
LD = arm-none-eabi-ld.exe
HX = arm-none-eabi-objcopy.exe -O ihex

MCU = -mcpu=cortex-m3 -mthumb

FLAGS_DEBUG = -g -gdwarf-2 -fdebug-prefix-map==../

FLAGS_INCLUDE = \
-I$(INCLUDE_DIR)

FLAGS_AS = 
FLAGS_AS += $(MCU)
ifeq ($(DEBUG_ENABLED), 1)
FLAGS_AS += $(FLAGS_DEBUG)
endif

FLAGS_CC = 
FLAGS_CC += $(MCU)
ifeq ($(DEBUG_ENABLED), 1)
FLAGS_CC += $(FLAGS_DEBUG)
endif
FLAGS_CC += -MMD -MP 
FLAGS_CC += $(FLAGS_INCLUDE)


LINKER_SCRIPT = linker_script.ld

all : $(BUILD_DIR)/$(PROGRAM).hex $(BUILD_DIR)/$(PROGRAM).elf

$(BUILD_DIR)/$(PROGRAM).hex : $(BUILD_DIR)/$(PROGRAM).elf
	$(HX) $(<) $(@)
	
$(BUILD_DIR)/$(PROGRAM).elf : $(OBJECTS_LIST) makefile
	$(LD) -T $(LINKER_SCRIPT) -o $(@) $(OBJECTS_LIST)
	
$(BUILD_DIR)/%.o : %.s makefile | $(BUILD_DIR)
	$(AS) $(FLAGS_AS) -o $(@) $(<)

$(BUILD_DIR)/%.o : %.c makefile | $(BUILD_DIR)
	$(CC) $(FLAGS_CC) -o $(@) $(<)
	
	
$(BUILD_DIR) : 
	mkdir $(@)
	
clean:
	rm -rf $(BUILD_DIR)
	
-include $(wildcard $(BUILD_DIR)/*.d)
