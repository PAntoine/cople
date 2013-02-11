# $Id: makefile,v 1.2 1999/11/07 10:36:53 devel Exp $
#	            COPLE version 1
#	Concurrent Object Programming LanguagE
#
#	This is a build file for the COPLE compiler. The complier
#	is based arround the YACC (bison) compiler compiler.
#
#	     (c) 1998 Copyright Peter Antoine.



SRCNAMES = add_clause add_parameter add_range_fixup add_symbol add_to_fixup add_to_list add_variable \
	calculate_array_entry cobos_string conv_const_list_to_array conv_int_to_bigendian conv_list_to_array \
	cople.lex create_block_from_list create_list create_xxx_const end_level \
	find_operand find_variable free_link free_list gen_block_code gen_dot_block gen_dot_clause \
	gen_fixup_code gen_forall_code gen_instr gen_local_call gen_lookup_tables gen_remote_call \
	gen_stat_code gen_the_dbs gen_when_code generate_code generate_intel_code get_op_string \
	get_type get_weight gi_block_code gi_bool_code gi_forall_code gi_generate_data gi_operand \
	gi_registers gi_remote_call gi_stat_code gi_when_code list_product new_array_clause \
	new_binary_clause new_branch_clause new_call_clause new_const_clause new_for_clause \
	new_forall_clause new_if_clause new_level new_param_list new_test_clause new_unary_clause \
	new_when_clause new_while_clause xxx_label xxx_service_level 

CFLAGS = -ansi -pedantic

HDRNAMES = codetree.h intel.h cople.h

SOURCES = $(addsuffix .c,$(SRCNAMES))
OBJECTS = $(addsuffix .o,$(SRCNAMES))


build:	cople.tab.c $(OBJECTS)
	$(CC) -o cople -ansi cople.c cople.tab.c $(OBJECTS)

cople.tab.c:
	$(YACC) -d -b cople cople.y

clean:
	rm *.o

