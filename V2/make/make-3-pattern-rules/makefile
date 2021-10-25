PROGRAM = program

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

all : $(PROGRAM).hex $(PROGRAM).elf

program.hex : $(PROGRAM).elf
	$(HX) $(<) $(@)
	
$(PROGRAM).elf : main.o startup_code.o systick.o
	$(LD) -T $(LINKER_SCRIPT) -o $(@) $(^)
	
%.o : %.s
	$(AS)  $(FLAGS_AS) -o $(@) $(<)

%.o : %.c
	$(CC) $(FLAGS_CC) -o $(@) $(<)
	
	
-include $(wildcard ./*.d)
