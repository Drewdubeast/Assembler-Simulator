//
//  Simulator.c
//  Assembler-Simulator-XP
//
//  Created by Drew Wilken on 3/11/18.
//  Copyright © 2018 Drew Wilken. All rights reserved.
//

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define NUMMEMORY 65535
#define NUMREGS 8

//struct for machine state
typedef struct state_struct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int num_memory;
} statetype;

//our machine state
statetype state;

//prototypes
int countLines(FILE *f);
int convert_num(int num);
void print_state(statetype *stateptr);

//main
int main(int argc, char **argv) {
    
    //indices
    int i=0;
    
    //allocate a line as 10 characters at max
    char line[15];
    
    //run-mode
    bool fileout = false;
    
    //instruction count
    int instr_count = 0;
    
    //File handling
    FILE *file = NULL;
    FILE *out = NULL;
    
    //argc = 2;
    //argv[1] = "run.mc";
    
    //file handling
    if (argc == 1) {
        printf("No file input!\n");
        return 0;
    }
    else if (argc == 2) {
        fileout = false;
        if (!(file = fopen(argv[1], "r"))) {
            printf("Couldn't find the file: %s\n", argv[1]);
            return 0;
        }
    }
    else if (argc == 3) {
        fileout = true;
        if (!(file = fopen(argv[1], "r"))) {
            printf("Couldn't find the file: %s\n", argv[1]);
            return 0;
        }
        if (!(out = fopen(argv[2], "w+"))) {
            printf("Couldn't find the file: %s\n", argv[2]);
            return 0;
        }
    }
    else {
        printf("Too many input arguments! Must have a maximum of two.\nEnter one file name argument to print output to stdout\nEnter two file names to output to second file.\n");
        return 0;
    }
    
    //file lines
    int flines = countLines(file);
    
    /*
     * FIRST PASS: Go through the file and grab each instruction and put into 'prgrm' array
     */
    for(i=0;fgets(line, sizeof(line), file) != NULL;i++) {
        state.mem[i] = atoi(line);
        state.num_memory++;
        printf("%d\n", state.mem[i]);
    }
    
    /*
     * SECOND PASS: Break down each instruction into opcode, registers, and immediates
     */
    
    int32_t opcode;
    int32_t r0, r1, r_dst;
    int32_t offset;
    
    while(state.pc<state.num_memory) {
        opcode = ((7<<22)&(state.mem[state.pc]))>>22;
        print_state(&state);
        
        printf("opcode: %d\n",opcode);
        
        switch(opcode) {
                /*
                 * ADD
                 */
            case 0: //ADD
                
                //find the next three registers and do the operation
                r0 = ((7<<19)&(state.mem[state.pc]))>>19;
                r1 = ((7<<16)&(state.mem[state.pc]))>>16;
                r_dst = 7&state.mem[state.pc];
                
                printf("%i %i %i %i\n", opcode, r0, r1, r_dst);
                
                //operation
                state.reg[r_dst] = state.reg[r0] + state.reg[r1];
                state.pc++;
                break;
                
                /*
                 * NAND
                 */
            case 1: //nand
                //find the next three registers and do the operation
                r0 = ((7<<19)&(state.mem[state.pc]))>>19;
                r1 = ((7<<16)&(state.mem[state.pc]))>>16;
                r_dst = 7&state.mem[state.pc];
                
                printf("%i %i %i %i\n", opcode, r0, r1, r_dst);
                
                // printf("%d nand %d: %d\n", state.reg[r0], state.reg[r1], )
                
                state.reg[r_dst] = ~(state.reg[r0]&state.reg[r1]);
                state.pc++;
                break;
                
                /*
                 * LW
                 */
            case 2: //lw
                
                r0 = ((7<<19)&(state.mem[state.pc]))>>19;
                r1 = ((7<<16)&(state.mem[state.pc]))>>16;
                offset = convert_num(65535&state.mem[state.pc]);
                
                printf("%i %i %i %i\n", opcode, r0, r1, offset);
                
                state.reg[r0] = state.mem[state.reg[r1] + offset];
                state.pc++;
                break;
                
                /*
                 * SW
                 */
            case 3: //sw
                
                r0 = ((7<<19)&(state.mem[state.pc]))>>19;
                r1 = ((7<<16)&(state.mem[state.pc]))>>16;
                offset = convert_num(65535&state.mem[state.pc]);
                
                printf("%i %i %i %i\n", opcode, r0, r1, offset);
                
                state.mem[state.reg[r1] + offset] = state.reg[r0];
                state.pc++;
                break;
                
                
            case 4: //beq
                
                r0 = ((7<<19)&(state.mem[state.pc]))>>19;
                r1 = ((7<<16)&(state.mem[state.pc]))>>16;
                offset = convert_num(65535&state.mem[state.pc]);
                
                printf("%i %i %i %i\n", opcode, r0, r1, offset);
                
                if(state.reg[r0] == state.reg[r1]) {
                    state.pc = (++state.pc)+ offset;
                }
                else
                {
                    state.pc++;
                }
                printf("BEQ OFFSET %d\n", offset);
                break;
                
            case 5: //jalr
                //nothing
                //I guess we just increment PC? We're not executing anything though....
                //state.pc++;
                break;
                
            case 6: //halt
                exit(0);
                
                break;
            case 7: //noop
                //don't do anything for a cycle
                //but we should update pc, otherwise it'll freeze.
                state.pc++;
                break;
                
            default: //default
                printf("Incorrect opcode in line %d\n", i+1);
                break;
        }
        instr_count++;
        printf("Instruction count: %d\n", instr_count);
        //Doing this here may very well lead to confusion. I've changed it to run in every case...
        //state.pc++;
    }
    
    //last state
    print_state(&state);
    printf("Instruction count: %d\n", instr_count);
    fclose(file);
    return 0;
}

int countLines(FILE* f) {
    int lines = 0;
    int c;
    
    if (f == NULL) {
        return 0;
    }
    while((c = fgetc(f)) != EOF) {
        if (c == '\n') {
            lines++;
        }
    }
    fseek(f, 0, SEEK_SET); //seek back to the beginning of the file
    //fclose(f);
    return lines;
}
int convert_num(int num) {
    if (num&(1<<15)) {
        num -= (1<<16);
    }
    return(num);
}
void print_state(statetype *stateptr){
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", stateptr->pc);
    printf("\tmemory:\n");
    for(i = 0; i < stateptr->num_memory; i++){
        printf("\t\tmem[%d]=%d\n", i, stateptr->mem[i]);
    }
    printf("\tregisters:\n");
    for(i = 0; i < NUMREGS; i++){
        printf("\t\treg[%d]=%d\n", i, stateptr->reg[i]);
    }
    printf("end state\n");
}

