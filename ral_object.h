#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"



typedef long long int	Ral_Integer;
typedef long double		Ral_Number;

typedef struct Ral_Object
{
	Ral_LISTLINKS(Ral_Object);

	union
	{
		Ral_Integer integer;
		Ral_Number number;
	} value;
} Ral_Object;
