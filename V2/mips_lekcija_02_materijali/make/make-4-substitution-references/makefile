PROGRAM = program

SOURCE_S_LIST = \
startup_code.s

SOURCE_C_LIST = \
main.c \
systick.c

OBJECTS_LIST = 
# GNU MAKE (6.3.1 Substitution References)
# https://www.gnu.org/software/make/manual/html_node/Substitution-Refs.html
OBJECTS_LIST += $(SOURCE_S_LIST:.s=.o)
OBJECTS_LIST += $(SOURCE_C_LIST:.c=.o)

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

all : $(PROGRAM).elf $(PROGRAM).hex

# GNU MAKE (6 How to Use Variables)
# https://www.gnu.org/software/make/manual/html_node/Using-Variables.html
# GNU MAKE (10.5.3 Automatic Variables)
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
$(PROGRAM).hex : $(PROGRAM).elf
	$(HX) $(<) $(@)

$(PROGRAM).elf : $(OBJECTS_LIST) makefile
	$(LD) -T $(LINKER_SCRIPT) -o $(@) $(OBJECTS_LIST)

# GNU MAKE (10.5 Defining and Redefining Pattern Rules)
# https://www.gnu.org/software/make/manual/html_node/Pattern-Rules.html
%.o : %.s makefile
	$(AS) $(FLAGS_AS) -o $(@) $(<)

%.o : %.c makefile
	$(CC) $(FLAGS_CC) -o $(@) $(<)

# GNU MAKE (4.4.3 The Function wildcard)
# https://www.gnu.org/software/make/manual/html_node/Wildcard-Function.html
# GNU MAKE (3.3 Including Other Makefiles)
# https://www.gnu.org/software/make/manual/html_node/Include.html	
-include $(wildcard ./*.d)
