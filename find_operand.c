/* $Revision: 1.2 $
 *		FIND OPERAND
 *
 *	This function will create a symbol reference clause for the
 *	variable named as the first parameter. What that function will
 *	do is to find the symbol that must allready be defined, and 
 *	check to see if it is of the type named. It will also check
 *	to see if it has dimension 0, which means that it is not an
 *	array.
 *
 *		(c) 1999 Peter Antoine.
 */

#include	<malloc.h>
#include	"codetree.h"

CLAUSE*	find_operand(char* name, DATA_TYPE type)
{
	CLAUSE	*code;
	SYMBOL	*symbol;
	char	*error_mess = malloc(100);

	code = calloc(1,sizeof(CLAUSE));

	symbol = find_variable(name);

	if (symbol == NULL)
		printf("ERROR: %s does not exist and is being referenced\n",name);
	else{
		if (symbol->type != type && type != d_null){
			strcpy(error_mess, "ERROR: ");
			strcat(error_mess, name);
			strcat(error_mess, " type is incorrect. \n");
			yyerror(error_mess);
			free(error_mess);
		}else{
			code->c_type = ct_variable;
			code->c_variable.value.type	   = c_reference;
			code->c_variable.value.data.symbol = symbol; 
		};
	}
	return code;
}
