#define STRING_CONST 257
#define VALID_NAME 258
#define CHARACTER 259
#define INTEGER 260
#define FLOAT 261
#define COPLE 262
#define COPLE_APP 263
#define COPLE_OBJ 264
#define COPLE_SYS 265
#define LT_OP 266
#define LE_OP 267
#define GT_OP 268
#define GE_OP 269
#define EQ_OP 270
#define NE_OP 271
#define ASSIGN_OP 272
#define AND_OP 273
#define OR_OP 274
#define NOT_OP 275
#define XOR_OP 276
#define MOD_OP 277
#define IF 278
#define THEN 279
#define ELSE 280
#define ELSEIF 281
#define FI 282
#define WHILE 283
#define FORALL 284
#define FOR 285
#define BEGIN 286
#define END 287
#define WHENEVER 288
#define REF 289
#define INT 290
#define CHAR 291
#define BOOLEAN 292
#define REAL 293
#define INSTANCE 294
#define STRING 295
#define VOID 296
#define MANDATORY 297
#define STRUCT 298
#define NEW 299
#define DELETE 300
#define OF 301
#define WITH 302
#define TRUE 303
#define FALSE 304
#define YYERROR_VERBOSE 305
typedef union{
	char	char_string[255];
	char	character;
	int	int_number;
	float	float_number;
	void*	complex_type;
	SYMBOL*	symbol_type;
	CONSTANT*	const_type;
	INST_TYPE	instr_type;
	CLAUSE*	clause;
	LIST*	list_type;
	PARAMETER	parameter;
	PARAMETER_LIST*	parameter_list;
	} YYSTYPE;
extern YYSTYPE yylval;
