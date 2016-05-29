#include "TableVariables.h"

using namespace std;

/*Inserta variable local en localTable*/
void TableVariables::insertVar(string& name, int posReg, int value){
  pair<int, int> parAux = pair<int, int>(posReg, value);
  localTable.insert(pair<string, pair<int, int> >(name, parAux));
}

/*Comprueba que una variable local esté en el map.
  Si está devuelve 1, en otro caso 0*/
bool TableVariables::checkVar(string& name){
  map<string,pair<int, int> >::iterator it;
  if(localTable.empty()){ //Si el map está vacio
    return false;
  }
  else{
    it = localTable.find(name);
    if(it == localTable.end()){ //Si no lo encuentra en el map
      return false;
    }
    else{
      return true;
    }
  }
}

/*Devuelve la posición de la variable que está en la pila de registros*/
int TableVariables::getPosition(string& name){
  map<string,pair<int, int> >::iterator it = localTable.find(name);
  return it->second.first;
}

/*Devuelve el valor de la variable*/
int TableVariables::getValue(string& name){
  map<string,pair<int, int> >::iterator it = localTable.find(name);
  return it->second.second;
}

/*Resetea el map pero no lo destruyte*/
void TableVariables::clearTable(){
  localTable.erase(localTable.begin(), localTable.end());
}
