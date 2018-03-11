//
//  main.c
//  Assembler-Simulator-XP
//
//  Created by Drew Wilken on 3/1/18.
//  Copyright © 2018 Drew Wilken and Nathan Taylor. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

/*
 * Function Prototypes
 */
char **split(char s[]);
int countTokens(char* s);
int countLines(FILE* f);
int isop(char* s);
int isDigit(char* s);
int isRegister(char* s);
int opcodepos(char** line, int len);
void printBits(uint32_t pack);

/*
 * MAIN FUNCTION
 */
int main() {
    
    //indices
    int i=0;
    int j=0;
    int k=0;
    
    //printf("isDigit: -9: %i\n", isDigit("-9 "));
    //File handling
    FILE *file;
    if ((file = fopen("run.a", "r"))) {}
    else {
        printf("Couldn't find the file\n");
        return 0;
    }
    //[31 UNUSED 25][24 OPCODE 22][21 rA 19][18 rB 16][15 Offset 0]
    //counts
    int lcount = 0;
    
    //current line in the input
    char line[200];
    
    //num lines in the file
    int flines = countLines(file);
    
    //label arrays
    char** labelArray = malloc(flines * sizeof(char*));
    
    //printf("isop of sw: %d\n", isop("sw0"));
    
    //Token count array for each instruction
    int tokensPerLine[flines];
    
    //our entire program array and allocate memory for the number of lines in the file
    char*** prgrm = malloc(flines * sizeof(char**));
    
    /*
     * Going through the file:
     * Take lines in and add the instructions to the program array
     */
    i=0;
    while(fgets(line, 255,  file)) {
        //TODO: check for white space first or label, if neither, then return
        lcount++;
        tokensPerLine[i] = countTokens(line);
        line[strlen(line)-1] = '\0';
        prgrm[i] = split(line);
        i++;
    }
    /*
     * Next: FIRST PASS to replace labels with offsets
     * Second pass to actually pack the bits and check for formatting
     */
    i=0;
    j=0;
    k=0;
    
    /*
     * FIRST PASS
     */
    for(i=0;i<lcount;i++) {
        char* firstToken = prgrm[i][0];
        if((isop(firstToken) == 0) && (isDigit(firstToken) == 0)) { //if it's a label
            labelArray[i] = strdup(firstToken); //add label to label array
        }
        else {
            labelArray[i] = "";
            putchar('\n');
        }
    }
    /*
     * SECOND PASS
     */
    for(i=0;i<lcount;i++) {
        uint32_t opcode = -1;
        uint32_t r0 = 0;
        uint32_t r1 = 0;
        uint32_t r_dst = 0;
        uint16_t offset = 0;
        uint32_t instruction = 0;
        char type = ' ';
        
        //go through first two tokens in each line and see where/if there's an opcode
        if(isop(prgrm[i][0]) == 1 || isop(prgrm[i][1]) == 1)  { //correct if
            int opcpos = opcodepos(prgrm[i], tokensPerLine[i]);
            
            
            /*
             * I-TYPE INSTRUCTIONS
             */
            if(strcmp(prgrm[i][opcpos],"lw") == 0) {
                
                type = 'I';
                opcode = 2;
                
                //next two fields must be register numbers
                if(isRegister(prgrm[i][opcpos+1]) == 1 && isRegister(prgrm[i][opcpos+2]) == 1) {
                    //set register ints
                    r0 = atoi(prgrm[i][opcpos+1]);
                    r1 = atoi(prgrm[i][opcpos+2]);
                }
                else {
                    printf("Illegal registers for I-Type instruction in line: %i\n\n\nRequired: [opcode] [int] [int] [int]\n", i);
                    return 0;
                }
                
                //if label position is a digit
                if(isDigit(prgrm[i][opcpos+3]) == 1) {//
                    int offset_temp = atoi(prgrm[i][opcpos+3]);
                    if(offset_temp>65535) {
                        printf("Offset value on line %i is incorrect!\n\nFound: %i\nRequired: A value less than 65535\n", i, offset_temp);
                        return 0;
                    }
                    offset = offset_temp;
                }
                //if label position isn't an opcode and isn't a digit
                else if (isop(prgrm[i][opcpos+3]) == 0) {
                    for(j=0;j<flines;j++) {
                        //go through label array and see if we can find a match, take that index
                        if(strcmp(prgrm[i][opcpos+3],labelArray[j]) == 0) {
                            offset = j-(i+1);
                        }
                    }
                    if(offset == 0) {
                        printf("Can't find destination label: %s\n", prgrm[i][opcpos+3]);
                        return 0;
                    }
                }
                else {
                    printf("Error in line: %i\n", i);
                    return 0;
                }
            }
            else if(strcmp(prgrm[i][opcpos],"sw") == 0) {
                
                type = 'I';
                opcode = 3;
                
                //next two fields must be register numbers
                if(isRegister(prgrm[i][opcpos+1]) == 1 && isRegister(prgrm[i][opcpos+2]) == 1) {
                    //set register ints
                    r0 = atoi(prgrm[i][opcpos+1]);
                    r1 = atoi(prgrm[i][opcpos+2]);
                }
                else {
                    printf("Illegal registers for I-Type instruction in line: %i\n\n\nRequired: [opcode] [int] [int] [int]\n", i);
                    return 0;
                }
                
                //if label position is a digit
                if(isDigit(prgrm[i][opcpos+3]) == 1) {//
                    int offset_temp = atoi(prgrm[i][opcpos+3]);
                    if(offset_temp>65535) {
                        printf("Offset value on line %i is incorrect!\n\nFound: %i\nRequired: A value less than 65535\n", i, offset_temp);
                        return 0;
                    }
                    offset = offset_temp;
                }
                //if label position isn't an opcode and isn't a digit
                else if (isop(prgrm[i][opcpos+3]) == 0) {
                    for(j=0;j<flines;j++) {
                        //go through label array and see if we can find a match, take that index
                        if(strcmp(prgrm[i][opcpos+3],labelArray[j]) == 0) {
                            offset = j-(i+1);
                            printBits(offset);
                        }
                    }
                    if(offset == 0) {
                        printf("Can't find destination label: %s\n", prgrm[i][opcpos+3]);
                        return 0;
                    }
                }
                else {
                    printf("Error in line: %i\n", i);
                    return 0;
                }
            }
            else if(strcmp(prgrm[i][opcpos],"beq") == 0) {
                
                type = 'I';
                opcode = 6;
                
                //next two fields must be register numbers
                if(isRegister(prgrm[i][opcpos+1]) == 1 && isRegister(prgrm[i][opcpos+2]) == 1) {
                    //set register ints
                    r0 = atoi(prgrm[i][opcpos+1]);
                    r1 = atoi(prgrm[i][opcpos+2]);
                }
                else {
                    printf("Illegal registers for I-Type instruction in line: %i\n\n\nRequired: [opcode] [int] [int] [int]\n", i);
                    return 0;
                }
                
                //if label position is a digit
                if(isDigit(prgrm[i][opcpos+3]) == 1) {//
                    int offset_temp = atoi(prgrm[i][opcpos+3]);
                    if(offset_temp>65535) {
                        printf("Offset value on line %i is incorrect!\n\nFound: %i\nRequired: A value less than 65535\n", i, offset_temp);
                        return 0;
                    }
                    offset = offset_temp;
                }
                //if label position isn't an opcode and isn't a digit
                else if (isop(prgrm[i][opcpos+3]) == 0) {
                    for(j=0;j<flines;j++) {
                        //go through label array and see if we can find a match, take that index
                        if(strcmp(prgrm[i][opcpos+3],labelArray[j]) == 0) {
                            offset = j-(i+1);
                            printBits(offset);
                        }
                    }
                    if(offset == 0) {
                        printf("Can't find destination label: %s\n", prgrm[i][opcpos+3]);
                        return 0;
                    }
                }
                else {
                    printf("Error in line: %i\n", i);
                    return 0;
                }
            }
            
            
            
            /*
             * R-TYPES
             */
            else if(strcmp(prgrm[i][opcpos], "add") == 0)
            {
                type = 'R';
                opcode = 0;
                
                if((isRegister(prgrm[i][opcpos+1]) == 1) && (isRegister(prgrm[i][opcpos+2]) == 1) && (isRegister(prgrm[i][opcpos+3]) == 1)) {
                    r_dst = atoi(prgrm[i][opcpos+1]);
                    r0 = atoi(prgrm[i][opcpos+2]);
                    r1 = atoi(prgrm[i][opcpos+3]);
                }
                else {
                    printf("Registers given are incorrect on line %i\n",  i);
                    return 0;
                }
            }
            else if ((strcmp(prgrm[i][opcpos], "nand") == 0)) {
                type = 'I';
                opcode = 1;
                
                if((isRegister(prgrm[i][opcpos+1]) == 1) && (isRegister(prgrm[i][opcpos+2]) == 1) && (isRegister(prgrm[i][opcpos+3]) == 1)) {
                    r_dst = atoi(prgrm[i][opcpos+1]);
                    r0 = atoi(prgrm[i][opcpos+2]);
                    r1 = atoi(prgrm[i][opcpos+3]);
                }
                else {
                    printf("Registers given are incorrect on line %i\n",  i);
                    return 0;
                }
            }
            
            
            /*
             * O-Type Instructions
             */
            else if (strcmp(prgrm[i][opcpos], "halt")) {
                opcode = 6;
                type = 'O';
            }
            else if (strcmp(prgrm[i][opcpos], "noop")) {
                opcode = 7;
                type = 'O';
            }
            
            /*
             * J-Type Instructions
             */
            else if(strcmp(prgrm[i][opcpos], "jalr") == 0) {
                opcode = 5;
                type = 'J';
                
                if(isRegister(prgrm[i][opcpos+1]) == 1 && isRegister(prgrm[i][opcpos+2]) == 1) {
                    r0 = atoi(prgrm[i][opcpos+1]);
                    r1 = atoi(prgrm[i][opcpos+2]);
                }
                else {
                    printf("Registers are incorrect on line: %i\n", i);
                }
            }
            
            /*
             * PACKING
             */
            if(type == 'I') {
                //[31 UNUSED 25][24 OPCODE 22][21 rA 19][18 rB 16][15 Offset 0]
                instruction = (opcode<<22)|(r0<<19)|(r1<<16)|(offset);
                printBits(instruction);
            }
            else if (type == 'R') {
                // [31 UNUSED 25][24 OPCODE 22][21 rA 19][18 rB 16][15 unused 3][2 dstReg 0]
                instruction = (opcode<<22)|(r0<<19)|(r1<<16)|(r_dst);
                printBits(instruction);
            }
            else if (type == 'J') {
                instruction = (opcode<<25)|(r0<<19)|(r1<<16);
            }
            else if (type == 'O') {
                // [31 UNUSED 25][24 OPCODE 22][21 UNUSED 0]
                instruction = opcode<<22;
            }
        }
        else {
            printf("No opcode found in correct position for line: %i", i);
            return 0;
        }
        //if label position is something else (maybe an opcode?)
    }
}



