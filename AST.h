#ifndef AST_H
#define AST_H

#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <string.h>
#include <stdio.h>

using namespace std;

extern string prologue = "";
extern string body = "";
extern map <string, int> table;
/************* clase abstracta *************************/

class Node {
 public:
  virtual int eval(void) = 0;
} ; // fin de la clase abstracta Node

/************ clases derivadas *************************/

class NumNode : public Node {
 private:
  int num;
 public:
  NumNode(int n) : num(n) {}

  int eval(void) {
    return num;
  }
} ;





class ArithmeticNode : public Node {
private:
  char type;
  Node *l, *r;
public:
  ArithmeticNode(char type, Node * left, Node* right) : type(type), l(left), r(right) {}
  int eval(void) {
    int left_operand = l->eval();
    int right_operand = r->eval();

    this->generate_assembly(left_operand, right_operand);

    switch(type) {
      case '+': return left_operand + right_operand; break;
      case '-': return left_operand - right_operand; break;
      case '*': return left_operand * right_operand; break;
      case '/': return left_operand / right_operand; break;
      default: printf("internal error: bad node %c\n", type);
    }
  }

  void generate_assembly(int left_operand, int right_operand) {
    char buf[100];

    switch(type) {
      case '+':
        sprintf(buf, "\tmovl $%d, %%eax\n\tmovl $%d, %%ebx\n\taddl %%eax, %%ebx\n\n", left_operand, right_operand);
        break;
      case '-':
        sprintf(buf, "\tmovl $%d, %%eax\n\tmovl $%d, %%ebx\n\tsubl %%eax, %%ebx\n\n", left_operand, right_operand);
        break;
      case '*':
        sprintf(buf, "\tmovl $%d, %%eax\n\tmovl $%d, %%ebx\n\timul %%eax, %%ebx\n\n", left_operand, right_operand);
        break;
      case '/':
        sprintf(buf, "\tmovl $%d, %%eax\n\tcltd\n\tmovl $%d, %%ebx\n\tidiv %%ebx\n\n", left_operand, right_operand);
        break;
    }

    body.append(buf);
  }
} ;


class AssignmentNode : public Node {
 private:
  string id;
  int value;
 public:
  AssignmentNode(string id_, string op, int v) : id(id_) {
    Node * n;
    if (!op.compare("=")) {
      table[id] = v;
    }
    else if (!op.compare("*=")) {
      n = new ArithmeticNode('*', new NumNode(table[id]), new NumNode(v));
      table[id] = n->eval();
    }
    else if (!op.compare("/=")) {
      n = new ArithmeticNode('/', new NumNode(table[id]), new NumNode(v));
      table[id] = n->eval();
    }
    else if (!op.compare("+=")) {
      n = new ArithmeticNode('+', new NumNode(table[id]), new NumNode(v));
      table[id] = n->eval();
    }
    else if (!op.compare("-=")) {
      n = new ArithmeticNode('-', new NumNode(table[id]), new NumNode(v));
      table[id] = n->eval();
    }

    value = table[id];
  }

  int eval(void) {
    this->generate_assembly();
    return value;
  }

  void generate_assembly() {
    char buf[100];
    sprintf(buf, "\tmovl $%d, %%eax\n\tmovl %%eax, %s\n\n", value, id.c_str());

    body.append(buf);
  }

} ;

class LogicalNode : public Node {
private:
  string op;
  Node *l, *r;
public:
  LogicalNode(Node * left, string logical_op, Node* right) : l(left), op(logical_op), r(right) {}
  int eval(void) {
    int left_operand = l->eval();
    int right_operand = r->eval();

    this->generate_assembly(left_operand, right_operand);

    if (!op.compare("||"))
      return left_operand || right_operand;
    else if (!op.compare("&&"))
      return left_operand && right_operand;

  }

  void generate_assembly(int left_operand, int right_operand) {
    char buf[100];
    body.append(buf);
  }
} ;


class RelationalNode : public Node {
private:
  string op;
  Node *l, *r;
public:
  RelationalNode(Node * left, string logical_op, Node* right) : l(left), op(logical_op), r(right) {}
  int eval(void) {
    int left_operand = l->eval();
    int right_operand = r->eval();

    this->generate_assembly(left_operand, right_operand);

    if (!op.compare("=="))
      return left_operand == right_operand;
    else if (!op.compare("!="))
      return left_operand != right_operand;
    else if (!op.compare("<"))
      return left_operand != right_operand;
    else if (!op.compare(">"))
      return left_operand != right_operand;
    else if (!op.compare("<="))
      return left_operand != right_operand;
    else if (!op.compare(">="))
      return left_operand != right_operand;

  }

  void generate_assembly(int left_operand, int right_operand) {
    char buf[100];
    body.append(buf);
  }
} ;
#endif
