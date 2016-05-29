#ifndef TABLE_VARIABLES_H
#define TABLE_VARIABLES_H

#include<iostream>
#include<map>
#include<string>

using namespace std;

class TableVariables{
public:
  TableVariables(){} //Constructor
  void insertVar(string& name, int posReg, int value); //Inserta variable en el map.
  bool checkVar(string& name); //Compueba si el nombre de una variable está en la lista.
  int getPosition(string& name); //Devuelve la posición de la variable (Ejem: -4, -8, -2,...)
  int getValue(string& name); //Devuelve el valor de la variable.
  void clearTable(); //No la elimina, la vacia.

private:
  map<string, pair<int, int> > localTable;
  //<nombre, <posicion, valor>
};

#endif
