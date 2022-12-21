/*
This file is here to test if Ralyu has compiled and is running correctly.
To embedd Ralyu in a program, simply delete this file and setup Ralyu in that program.
*/

#include "ralu_stdinclude.h"
#include "ralu_list.h"
#include "ralu_memory.h"
#include "ralu_string.h"
#include <stdio.h>

#include "ral_lexer.h"



int main(int argc, char** argv)
{
	printf("Test implementation of Ralyu\n");

	Ral_ListNode node1 = { 0 };
	Ral_ListNode node2 = { 0 };
	Ral_ListNode node3 = { 0 };
	Ral_List list = { 0 };

	Ral_PushBackList(&list, &node1);
	Ral_PushBackList(&list, &node2);
	Ral_PushBackList(&list, &node3);
	Ral_UnlinkFromList(&list, &node2);



	Ral_SourceUnit* source = Ral_ALLOC_TYPE(Ral_SourceUnit);
	Ral_ParseSourceUnit(source, Ral_STR_COMMA_LEN("int a = \"Hello\"\nint b = 0\nif a == 0 then\nprint()\nend"));

}
