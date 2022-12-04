#include <stdio.h>
#include <stdlib.h>
#include "ral_memory.h"
#include "ral_file.h"

#include "ral_cli.h"
#include "ral_sourceunit.h"
#include "ral_lexer.h"
#include "ral_logging.h"
#include "ral_execute.h"



int main(int argc, char** argv)
{
	printf(" --- Ralyu interpreter ---\n");

	//RalCLI_ReadArgs(argc, argv);



	// This is all temporary code just to load some test ralyu code, it is bad and horrible

	Ral_TextFile* sourcefile = Ral_LoadTextFile(argv[1]);
	if (!sourcefile)
	{
		// Couldn't load source file
		return 0;
	}

	Ral_SourceUnit* mainsource = Ral_CreateSourceUnit(sourcefile);
	if (!mainsource)
	{
		// Couldn't create sourceunit from file
		return 0;
	}

	if (!Ral_TokenizeSourceUnit(mainsource))
	{
		// Couldn't tokenize sourceunit
		Ral_PrintAllErrorMessages(mainsource);
		return 0;
	}



	// Print what the tokenizer got
	for (int i = 0; i < mainsource->numstatements; i++)
	{
		Ral_PrintStatementTokens(&mainsource->statements[i]);
	}



	Ral_ExecuteSource(mainsource);
	


	Ral_DestroySourceUnit(mainsource);

	Ral_DestroyTextFile(mainsource->file);
	Ral_FREE(mainsource);

	return 0;
}
