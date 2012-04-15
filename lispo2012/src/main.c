#include "lispo.h"

int main(int argc, char **argv)
{
	if(argc > 2) {
		// TODO print arg error.
		exit(1);
	}

	lcontext_t *ctx = new_rootContext(argc, argv);

	while(1) {
		read(ctx);
		init_rootContext(ctx);
	}

	return 0;
}
