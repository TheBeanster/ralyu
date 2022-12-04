#pragma once

#include "ralu_stdinclude.h"

#define RalCLI_DEBUGLOG(format, ...) printf("> "format"\n", __VA_ARGS__)
#define RalCLI_ERROR(format, ...) printf("ERROR > "format"\n", __VA_ARGS__)

void RalCLI_ReadArgs(int argc, char** argv);
