/*
Grupo:
- Diego Cardoso Nunes (00333984)
- Geancarlo Kozenieski (00264414)
*/

#include "asm.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define streq(a, b) (strcmp(a, b) == 0)

char* get_register(char *temp) {
    if (streq(temp, "r0")) {
        return "%ebx";
    } else if (streq(temp, "r1")) {
        return "%ecx";
    } else if (streq(temp, "r2")) {
        return "%edx";
    } else if (streq(temp, "r3")) {
        return "%esi";
    } else if (streq(temp, "r4")) {
        return "%edi";
    } else if (streq(temp, "r5")) {
        return "%r8d";
    } else if (streq(temp, "r6")) {
        return "%r9d";
    } else if (streq(temp, "r7")) {
        return "%r10d";
    } else if (streq(temp, "r8")) {
        return "%r11d";
    } else if (streq(temp, "r9")) {
        return "%r12d";
    } else if (streq(temp, "r10")) {
        return "%r13d";
    } else if (streq(temp, "r11")) {
        return "%r14d";
    } else if (streq(temp, "r12")) {
        return "%r15d";
    } else {
        printf("The program requires more registers than available\n");
        exit(100);
    }
}

void generate_preamble() {
    printf("\t.globl main\n");
    printf("main:\n");
    printf(".LFB0:\n");
    printf("\tpushq	%%rbp\n");
	printf("\tmovq	%%rsp, %%rbp\n");
}

void generate_epilogue() {
    printf(".LFE0:\n");
	printf("\t.section .note.GNU-stack,\"\",@progbits\n");
}

