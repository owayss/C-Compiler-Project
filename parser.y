%{
    #include <iostream>
    #include <stdio.h>
    #include <string>
    #include <map>
    #include <vector>
    #include "AST.h"

    using namespace std;

    map <string, int> table;

    int yyerror(const char *msg);
    int yylex(void);

%}


%union{
    int num;
    string * name;
    Node * nod;
}

%token NUMBER ID INT CHAR FLOAT EOL
%type<num> NUMBER
%type<name> ID
%type<nod> function_definition parameter_type_list parameter_list compound_statement block_item_list block_item declaration statement expression_statement additive_expression multiplicative_expression unary_expression
%start input

%%

input: /* empty rule */
| function_definition compound_statement {} input
;

function_definition: type ID '(' parameter_type_list ')' {}
;

parameter_type_list: /* void */
| parameter_list
;

parameter_list: declaration
| parameter_list ',' declaration
;

declaration: type ID {}
;

compound_statement: '{' block_item_list '}' {}
;

block_item_list: block_item
| block_item_list block_item
;

block_item: parameter_type_list EOL
| statement EOL {cout << "Res = " << $1->eval();}
;

statement: expression_statement {$$ = $1;}
/*| selection_statement {}
| iteration_statement {}*/
;

expression_statement: additive_expression {$$ = $1;}
;

additive_expression: multiplicative_expression
| additive_expression '+' multiplicative_expression {$$ = new ArithmeticNode('+', $1, $3);}
| additive_expression '-' multiplicative_expression {$$ = new ArithmeticNode('-', $1, $3);}
;

multiplicative_expression: unary_expression
| multiplicative_expression '*' unary_expression {$$ = new ArithmeticNode('*', $1, $3);}
| multiplicative_expression '/' unary_expression {$$ = new ArithmeticNode('/', $1, $3);}
;

type: INT
| CHAR
| FLOAT
;

unary_expression: NUMBER {$$ = new NumNode($1);}
| '-' NUMBER {$$ = new NumNode(-$2);}
| ID {$$ = new NumNode(table[*$1]);}

%%

int main()
{

    yyparse();

    prologue.append(".LC0:\n\t.string \"El resultado = %d\"\nmain:\n\tpushl %ebp\n\tmovl %esp, %ebp\n");
    string epilogue = "\tmovl $0, %eax\n\tmovl %ebp, %esp\n\n\tpopl %ebp\n\tret\n";

    cout << prologue << endl << body << epilogue << endl;;
    return 0;
}

int yyerror(const char *msg)
{
    cerr << msg << endl;
    return 1;
}
