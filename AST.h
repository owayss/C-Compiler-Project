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

class DeclarationNode : public Node {
 private:
  string id;
  int value;
 public:
  DeclarationNode(string id_, int v) : id(id_), value(v) {}

  int eval(void) {
    this->generate_assembly();
    return value;
  }

  void generate_assembly() {
        table[id] = value;
        prologue.append("\t.comm " + id + ", 4, 4\n");
  }

} ;


// class AssignNode : public Node {
//  private:
//   string id;
//   int value;
//  public:
//   AssignNode(string name, int n) : id(name), value(n) {
//     cout << "id =" << id << "value = " << value <<  endl;
//     if ( table.find(id) != table.end() )
//       table[id] = value;
//     else if (pointers_table.find(id) != pointers_table.end())
//       pointers_table[id] = &value;
//     else
//       **pointers_pointers_table[id] = value;
//     }
//
//   int eval(void) {
//     // this->generate_assembly();
//     return value;
//   }
//
//   void generate_assembly() {
//     char buf[100];
//     sprintf(buf, "\tmovl $%d, %%eax\n\tmovl %%eax, %s\n\tmovl %s, 4(%%esp)\n\tmovl $.LC0, (%%esp)\n\tcall printf\n\n", value, id.c_str(), id.c_str());
//
//     body.append(buf);
//   }
// } ;


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


#endif
