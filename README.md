# UST-3400 ISA Assembler and Simulator
An assembler and single-cycle simulator written in C for the UST-3400 ISA. Written by Nathan Taylor and Drew Wilken.

## Instruction Set ##
Instruction  | Purpose
------------- | -------------
`lw` - load word | Loads a word sized value into register from memory
`sw` - store word | Store a word sized value into memory
`add` - add | Adds the content of two registers into the destination register
 `nand` - nand | NANDs the contents of two registers and stores into the destination register
 `jalr ` - jump and link register | Stores `PC+1` into the first register and jumps to address of second register
 `halt` - halt | Ends execution
 `noop` - no operation | burns a cycle
 
 ### Instruction Types ###
 * I-Type
 * R-Type
 * O-Type
 * J-Type
 
 ### Instruction Formatting ###
 
 Instruction Type | Format
 -----------------|--------------
 I-Type | `opcode regA regB offset/label`
 R-Type | `opcode reg_dest regA regB`
 O-Type | `opcode`
 J-Type | `opcode regA regB`
 
 **For example:**
 * Load Word: `lw 1 0 five`: loads a memory address 'five' into register 1 with offset of register 0.
 * Store Word: `sw 1 0 five`: stores the contents of register 1 into the memory location for 'five'
 * Add: `add 1 2 3`: adds the contents of registers 2 and 3 and stores the result into register 1
 
 ## To Use ##
 
 1. Download/clone the project
 2. Navigate into the main project directory, and create your new assembly code file
 3. To assemble it, in the command line, `./Assembler.c [your file name]` to have it assemble to `stdout`. 
 4. To assemble it and store in a file, give two arguments, such as `./Assembler.c [your source code file name] [output file name]`
 5. To simulate the machine code, `./simulator.c [your output file from assembler` 
