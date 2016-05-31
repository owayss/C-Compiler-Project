%{
    #include <iostream>
    #include <stdlib.h>
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

%token NUMBER ID INT EOL ASSIGN MUL_ASSIGN DIV_ASSIGN ADD_ASSIGN SUB_ASSIGN OR_OP AND_OP EQ_OP NE_OP LE_OP GE_OP IF ELSE
%type<num> NUMBER
%type<name> ID ASSIGN MUL_ASSIGN DIV_ASSIGN ADD_ASSIGN SUB_ASSIGN assignment_operator OR_OP AND_OP EQ_OP NE_OP LE_OP GE_OP relational_operator equality_operator
%type<nod> function_definition parameter_type_list parameter_list compound_statement block_item_list block_item declaration statement expression_statement expression assignment_expression or_expression and_expression equality_expression relational_expression additive_expression multiplicative_expression unary_expression
%start input

%%

input: /* empty rule */
| function_definition compound_statement {limpiar_contexto()} input
;

function_definition: type ID '(' parameter_type_list ')' {}
| parameter_list {}
;

parameter_type_list: /* void */
| parameter_list {}
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
| statement {cout << "Res = " << $1->eval();}
;


statement: expression_statement {$$ = $1;}
| selection_statement {}
/*| iteration_statement {}*/
;


selection_statement: IF '(' expression ')' statement
| IF '(' expression ')' statement ELSE statement
;


expression_statement: EOL
| expression EOL
;

expression: assignment_expression
| expression ',' assignment_expression
;

assignment_expression: or_expression
| ID assignment_operator assignment_expression {$$ = new AssignmentNode(*$1, *$2, $3->eval());}
;

or_expression: and_expression
| or_expression OR_OP and_expression {$$ = new LogicalNode($1, *$2, $3); }
;


and_expression: equality_expression
| and_expression AND_OP equality_expression {$$ = new LogicalNode($1, *$2, $3); }
;

equality_expression: relational_expression
| equality_expression equality_operator relational_expression {$$ = new RelationalNode($1, *$2, $3);}
;

relational_expression: additive_expression
| relational_expression relational_operator additive_expression {$$ = new RelationalNode($1, *$2, $3);}
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
;


assignment_operator: ASSIGN
| MUL_ASSIGN
| DIV_ASSIGN
| ADD_ASSIGN
| SUB_ASSIGN
;

relational_operator: LE_OP
| GE_OP
| '<' {$$ = new string("<");}
| '>' {$$ = new string(">");}
;

equality_operator: EQ_OP
| NE_OP
;

unary_expression: NUMBER {$$ = new NumNode($1);}
| '-' NUMBER {$$ = new NumNode(-$2);}
| ID {$$ = new NumNode(table[*$1]);}
| '(' or_expression ')' {$$ = $2;}
| '!' ID {$$ = (table[*$2] == 0) ? new NumNode(1):new NumNode(0);}

%%

int main()
{

    yyparse();

    /*cout << endl << "hey! howdy? table[n] = " << table["n"] << endl;*/
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
