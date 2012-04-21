#include "lispo.h"

int depth = 0;

static void print_tab(void)
{
	int i = 0;
	while(i < depth) {
		printf("\t");
		i++;
	}
}

void print_consTree(cons_t *head)
{
	if(!head) {
		depth--;
		return;
	}

	switch(head->otype) {
		case O_START_BRACKET:
			print_tab();
			depth++;
			printf("(\n");
			print_consTree(head->car);
			print_consTree(head->cdr);
			return;
		case O_ADD:
			print_tab();
			printf("+\n");
			print_consTree(head->cdr);
			return;
		case O_SUB:
			print_tab();
			printf("-\n");
			print_consTree(head->cdr);
			return;
		case O_MUL:
			print_tab();
			printf("*\n");
			print_consTree(head->cdr);
			return;
		case O_DIV:
			print_tab();
			printf("/\n");
			print_consTree(head->cdr);
			return;
		case O_NUM:
			print_tab();
			printf("%d\n", head->ivalue);
			print_consTree(head->cdr);
			return;
		case O_IF:
			print_tab();
			printf("if\n");
			print_consTree(head->cdr);
			return;
		case O_SETQ:
			print_tab();
			printf("setq\n");
			print_consTree(head->cdr);
			return;
		case O_DEFUN:
			print_tab();
			printf("defun\n");
			print_consTree(head->cdr);
			return;
		case O_STRING:
			print_tab();
			printf("%s\n", head->svalue);
			print_consTree(head->cdr);
			return;
		default:
			return;
	}
}

void print_VMCode(VMCode *start)
{
	switch(start->otype) {
		case O_OpPUSH:
			printf("PUSH->");
			print_VMCode(start->next);
			return;
		case O_OpPOP:
			printf("POP->");
			print_VMCode(start->next);
			return;
		case O_OpADD:
			printf("ADD->");
			print_VMCode(start->next);
			return;
		case O_OpEND:
			printf("END\n");
			return;
	}
}
