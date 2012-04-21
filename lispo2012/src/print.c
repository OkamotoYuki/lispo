#include "lispo.h"

void print_result(data_t *result)
{
	if(result->dtype == D_INT) printf("%d\n", result->value);
	else if(result->dtype == D_BOOL) {
		switch(result->value) {
			case T:
				printf("t\n");
				break;
			case NIL:
				printf("nil\n");
				break;
		}
	}
}