void translate_instruction(struct iloc_t instruction) {
    if (instruction.label != NULL)
        printf("%s:\n", instruction.label);

    if (streq(instruction.mnemonic, "loadI")) {
        printf("\tmovl\t$%s, %s\n", instruction.arg1, get_register(instruction.arg3));
    } else if (streq(instruction.mnemonic, "ret")) {
        printf("\tmovl\t%s, %%eax\n", get_register(instruction.arg1));
        printf("\tpopq\t%%rbp\n");
        printf("\tret\n");
    } else if (streq(instruction.mnemonic, "loadAI")) {
        printf("\tmovl\t-%d(%%rbp), %s\n", atoi(instruction.arg2)+4, get_register(instruction.arg3));
    } else if (streq(instruction.mnemonic, "storeAI")) {
        printf("\tmovl\t%s, -%d(%%rbp)\n", get_register(instruction.arg1), atoi(instruction.arg3)+4);
    } else if (streq(instruction.mnemonic, "add")) {
        printf("\tmovl\t%s, %s\n", get_register(instruction.arg1), get_register(instruction.arg3));
        printf("\taddl\t%s, %s\n", get_register(instruction.arg2), get_register(instruction.arg3));
    } else if (streq(instruction.mnemonic, "sub")) {
        printf("\tmovl\t%s, %s\n", get_register(instruction.arg1), get_register(instruction.arg3));
        printf("\tsubl\t%s, %s\n", get_register(instruction.arg2), get_register(instruction.arg3));
    } else if (streq(instruction.mnemonic, "mult")) {
        printf("\tmovl\t%s, %s\n", get_register(instruction.arg1), get_register(instruction.arg3));
        printf("\timull\t%s, %s\n", get_register(instruction.arg2), get_register(instruction.arg3));
    } else if (streq(instruction.mnemonic, "div")) {
        printf("\tmovl\t%s, %%eax\n", get_register(instruction.arg1));
        printf("\tcltd\n");
        printf("\tidivl\t%s\n", get_register(instruction.arg2));
        printf("\tmovl\t%%eax,%s\n", get_register(instruction.arg3));
    } else if (streq(instruction.mnemonic, "rsubI")) {
        printf("\tmovl\t$%s, %s\n", instruction.arg2, get_register(instruction.arg3));
        printf("\tsubl\t%s, %s\n", get_register(instruction.arg1), get_register(instruction.arg3));
    } else if (streq(instruction.mnemonic, "cmp_EQ")) {
        printf("\tcmpl\t%s, %s\n", get_register(instruction.arg2), get_register(instruction.arg1));
        printf("\tsete\t%%al\n");
        printf("\tmovzbl\t%%al, %%eax\n");
        printf("\tmovl\t%%eax, %s\n", get_register(instruction.arg3));
    } else if (streq(instruction.mnemonic, "cmp_NE")) {
        printf("\tcmpl\t%s, %s\n", get_register(instruction.arg2), get_register(instruction.arg1));
        printf("\tsetne\t%%al\n");
        printf("\tmovzbl\t%%al, %%eax\n");
        printf("\tmovl\t%%eax, %s\n", get_register(instruction.arg3));
    } else if (streq(instruction.mnemonic, "cmp_LE")) {
        printf("\tcmpl\t%s, %s\n", get_register(instruction.arg2), get_register(instruction.arg1));
        printf("\tsetle\t%%al\n");
        printf("\tmovzbl\t%%al, %%eax\n");
        printf("\tmovl\t%%eax, %s\n", get_register(instruction.arg3));
    } else if (streq(instruction.mnemonic, "cmp_LT")) {
        printf("\tcmpl\t%s, %s\n", get_register(instruction.arg2), get_register(instruction.arg1));
        printf("\tsetl\t%%al\n");
        printf("\tmovzbl\t%%al, %%eax\n");
        printf("\tmovl\t%%eax, %s\n", get_register(instruction.arg3));
    } else if (streq(instruction.mnemonic, "cmp_GE")) {
        printf("\tcmpl\t%s, %s\n", get_register(instruction.arg2), get_register(instruction.arg1));
        printf("\tsetge\t%%al\n");
        printf("\tmovzbl\t%%al, %%eax\n");
        printf("\tmovl\t%%eax, %s\n", get_register(instruction.arg3));
    } else if (streq(instruction.mnemonic, "cmp_GT")) {
        printf("\tcmpl\t%s, %s\n", get_register(instruction.arg2), get_register(instruction.arg1));
        printf("\tsetg\t%%al\n");
        printf("\tmovzbl\t%%al, %%eax\n");
        printf("\tmovl\t%%eax, %s\n", get_register(instruction.arg3));
    } else if (streq(instruction.mnemonic, "and")) {
        printf("\tmovl\t%s, %s\n", get_register(instruction.arg1), get_register(instruction.arg3));
        printf("\timull\t%s, %s\n", get_register(instruction.arg2), get_register(instruction.arg3));
        printf("\ttest\t%s, %s\n",get_register(instruction.arg3), get_register(instruction.arg3));
        printf("\tsetne\t%%al\n");
        printf("\tmovzbl\t%%al, %%eax\n");
        printf("\tmovl\t%%eax, %s\n", get_register(instruction.arg3));
    } else if (streq(instruction.mnemonic, "or")) {
        printf("\tmovl\t%s, %s\n", get_register(instruction.arg1), get_register(instruction.arg3));
        printf("\tor\t%s,%s\n",get_register(instruction.arg2),get_register(instruction.arg3));
        printf("\ttest\t%s, %s\n",get_register(instruction.arg3), get_register(instruction.arg3));
        printf("\tsetne\t%%al\n");
        printf("\tmovzbl\t%%al, %%eax\n");
        printf("\tmovl\t%%eax, %s\n", get_register(instruction.arg3));
    } else if (streq(instruction.mnemonic, "cbr")) {
        printf("\ttest\t%s,%s\n",get_register(instruction.arg1), get_register(instruction.arg1));
        printf("\tjne\t%s\n", instruction.arg2);
        printf("\tjmp\t%s\n", instruction.arg3);
    } else if (streq(instruction.mnemonic, "jumpI")) {
        printf("\tjmp %s\n", instruction.arg2);
    } else if (streq(instruction.mnemonic, "nop")) {
        printf("\tnop\n");
    } 
}

void generateAsm(code_t* ir_code) {

    generate_preamble();

    code_t *remaining = ir_code;

    while (remaining != NULL) {
        translate_instruction(remaining->instruction);
        remaining = remaining->next;
    }

    generate_epilogue();
}

