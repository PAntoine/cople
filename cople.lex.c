/* $Revision: 1.2 $
 *			COPLE LEX
 *
 *	This is the lex function for the cople compiler. It handles all the pre-processing
 *	of the inputs. It also throws away the comments here, and does not pass them on
 *	to the yyparse function. This function is designed to be called from the yyparse
 *	file generated from the BISON file cople.y.
 *
 *			P.Antoine.
 *
 */

#include	<math.h>
#include	<ctype.h>
#include	<stdio.h>
#include	"cople.h"
#include	"codetree.h"
#include	"cople.tab.h"

/* Note: 	if the reserved word list is changed the corresponding list in the cople.y
 *	file must also be changed. The reserved words from "cople" to "false"
 *	are in the same order as they are in the cople.y file. The order is used
 *	to tie the word form of the operators with the character forms plus the
 *	special words.
 */

char	*reserved[] = { "cople","application","object","system",
		"lt", "le", "gt", "ge", "eq", "ne", ".", "and","or", "not", "xor", "mod",
		"if", "then", "else", "elseif", "fi", "while", "forall", "for",
		"begin", "end", "whenever", "ref", "int", "char", "boolean", "real", "instance", "string", "void", 
 		"mandatory", "struct",
		"new", "delete", "of", "with", "true", "false"
		};


#define	RESERVED COPLE 
#define	num_of_reserved (FALSE - COPLE)  

