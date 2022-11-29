#include <stdio.h>
#include <stdlib.h>

#include "ral_cli.h"

int main(int argc, char** argv)
{
	printf(" --- Ralyu interpreter ---\n");

	RalCLI_ReadArgs(argc, argv);

	system("pause");

	return 0;
}
