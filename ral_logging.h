#pragma once

#include "ralu_stdinclude.h"
#include <stdio.h>

#define Ral_LOG_SYNTAXERROR(linenum, msg, ...) printf("Syntax error on line %i: " msg "\n", linenum, __VA_ARGS__);
