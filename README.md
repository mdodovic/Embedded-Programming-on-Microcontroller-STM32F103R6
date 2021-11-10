# Embedded-Programming-on-Microcontroller-STM32F103R6

Microcontroller STM32F103R6

## Structure of the repository
### ./timer-and-interrupts
This project shows basics configuration, that means the initialization of the microcontroller by writing reset handler. Reset handler has the only function to restore the content of the .data section from the LMA (Load Memory Address) region and place it to the VMA (Virtual Memory Address) region. After that the configuration of system timer is shown, and the full configuration of 1s tick will generate the interrupt request.

### ./scb-and-nvic-interrupts