int	yylex()
{
	extern FILE	*input_file;
	extern int	lineno;
	char	c, operator = '\0',nextc;
	char	*space = yylval.char_string;
	int	finished = 0,type = 0, count=0,kount=0;
	int	failed = 0;


	/* If the first character is the end of file, then the input should 
	 * end. Also remove any leading delimiters.
	 */

	c = getc(input_file);
	
	while( c == ' ' || c == '\t' || c == '\n' ){
		if (c == '\n')
			lineno++;
		c = getc(input_file);
	}
	
	if (c == EOF)
		return -1;

	/* get a token that is delimited by a space, tab, or newline. If there
	 * is an operator then the token also ends.
	 */
	 
	while( c != EOF && !finished )
	{
		switch(c)
		{
		case '\n':	lineno++;
		case '\t':
		case ' ':	space[count] = '\0';
			finished = 1;
			break;
		
		case '?':	if (count == 0){
				/* Comments use the comment char to the end of line style
				 * of comment. This makes it less likely that a comment
				 * will start and not be ended which makes life easy.
				 */
				c = getc(input_file);
				while ( (c != '?') && (c != EOF) && (c != '\n'))
				{
					c = getc(input_file);
				}

				if (c == '\n')
					lineno++;
				
				c = getc(input_file);
				
				/* Remove all the blank lines after a comment so that the
				 * parser can carry on Ok.
				 */
				while( c == ' ' || c == '\t' || c == '\n' ){
					c = getc(input_file);
					if (c == '\n')
						lineno++;
				}
	
				if (c == EOF)
					return -1;
			} else{
				/* Comments follow after a normal statement - or phrase so
				 * it is used as an operator that ends the current input.
				 */

				ungetc(c,input_file);
				space[count] = '\0';
				finished = 1;
			}
			break;
		
		case '<':
		case '>':
		case '%':	
		case '{':
		case '}':
		case '\"':
		case '\'':
		case ',':
		case '@':
		case '!':
		case ';':
		case '$':
		case '#':
		case ':':
		case '*':
		case '+':
		case '-':
		case '/':
		case '(':
		case ')':
		case '[':
		case ']':
		case '=':
		case '&':
		case '.':	space[count] = '\0';
			operator = c;
			finished = 1;
			break;

		default:	space[count++] = c;
			c = getc(input_file);
			break;
		};
	};

	finished = 0;

	if (count != 0)
	{
		if (operator != '\0'){
			ungetc(operator,input_file);
			operator = '\0';
		};

		/* First character is an alpha char so it must be either
		 * an reserved word or a valid_name.
		 */
		 
		if (isalpha(space[0])){
			/* check for reseved words. The type of the reserved word
			 * is simply the position in the array + the first reserved
			 * in cople.y i.e. COPLE.
			 */
			for (kount=0; kount < num_of_reserved && !finished; kount++)
			{
				if (strcmp(space,reserved[kount]) == 0)
				{
					type = RESERVED + kount;
					finished = 1;
				};
			};
	
			/* if !finished then it was not a RESERVED word.
			 */
			if ( !finished )
			{
				/* vaild names must be [a-zA-Z][a-zA-Z0-9_]* and
				 * will be no longer than 32 bytes.
				 */

				type = VALID_NAME;
				for (kount=0; kount<strlen(space) && !finished;kount++)
				{
					if (!isalnum(space[kount]))
						if (space[kount] != '_'){
							finished = 1;
							failed = 1;
							type = 0;
						};
				};

				if (kount > 32){
					type = 0;
					failed = 1;
				};
			};

		} else if (isdigit(space[0])){
			/* check for numbers only looking for intergers floats are built
			 * inside the parser.
			 */
			type = INTEGER;
			kount = strlen(space);
			count = 0;

			while( isdigit(space[count]) && count < kount )
			{
				count++;
			};

			if (space[count] == '\0'){
				yylval.int_number = atoi(space);
				failed = 0;
			}else
				failed = 1;
		};
	} else if (operator != '\0'){
		/* Now check for the symbolic operators - these are the ones that have
		 * a reserved word option or are multi-character or the first charator
		 * can be part (or all) of another operator.
		 */
		
		switch(operator){

		/* == is the equal sign - this is a cop out the avoid shift/reduce as = is used
		 * elsewhere in the code - and using the same op causes sr's.
		 */
		case '=':	nextc = getc(input_file);
			if (nextc == '='){
				type = EQ_OP;
				operator = '\0';
			}else
				ungetc(nextc,input_file);
			break;
		
		/* if % then needs to be MOD_OP */

		case '%':	type = MOD_OP;
			operator = '\0';
			break;
		
		/* Check for ":=" the assignment operator. */
		
		case ':':	nextc = getc(input_file);
			if (nextc  == '='){
				type = ASSIGN_OP;
				operator = '\0';
			}else
				ungetc(nextc,input_file);
			break;
		/* Check for the "<=" and the "<>" operators
		 * and if not these then return the "<" op.
		 */
	
		case '<':	nextc = getc(input_file);
			if (nextc == '='){
				type = LE_OP;
				operator = '\0';
			}else if (nextc == '>'){
				type = NE_OP;
				operator = '\0';
			}else{
				type = LT_OP;
				operator = '\0';
				ungetc(nextc,input_file);
			};
			break;
	
		/* Check for the ">=" operator, or return the ">" op. */

		case '>':	nextc = getc(input_file);
			if (nextc == '='){
				type = GE_OP;
				operator = '\0';
			}else{
				type = GT_OP;
				operator = '\0';
				ungetc(nextc,input_file);
			};
			break;

		
		/* Check for the strings i.e " .... " and just return the
		 * contents between the quotes.
		 */

		case '\"':	nextc = '\0';
		
			while ( (nextc != '\"') && (nextc != EOF) && (nextc != '\n') && (!failed))
			{
				nextc = getc(input_file);
			
				/* must check for multibyte characters */
				if (nextc == '\\')
				{
					nextc = getc(input_file);
					switch(nextc)
					{
					case '\\':
					case '\'':
					case '\"':
					case 'f':
					case 'r':
					case 't':
					case 'n':	space[count++] = nextc;
					break;
				
					default:	failed = 1;
					};
				}else
					space[count++] = nextc;
			}

			if (nextc == '\"'){
				type = STRING_CONST;
				space[--count] = '\0';
				operator = '\0';
			} else{
				printf("FAILED in string");
				failed = 1;
			}
			break;

		/* check for a character - not allowing break chars yet - in a future
		 * (less lazy) version.
		 */

		case '\'':	nextc = getc(input_file);
			
			if (nextc != EOF)
				yylval.character = nextc;

			nextc = getc(input_file);
			
			/* must check for multibyte characters */
			if (nextc == '\\')
			{
				nextc = getc(input_file);
				switch(nextc)
				{
				case '\\':
				case '\'':
				case '\"':
				case 'f':
				case 'r':
				case 't':
				case 'n':	yylval.character = nextc;
					break;
				
				default:	failed = 1;
				};
			
				nextc = getc(input_file);
			};

			if (nextc != '\'')
				failed = 1;
			else{
				type = CHARACTER;
				operator = '\0';
			};
			break;
		};	
		
	};

	if (failed)
		return 0;
	else if (operator != '\0')
		return operator;
	else
		return type;
}
