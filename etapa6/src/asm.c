/*
Grupo:
- Diego Cardoso Nunes (00333984)
- Geancarlo Kozenieski (00264414)
*/

#include "asm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "errors.h"


#define streq(a, b) (strcmp(a, b) == 0)
#define MAX_TEMPS 10000
// x86_64 has 16 general purpose registers, but two of them are intended for 
// stack management and we reserve eax for operations that require its use 
// (such as division) and for auxiliary use
#define NUM_REGISTERS 13

// Variables used to assign registers
static int num_temps = 0;
static int first_use[MAX_TEMPS];
static int last_use[MAX_TEMPS];
static int interference_graph[MAX_TEMPS][MAX_TEMPS];
static int color[MAX_TEMPS];

// Mark that a temporary was used at the specified line.
// Temporaries must start with 'r' followed by a number.
// As a special case, the usage of 'rfp' is ignored
void mark_temp_use(char *temp, int line) {
    if (temp != NULL && temp[0] == 'r' && !streq(temp, "rfp") ) {
        int reg_num = atoi(temp + 1);
        if (first_use[reg_num] == -1) {
            first_use[reg_num] = line;
            num_temps++;
        }
        last_use[reg_num] = line;
    }
}

// Create the life-time table for every temporary used
void build_table(code_t *code) {
    memset(first_use, -1, sizeof(first_use));
    memset(last_use, -1, sizeof(last_use));

    code_t *current = code;
    int line = 1;
    while (current != NULL) {
        struct iloc_t inst = current->instruction;

        mark_temp_use(inst.arg1, line);
        mark_temp_use(inst.arg2, line);
        mark_temp_use(inst.arg3, line);

        current = current->next;
        line++;
    }
}

// Build the interference graph based on the life-time table
void build_graph() {
    memset(interference_graph, 0, sizeof(interference_graph));

    for (int temp1 = 0; temp1 < num_temps; temp1++) {
        for (int temp2 = temp1; temp2 < num_temps; temp2++) {
            if (
                (first_use[temp1] <= first_use[temp2] && first_use[temp2] <= last_use[temp1]) || (first_use[temp2] <= first_use[temp1] && first_use[temp1] <= last_use[temp2])
            ) {
                interference_graph[temp1][temp2] = 1;
                interference_graph[temp2][temp1] = 1;
            }
        }
    }    
}

// Color the interference graph to assign registers
void color_graph() {
    memset(color, -1, sizeof(color));
    int colored_nodes = 0;

    while (colored_nodes < num_temps) {
        int max_degree = -1;
        int max_degree_node = 0;

        for (int i = 0; i < num_temps; i++) {
            if (color[i] != -1) continue;
            int node_degree = 0;
            for (int j = 0; j < num_temps; j++) {
                if (i != j && interference_graph[i][j] && color[j] == -1) {
                    node_degree++;
                }
            }
            if (node_degree > max_degree) {
                max_degree = node_degree;
                max_degree_node = i;
            }
        }

        if (max_degree > NUM_REGISTERS) {
            printf("The program require more registers than available\n");
            exit(ERR_REG_COUNT);
        }

        for (int possible_color = 0; possible_color < NUM_REGISTERS; possible_color++) {
            int found = 1;
            for (int adj = 0; adj < num_temps; adj++) {
                if (interference_graph[max_degree_node][adj] == 1 && color[adj] == possible_color) {
                    found = 0;
                    break;
                }
            }
            if (found) {
                color[max_degree_node] = possible_color;
                break;
            }
        }
        colored_nodes++;
    }
}

// Perform register assignment
void assign_registers(code_t *code) {
    build_table(code);
    build_graph();
    color_graph();
}

// Get the real registers assigned to the temporary value
char* get_register(char *temp) {
    char* register_map[NUM_REGISTERS] = {
        "%ebx", "%ecx", "%edx", "%esi", "%edi","%r8d", "%r9d",
        "%r10d", "%r11d", "%r12d", "%r13d", "%r14d", "%r15d",
    };

    int temp_num = atoi(temp + 1);
    return register_map[color[temp_num]];
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

// Translate a single iloc instruction to assembly
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
    assign_registers(ir_code);

    generate_preamble();

    code_t *remaining = ir_code;

    while (remaining != NULL) {
        translate_instruction(remaining->instruction);
        remaining = remaining->next;
    }

    generate_epilogue();
}

