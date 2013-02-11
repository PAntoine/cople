/* $Revision: 1.2 $
 *		       Generate DOT clause
 *
 *	This function will generate an entry into the dot file for a
 *	code clause. It will recursivly desend the code tree. It also
 *	returns the node number of the node that has been created. If
 *	the tree has a leaf then the name of the simbol is placed in
 *	the coresponding entry.
 *
 *		Copyright (c) 1999 Petee Antoine.
 *
 */

#include	<stdio.h>
#include	"codetree.h"

int	gen_dot_clause(CLAUSE* clause)
{
	int	one,two,three,four;
	char	*ref_tag;
	char	*name,*realm,*service;
	extern int	clause_num;
	extern int	initial_size;
	extern char	*prog_name;
	extern char	*initial_space;
	extern FILE	*dot_file;

	switch(clause->c_type)
	{
	case ct_bool:	one = gen_dot_clause(clause->c_bool.left);
		two = gen_dot_clause(clause->c_bool.right);
		fprintf(dot_file,"node%d[label = \"<instr> %s |<left> |<right> \"];\n",
			clause_num,instr_name[clause->c_bool.instr]);
		fprintf(dot_file,"node%d:left -> node%d:instr;\n",clause_num,one);
		fprintf(dot_file,"node%d:right -> node%d:instr;\n",clause_num,two);
		break;

	case ct_unary:	one = gen_dot_clause(clause->c_unary.operand);
		fprintf(dot_file,"node%d[label = \"<instr> %s |<op> \"];\n",clause_num,instr_name[clause->c_unary.instr]);
		fprintf(dot_file,"node%d:op -> node%d:instr;\n",clause_num,one);
		break;

	case ct_whenever:
		one = gen_dot_clause(clause->c_whenever.code);
		fprintf(dot_file,"node%d[label = \"{<instr> WHENEVER |<action>%s } \"];\n", clause_num,
					exception[clause->c_whenever.exception]);
		fprintf(dot_file,"node%d:action -> node%d:instr;\n",clause_num,one);
		break;

	case ct_test:	one = gen_dot_clause(clause->c_test.condition);
		two = gen_dot_clause(clause->c_test.action);
		fprintf(dot_file,"node%d[label = \"<instr> TEST |<left> |<right> \"];\n", clause_num);
		fprintf(dot_file,"node%d:left -> node%d:instr;\n",clause_num,one);
		fprintf(dot_file,"node%d:right -> node%d:instr;\n",clause_num,two);
		break;

	case ct_if:	one = gen_dot_clause(clause->c_if.condition);
		two = gen_dot_clause(clause->c_if.action);
		three = gen_dot_clause(clause->c_if.on_fail);
		fprintf(dot_file,"node%d[label = \"<instr> TEST |<left> |<right> |<fail> \"];\n", clause_num);
		fprintf(dot_file,"node%d:left -> node%d:instr;\n",clause_num,one);
		fprintf(dot_file,"node%d:right -> node%d:instr;\n",clause_num,two);
		fprintf(dot_file,"node%d:fail -> node%d:instr;\n",clause_num,three);
		break;

	case ct_while:	one = gen_dot_clause(clause->c_test.condition);
		two = gen_dot_clause(clause->c_while.action);
		fprintf(dot_file,"node%d[label = \"<instr> while |<left> |<right> \"];\n", clause_num);
		fprintf(dot_file,"node%d:left -> node%d:instr;\n",clause_num,one);
		fprintf(dot_file,"node%d:right -> node%d:instr;\n",clause_num,two);
		break;

	case ct_for:	one   = gen_dot_clause(clause->c_while.condition);
		two   = gen_dot_clause(clause->c_for.initial);
		three = gen_dot_clause(clause->c_for.step);
		four  = gen_dot_clause(clause->c_for.action);
		fprintf(dot_file,"node%d[label = \"<instr> for |<one> |<two> |<three> |<four>\"];\n", clause_num);
		fprintf(dot_file,"node%d:one -> node%d:instr;\n",clause_num,one);
		fprintf(dot_file,"node%d:two -> node%d:instr;\n",clause_num,two);
		fprintf(dot_file,"node%d:three -> node%d:instr;\n",clause_num,three);
		fprintf(dot_file,"node%d:four -> node%d:instr;\n",clause_num,four);
		break;

	case ct_forall:	one = gen_dot_clause(clause->c_forall.ref_var);
		two = gen_dot_clause(clause->c_forall.action);
		fprintf(dot_file,"node%d[label = \"<instr> forall |<one>var |<two> action\"];\n", clause_num);
		fprintf(dot_file,"node%d:one -> node%d:instr;\n",clause_num,one);
		fprintf(dot_file,"node%d:two -> node%d:instr;\n",clause_num,two);
		break;
	
	case ct_call:	if (clause->c_call.object != NULL)
		{
			if (clause->c_call.object->c_variable.value.type != c_reference)
				four = clause->c_call.object->c_variable.value.data.integer;
			else {
				if (clause->c_call.object->c_variable.value.data.symbol->access_type & a_global)
					four = clause->c_call.object->c_variable.value.data.symbol->offset;
				else
					four = initial_size + clause->c_call.object->c_variable.value.data.symbol->offset;
			};
		
			realm   = conv_cobos_to_unix_str(initial_space + four);
			name    = conv_cobos_to_unix_str(initial_space + four+4);
			three   = (long int) *(initial_space + four + 8);
		} else {
			realm = "<local>";
			name  = prog_name;
			three = 0;
		};	
		
		service = conv_cobos_to_unix_str(initial_space + clause->c_call.service->data.string);


		fprintf(dot_file,"node%d[label = \"{<instr>service call|%s%s|%s%s|%s%d|%s%s}\"];\n",
				clause_num,
				"realm:",realm,
				"object:",name,
				"instance:",three,
				"service:",service);
		break;

	case ct_array:	one   = gen_dot_clause(clause->c_array.symbol);
		two   = gen_dot_clause(clause->c_array.entry);
		fprintf(dot_file,"node%d[label = \"<instr> ARRAY REF |<one> |<two> \"];\n", clause_num);
		fprintf(dot_file,"node%d:one -> node%d:instr;\n",clause_num,one);
		fprintf(dot_file,"node%d:two -> node%d:instr;\n",clause_num,two);
		break;

	case ct_variable:
		switch(clause->c_variable.value.type)
		{
		case c_int:	fprintf(dot_file,"node%d[label = \"<instr>%d\"];\n",
			clause_num,
			clause->c_variable.value.data.integer);
			break;
	
		case c_boolean:	fprintf(dot_file,"node%d[label = \"<instr>%s\"];\n",
			clause_num,
			clause->c_variable.value.data.boolean?"true":"false");
			break;
		
		case c_real:	fprintf(dot_file,"node%d[label = \"<instr>%f\"];\n",
			clause_num,
			clause->c_variable.value.data.flt_point);
			break;
		
		case c_char:	fprintf(dot_file,"node%d[label = \"<instr>%c\"];\n",
			clause_num,
			clause->c_variable.value.data.character);
			break;
		
		case c_string:	name = conv_cobos_to_unix_str(initial_space+clause->c_variable.value.data.string);
			fprintf(dot_file,"node%d[label = \"<instr>%s\"];\n",clause_num,name);
			free(name);
			break;
		
		case c_reference: 
		case c_addr:	
			if (clause->c_variable.value.type == c_addr)
				ref_tag = "ref to ";
			else
				ref_tag = "";
			
			if (clause->c_variable.value.data.symbol->type == d_array)
			{
				fprintf(dot_file,"node%d[label = \"{<instr>%sARRAY|%s%s|%s%s|%s%d|%s%d}\"];\n",
					clause_num,ref_tag,
					"name:",clause->c_variable.value.data.symbol->name,
					"type:",type_name[clause->c_variable.value.data.symbol->array_type],
					"size:",clause->c_variable.value.data.symbol->size,
					"dimensions:",clause->c_variable.value.data.symbol->num_dim);
			}else{
				fprintf(dot_file,"node%d[label = \"{<instr>%sSYMBOL|name: %s|type:%s}\"];\n",
				clause_num,ref_tag,
				clause->c_variable.value.data.symbol->name,
				type_name[clause->c_variable.value.data.symbol->type]);
			}
			break;
		};
		break;

	case ct_service:
		one = gen_dot_clause(clause->c_service.code);
		fprintf(dot_file,"node%d[label = \"{<instr>SERVICE|%s%s|%s%s}\"];",
				clause_num,
				"name:",clause->c_service.name->name,
				"returns:", type_name[clause->c_service.result]);
		fprintf(dot_file,"node%d -> node%d:instr;\n",clause_num,one);
		break;	

	case ct_block:	gen_dot_block(clause);
		break;
		
	/* others - need this holder for it to work.*/
	default:	fprintf(dot_file,"node%d[label = \"<instr> not done yet! \"];\n",clause_num);
	};

	return(clause_num++);
}