/*
 * Splits a string into tokens and returns a pointer to a str array
 */
char **split(char s[]) {
    char *token;
    int i = 0;
    int count = countTokens(s);
    
    char **instr = malloc((count+1) * sizeof(char*));
    
    //get a count of tokens so we can allocate memory for them
    token = strtok (s," ");
    while (token != NULL)
    {
        //since token reference keeps changing and is static, use strdup instead.
        instr[i] = strdup(token);
        token = strtok (NULL, " ");
        
        i++;
    }
    return instr;
}

/*
 * Returns the amount of tokens in the str
 */
int countTokens(char* s) {
    int i = 0;
    int count = 0;
    char lastChar = ' ';
    
    while(s[i] != '\0'){
        if(lastChar == ' ' && s[i] != ' ') {
            count++;
        }
        lastChar = s[i];
        i++;
    }
    return count;
}

//counts the lines in a given file pointer
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

//checks if the given string is an opcode or not
int isop(char* s) {
    //opcodes
    char *opcodes[] = {"lw", "sw", "add", "nand", "beq", "jalr", "halt", "noop"};
    int i=0;
    for(i=0;i<7;i++) {
        if(strcmp(opcodes[i],s) == 0) {
            return 1;
        }
    }
    return 0;
}

//checks if the given string is a digit or not
int isDigit(char* s) {
    int i;
    if(s[0] == '-' || (s[0] <= '9' && s[0] >='0' )) {
        for(i=1;i<strlen(s);i++) {
            if(s[i] > '9' || s[i] < '0' ) {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}
//if the given string follows register protocol:
//-single digit
//-between 0 and 7
int isRegister(char* s) {
    int i=0;
    if(strlen(s) > 1) {
        return 0;
    }
    for(i=0;i<strlen(s);i++) {
        if(s[i] > '7' || s[i] < '0' ) {
            return 0;
        }
    }
    return 1;
}

//returns the first opcode position
int opcodepos(char** line, int len) {
    int i = 0;
    while(i<len) {
        if(isop(line[i]) == 1) {
            return i;
        }
        i++;
    }
    return -1;
}
void printBits(uint32_t pack) {
    int i;
    uint32_t temp = pack;
    
    for(i=31;i>=0;i--) {
        if(((temp>>i)&1) == 1) {
            printf("1");
        }
        else {
            printf("0");
        }
    }
    putchar('\n');
}
