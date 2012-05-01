#include "lispo.h"

int main(int argc, char **argv)
{
	if(argc > 2) {
		lstrerr(ARG_ERROR);
		exit(1);
	}

	lcontext_t *ctx = new_rootContext(argc, argv);
	data_t *result;

	while(1) {
		read(ctx);
//		print_consTree(TREE_ROOT);
		compile(ctx);
//		print_VMCode(START_OF_VM_CODE);
		if(!ctx->isDefun) {
			result = run_VM(ctx);
			print_result(result);
		}
		depth = 0;
		init_rootContext(ctx);
	}

	return 0;
}
