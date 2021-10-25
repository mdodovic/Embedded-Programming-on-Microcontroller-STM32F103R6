PROGRAM = program
BUILD_DIR = build

SOURCE_S_LIST = \
startup_code.s 

SOURCE_C_LIST = \
main.c \
systick.c 

OBJECTS_LIST = 
OBJECTS_LIST += $(addprefix $(BUILD_DIR)/, $(SOURCE_S_LIST:.s=.o))
OBJECTS_LIST += $(addprefix $(BUILD_DIR)/, $(SOURCE_C_LIST:.c=.o))

CC = arm-none-eabi-gcc.exe -c
AS = $(CC) -x assembler
LD = arm-none-eabi-ld.exe
HX = arm-none-eabi-objcopy.exe -O ihex

MCU = -mcpu=cortex-m3 -mthumb

FLAGS_AS = 
FLAGS_AS += $(MCU)

FLAGS_CC = 
FLAGS_CC += $(MCU)
FLAGS_CC += -MMD -MP 

LINKER_SCRIPT = linker_script.ld

all : $(BUILD_DIR)/$(PROGRAM).hex $(BUILD_DIR)/$(PROGRAM).elf

$(BUILD_DIR)/$(PROGRAM).hex : $(BUILD_DIR)/$(PROGRAM).elf
	$(HX) $(<) $(@)
	
$(BUILD_DIR)/$(PROGRAM).elf : $(OBJECTS_LIST) makefile
	$(LD) -T $(LINKER_SCRIPT) -o $(@) $(OBJECTS_LIST)
	
$(BUILD_DIR)/%.o : %.s makefile | $(BUILD_DIR)
	$(AS)  $(FLAGS_AS) -o $(@) $(<)

$(BUILD_DIR)/%.o : %.c makefile | $(BUILD_DIR)
	$(CC) $(FLAGS_CC) -o $(@) $(<)
	
	
$(BUILD_DIR) : 
	mkdir $(@)
	
clean:
	rm -rf $(BUILD_DIR)
	
-include $(wildcard $(BUILD_DIR)/*.d)
