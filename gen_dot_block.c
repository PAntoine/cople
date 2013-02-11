/* $Revision: 1.2 $
 *		       Generate DOT block 
 *
 *	This function will generate the block that the clauses all hang off
 *	this funcion basically calls the gen_dot_block for every instruction
 *	in the block.
 *
 *		Copyright (c) 1999 Petee Antoine.
 *
 */

#include	<stdio.h>
#include	<memory.h>
#include	"codetree.h"

int	gen_dot_block(CLAUSE* clause)
{
	int	count,size,level_size;
	int	*clauses;
	char	*buff;
	extern int	clause_num;
	extern FILE	*dot_file;

	clauses = (int*) calloc(clause->c_block.num_instr,sizeof(int));
	size    = clause->c_block.num_instr;

	for(count=0;count<size;count++)
		clauses[count] = gen_dot_clause(clause->c_block.instr_list[count]);

	clause_num++;

	buff = (char*) calloc((22+(size*9)),sizeof(char));

	strcpy(buff,"[label = \"<instr>block|");
	level_size = 23;

	for(count=0;count<size-1;count++)
		sprintf(&buff[level_size+(count*8)],"<e%03d> |",count);

	sprintf(&buff[level_size+(count*8)],"<e%03d> \"];",count);
	fprintf(dot_file,"node%d%s\n",clause_num,buff);

	for(count=0;count<size;count++)
		fprintf(dot_file,"node%d:e%03d -> node%d:instr;\n",clause_num,count,clauses[count]);

	free(buff);
	free(clauses);
	return(clause_num);
}
