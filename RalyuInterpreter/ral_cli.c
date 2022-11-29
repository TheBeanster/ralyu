#include "ral_cli.h"

#include <stdio.h>



void RalCLI_ReadArgs(int argc, char** argv)
{
	if (argc <= 1)
	{
		// No args given
		printf("No args given\n");
	} else
	{
		printf("Args:\n");
		for (int i = 0; i < argc; i++)
		{
			printf("> %s\n", argv[i]);
		}
	}
}
