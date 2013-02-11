/* $revision$
 *
 *		         LABEL functions
 *
 *	These functions will assign a label and place it on the label
 *	stack so that when the next generate instrucion call is made
 *	it will be able to find the label and apply it to the output.
 *
 *		Copyright (c) 1999 Peter  Antoine
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<memory.h>
#include	"cople.h"


OPERAND	get_label(void)
{
	OPERAND	op;
	extern int	label_number;

	op.type  = o_label;
	op.data  = label_number++;

	return op;
}

void	put_label(OPERAND label)
{
	LEVEL_LNK	*walk,*last;
	extern LEVEL_LNK *label_list;

	walk = label_list;
	
	if (label_list == NULL)
	{
		label_list = (LEVEL_LNK*) calloc(1,sizeof(LEVEL_LNK));
		label_list->count = label.data;
	} else {
		while(walk != NULL)
		{
			last = walk;
			walk = walk->next;
		};

		last->next = (LEVEL_LNK*) malloc(sizeof(LEVEL_LNK));
		last->next->prev  = last;
		last->next->count = label.data;
	};
}

void	gen_labels(void)
{
	LEVEL_LNK		*walk,*last;
	extern FILE		*object_file;
	extern LEVEL_LNK 	*label_list;

	walk = label_list;
	
	while(walk != NULL)
	{
		fprintf(object_file,"l_%d:\n",walk->count);
		last = walk;
		walk = walk->next;
		free(last);
	};

	label_list = NULL;
}

	
