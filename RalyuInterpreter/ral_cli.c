#include "ral_cli.h"

#include <stdio.h>
#include "ral_file.h"



void RalCLI_ReadArgs(int argc, char** argv)
{
	if (argc <= 1)
	{
		// No args given
		printf("No args given\n");
	} else
	{
		// Temporary: treat arg 1 as pointer to ral source file

		Ral_TextFile* mainsource = Ral_LoadTextFile(argv[1]);
		if (!mainsource)
		{
			RalCLI_ERROR("No main sourcefile has been supplied!");
			return;
		}
		RalCLI_DEBUGLOG("Main source:\n%s", mainsource->buffer);
	}
}
