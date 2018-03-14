//
//  Assembler.c
//  Assembler-Simulator-XP
//
//  Created by Drew Wilken and Nathan Taylor on 3/1/18.
//  Copyright © 2018 Drew Wilken and Nathan Taylor. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <ctype.h>

/*
 * Function Prototypes
 */
char **split(char s[]);
int countTokens(char* s);
int countLines(FILE* f);
int isop(char* s);
int isDigit(char* s);
int isRegister(char* s);
bool isValidLabel(char* s);
int opcodepos(char** line, int len);
void printBits(uint32_t pack);
bool isSpaceBeforeOpcode(char* s);

/*
 * MAIN FUNCTION
 */
int main(int argc, char **argv) {
    
    //indices
    int i=0;
    int j=0;
    int k=0;
    
    //run-mode
    bool fileout = false;
    
    //File handling
    FILE *file = NULL;
    FILE *out = NULL;
    
    argc = 3;
    argv[1] = "run.a";
    argv[2] = "run.mc";
    
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
    
    //counts
    int lcount = 0;
    
    //current line in the input
    char line[200];
    
    //num lines in the file
    int flines = countLines(file);
    
    //label arrays
    char** labelArray = malloc(flines * sizeof(char*));
    
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
        
        if(isSpaceBeforeOpcode(line) == false) {
            printf("Warning: opcodes must have a space before them, and labels must not.\nLine %d does not follow this rule!\n", lcount);
            return 0;
        }
        prgrm[i] = split(line);
        labelArray[i] = "";
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
            if(!isValidLabel(firstToken)) {
                printf("Incorrect label: %s\nLabels must not start with a number or be larger than 9 characters, and not contain any special characters!\n", firstToken);
                return 0;
            }
            for(j=0; j<lcount;j++) {
                if(strcmp(firstToken,labelArray[j]) == 0) {
                    printf("Error: Duplicate label: %s\n in line: %i\n", firstToken, i+1);
                    return 0;
                }
            }
            labelArray[i] = strdup(firstToken); //add label to label array
        }
        else {
            labelArray[i] = "";
        }
    }
    /*
     * SECOND PASS
     */
    uint32_t opcode = -1;
    uint32_t r0 = 0;
    uint32_t r1 = 0;
    uint32_t r_dst = 0;
    uint16_t offset = 0;
    int32_t fillValue = 0;
    uint32_t instruction = 0;
    bool foundOffset = false;
    bool hasHalted = false;
    char type = ' ';
    for(i=0;i<lcount;i++) {
        
        //go through first two tokens in each line and see where/if there's an opcode
        if(isop(prgrm[i][0]) == 1 || isop(prgrm[i][1]) == 1)  { //correct if
            int opcpos = opcodepos(prgrm[i], tokensPerLine[i]);
            /*
             * I-TYPE INSTRUCTIONS
             */
            if(strcmp(prgrm[i][opcpos],"lw") == 0 || strcmp(prgrm[i][opcpos],"sw") == 0 || strcmp(prgrm[i][opcpos],"beq") == 0 ) {
                if(strcmp(prgrm[i][opcpos],"lw") == 0) {
                    opcode = 2;
                }
                else if(strcmp(prgrm[i][opcpos],"sw") == 0){
                    opcode = 3;
                }
                else if(strcmp(prgrm[i][opcpos],"beq") == 0){ //beq
                    opcode = 4;
                }
                type = 'I';
                
                //next two fields must be register numbers
                if(isRegister(prgrm[i][opcpos+1]) == 1 && isRegister(prgrm[i][opcpos+2]) == 1) {
                    //set register ints
                    r0 = atoi(prgrm[i][opcpos+1]);
                    r1 = atoi(prgrm[i][opcpos+2]);
                }
                else {
                    printf("Error: Illegal registers for I-Type instruction in line: %i\n\n\nRequired: [opcode] [int] [int] [int]\n", i+1);
                    return 0;
                }
                
                //if label position is a digit
                if(isDigit(prgrm[i][opcpos+3]) == 1) {//
                    uint16_t offset_temp = atoi(prgrm[i][opcpos+3]);
                    if(offset_temp>65535) {
                        printf("Error: Offset value on line %i is incorrect!\n\nFound: %i\nRequired: A value less than 65535\n", i+1, offset_temp);
                        return 0;
                    }
                    offset = offset_temp;
                }
                //if label position isn't an opcode and isn't a digit
                else if (isop(prgrm[i][opcpos+3]) == 0) {
                    for(j=0;j<flines;j++) {
                        //go through label array and see if we can find a match, take that index
                        if(strcmp(prgrm[i][opcpos+3],labelArray[j]) == 0) {
                            
                            //if beq, set offset = to actual address in file rather than difference
                            if (strcmp(prgrm[i][opcpos],"beq") == 0) { offset = j-(i+1); }
                            else { offset = j; }
                            foundOffset = true;
                        }
                    }
                    if(!foundOffset) {
                        printf("Error: Can't find destination label: %s in line: %i\n", prgrm[i][opcpos+3], i+1);
                        return 0;
                    }
                }
                else {
                    printf("Error in line: %i\n", i+1);
                    return 0;
                }
            }
            
            
            
            /*
             * R-TYPES
             */
            else if(strcmp(prgrm[i][opcpos], "add") == 0 || strcmp(prgrm[i][opcpos], "nand") == 0)
            {
                if (strcmp(prgrm[i][opcpos], "add") == 0) {
                    opcode = 0;
                }
                else {
                    opcode = 1;
                }
                type = 'R';
                
                if((isRegister(prgrm[i][opcpos+1]) == 1) && (isRegister(prgrm[i][opcpos+2]) == 1) && (isRegister(prgrm[i][opcpos+3]) == 1)) {
                    r_dst = atoi(prgrm[i][opcpos+1]);
                    r0 = atoi(prgrm[i][opcpos+2]);
                    r1 = atoi(prgrm[i][opcpos+3]);
                }
                else {
                    printf("Error: Registers given are incorrect on line %i\n",  i+1);
                    return 0;
                }
            }
            
            
            /*
             * O-Type Instructions
             */
            else if (strcmp(prgrm[i][opcpos], "halt") == 0 || strcmp(prgrm[i][opcpos], "noop") == 0) {
                if (strcmp(prgrm[i][opcpos], "halt") == 0) {
                    hasHalted = true;
                    opcode = 6;
                }
                else {
                    opcode = 7;
                }
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
                    printf("Error: Registers are incorrect on line: %i\n", i+1);
                }
            }
            /*
             * .fill
             */
            else if(strcmp(prgrm[i][opcpos], ".fill") == 0) {
                if(!hasHalted) {
                    printf("Error: .fill instructions must be written after halt only!\n");
                    return 0;
                }
                opcode = 0;
                type = 'F';
                
                if(isDigit(prgrm[i][opcpos+1]) == 1) {
                    fillValue = atoi(prgrm[i][opcpos+1]);
                }
                else if (isop(prgrm[i][opcpos+1]) == 0 && isDigit(prgrm[i][opcpos+1]) == 0 && isValidLabel(prgrm[i][opcpos+1])) {
                    for(j=0;j<flines;j++) {
                        //go through label array and see if we can find a match, take that index
                        if(strcmp(prgrm[i][opcpos+1],labelArray[j]) == 0) {
                            fillValue = j;
                        }
                    }
                }
                else {
                    printf("Error: Registers are incorrect on line: %i\n", i+1);
                    return 0;
                }
            }
            
            /*
             * PACKING
             */
            if(type == 'I') {
                //[31 UNUSED 25][24 OPCODE 22][21 rA 19][18 rB 16][15 Offset 0]
                instruction = (opcode<<22)|(r0<<19)|(r1<<16)|(offset);
                printBits(instruction);
                fprintf(fileout == false ? stdout : out, "%" PRIu32 "\n", instruction);
            }
            else if (type == 'R') {
                // [31 UNUSED 25][24 OPCODE 22][21 rA 19][18 rB 16][15 unused 3][2 dstReg 0]
                instruction = (opcode<<22)|(r0<<19)|(r1<<16)|(r_dst);
                printBits(instruction);
                fprintf(fileout == false ? stdout : out, "%" PRIu32 "\n", instruction);
            }
            else if (type == 'J') {
                instruction = (opcode<<25)|(r0<<19)|(r1<<16);
                printBits(instruction);
                fprintf(fileout == false ? stdout : out, "%" PRIu32 "\n", instruction);
            }
            else if (type == 'O') {
                // [31 UNUSED 25][24 OPCODE 22][21 UNUSED 0]
                instruction = opcode<<22;
                printBits(instruction);
                fprintf(fileout == false ? stdout : out, "%" PRIu32 "\n", instruction);
            }
            else if (type == 'F') {
                // [31 UNUSED 25][24 OPCODE 22][21 UNUSED 0]
                instruction = fillValue;
                printBits(instruction);
                fprintf(fileout == false ? stdout : out, "%" PRId32 "\n", instruction);
            }
        }
        else {
            printf("Error: No opcode found in correct position for line: %i\n", i+1);
            return 0;
        }
    }
    fclose(file);
    fclose(out);
    
    /*
     * FREEING MEMORY
     */
    for(i=0;i<lcount;i++) {
        for(j=0;j<tokensPerLine[i];j++) {
            free(prgrm[i][j]);
        }
        free(prgrm[i]);
    }
    free(labelArray);
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
    char *opcodes[] = {"lw", "sw", "add", "nand", "beq", "jalr", "halt", "noop", ".fill"};
    int i=0;
    for(i=0;i<9;i++) {
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

//checks whether or not a string is a valid label
bool isValidLabel(char* s) {
    int i;
    if(isdigit(s[0])) {
        return false;
    }
    if(strlen(s) > 9) {
        return false;
    }
    if(isop(s) == 1) {
        return false;
    }
    for(i=1;i<strlen(s);i++) {
        if(!isalnum(s[i])) {
            return false;
        }
    }
    return true;
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

//Checks if the first character is a space and if it's an opcode
//in conformance to Dr. Myre's format rules
bool isSpaceBeforeOpcode(char* s) {
    char* firstToken;
    char* temp;
    
    temp = strdup(s);
    
    firstToken = strtok (temp," ");
    
    if(isop(firstToken)) {
        //check to see if space before
        if (temp[0] == ' ') {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        if (temp[0] == ' ') {
            return false;
        }
        else {
            return true;
        }
    }
}

