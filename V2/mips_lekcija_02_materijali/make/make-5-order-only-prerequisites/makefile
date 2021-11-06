PROGRAM = program
BUILD_DIR = build

SOURCE_S_LIST = \
startup_code.s

SOURCE_C_LIST = \
main.c \
systick.c

OBJECTS_LIST = 
# GNU MAKE (8.3 Functions for File Names)
# https://www.gnu.org/software/make/manual/html_node/File-Name-Functions.html
# GNU MAKE (6.3.1 Substitution References)
# https://www.gnu.org/software/make/manual/html_node/Substitution-Refs.html
OBJECTS_LIST += $(addprefix $(BUILD_DIR)/, $(SOURCE_S_LIST:.s=.o))
OBJECTS_LIST += $(addprefix $(BUILD_DIR)/, $(SOURCE_C_LIST:.c=.o))

CC = arm-none-eabi-gcc.exe -c
AS = arm-none-eabi-gcc.exe -c -x assembler
LD = arm-none-eabi-ld.exe
HX = arm-none-eabi-objcopy.exe --output-target ihex

MCU = -mcpu=cortex-m3 -mthumb

FLAGS_AS =
FLAGS_AS += $(MCU)

FLAGS_CC =
FLAGS_CC += $(MCU)
# GNU GCC (3.13 Options Controlling the Preprocessor)
# https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html
FLAGS_CC += -MMD -MP

LINKER_SCRIPT = linker_script.ld

all : $(BUILD_DIR)/$(PROGRAM).elf $(BUILD_DIR)/$(PROGRAM).hex

# GNU MAKE (6 How to Use Variables)
# https://www.gnu.org/software/make/manual/html_node/Using-Variables.html
# GNU MAKE (10.5.3 Automatic Variables)
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
$(BUILD_DIR)/$(PROGRAM).hex : $(BUILD_DIR)/$(PROGRAM).elf
	$(HX) $(<) $(@)

$(BUILD_DIR)/$(PROGRAM).elf : $(OBJECTS_LIST) makefile
	$(LD) -T $(LINKER_SCRIPT) -o $(@) $(OBJECTS_LIST)

# GNU MAKE (4.3 Types of Prerequisites)
# https://www.gnu.org/software/make/manual/html_node/Prerequisite-Types.html
# GNU MAKE (10.5 Defining and Redefining Pattern Rules)
# https://www.gnu.org/software/make/manual/html_node/Pattern-Rules.html
$(BUILD_DIR)/%.o : %.s makefile | $(BUILD_DIR)
	$(AS) $(FLAGS_AS) -o $(@) $(<)

$(BUILD_DIR)/%.o : %.c makefile | $(BUILD_DIR)
	$(CC) $(FLAGS_CC) -o $(@) $(<)

$(BUILD_DIR) :
	mkdir $(@)
	
clean :
	rm -rf $(BUILD_DIR)

# GNU MAKE (4.4.3 The Function wildcard)
# https://www.gnu.org/software/make/manual/html_node/Wildcard-Function.html
# GNU MAKE (3.3 Including Other Makefiles)
# https://www.gnu.org/software/make/manual/html_node/Include.html
-include $(wildcard $(BUILD_DIR)/*.d)
