/* Resuelve el problema de los cántaros:
    Se tienen dos cántaros, no aforados (sin marcas de medición),
    El cántaro A tiene 3 litros de capacidad, y B 4 litros. Tambien hay una pila abierta, y vale mojar el piso.
    Al principio ambos cántaros estan vacíos.
    Hay que llenar el cantaro B con exactamente 2 litros.
*/

#include <string>
#include <vector>
#include <iostream>

#define IA_NO_CRONOMETRAR
#include "../include/ia_ciega.h"

using namespace std;
using namespace ia;

// Capacidad de cada cántaro
const unsigned int MAX_A = 3;
const unsigned int MAX_B = 4;

struct Estado {
   // indica la cantidad de agua en el cantaro A y B
   unsigned int a, b; 
   Estado() {}
   Estado( int a_, int b_ ) : a(a_), b(b_) {}
   bool es_meta() const { 
      return b == 2;
   }
   bool operator<( const Estado& otro ) const {
      return (a * 10 + b) < (otro.a * 10 + otro.b);
   }
   friend ostream& operator<<(ostream& os, Estado& e ) {
      os << "A con " << e.a << " litros; B con " << e.b << " litros.";
      return os;
   }
};

bool vaciarAaB( const Estado& antes, Estado& despues ) {
      if ( ! antes.a || antes.a + antes.b > MAX_B ) // Si A esta vacío o rebalsaría B
      return false;
   despues.b = antes.b + antes.a;
   despues.a = 0;
   return true;
}

bool vaciarBaA(const Estado& antes, Estado& despues ) {
   if ( ! antes.b || antes.a + antes.b > MAX_A ) // Si B esta vacío o rebalsaría A
      return false;
   despues.a = antes.b + antes.a;
   despues.b = 0;
   return true;
}

bool llenarA(const Estado& antes, Estado& despues ) {
   if ( antes.a == MAX_A )
      return false;
   despues.a = MAX_A;
   despues.b = antes.b;
   return true;
}

bool llenarB(const Estado& antes, Estado& despues ) {
   if ( antes.b == MAX_B )
      return false;
   despues.a = antes.a;
   despues.b = MAX_B;
   return true;
}


// Verter de A a B hasta llenar B
bool verterAllenandoB(const Estado& antes, Estado& despues ) {
   if ( antes.b + antes.a < MAX_B )
      return false;
   despues.a = antes.a - ( MAX_B - antes.b );
   despues.b = MAX_B;
   return true;
}

// Verter de B a A hasta llenar A
bool verterBllenandoA(const Estado& antes, Estado& despues ) {
   if ( antes.a + antes.b < MAX_A )
      return false;
   despues.a = MAX_A;
   despues.b = antes.b - ( MAX_A - antes.a);
   return true;
}

// Vaciar A al piso
bool vaciarA(const Estado& antes, Estado& despues ) {
   if ( ! antes.a )
      return false;
   despues.a = 0;
   despues.b = antes.b;
   return true;
}

// Vaciar B al piso
bool vaciarB(const Estado& antes, Estado& despues ) {
   if ( ! antes.b )
      return false;
   despues.a = antes.a;
   despues.b = 0;
   return true;
}   


int main() {
   Operaciones<Estado> operaciones;
   NombresOperadores<Estado> nombres;
   operaciones.push_back( &vaciarAaB ); nombres[vaciarAaB] = "Vaciar de A a B";
   operaciones.push_back( &vaciarBaA ); nombres[vaciarBaA] = "Vaciar de B a A";
   operaciones.push_back( &llenarA ); nombres[llenarA] = "Llenar A";
   operaciones.push_back( &llenarB ); nombres[llenarB] = "Llenar B";
   operaciones.push_back( &verterAllenandoB ); nombres[verterAllenandoB] = "Verter de A a B hasta llenar B";
   operaciones.push_back( &verterBllenandoA ); nombres[verterBllenandoA] = "Verter de B a A hasta llenar A";
   operaciones.push_back( &vaciarA ); nombres[vaciarA] = "Hechar todo A al piso";
   operaciones.push_back( &vaciarB ); nombres[vaciarB] = "Hechar todo B al piso";
   
   Estado inicial(0,0); // Al principio, ambos cántaros estan vacíos
   
   cout << "\nPor preferencia en profundidad:\n";
   mostrar_solucion( inicial, preferencia_profundidad( operaciones, inicial), nombres);
   mostrar_estadisticas();
   
   cout << "Por profundidad limitada:\n";
   mostrar_solucion( inicial, profundidad_limitada(operaciones, inicial, 9), nombres );   
   mostrar_estadisticas();
   
   cout << "\nPreferencia por amplitud:\n";
   mostrar_solucion( inicial, preferencia_amplitud( operaciones, inicial ), nombres );
   mostrar_estadisticas();
   
   cout << "\nProfundidad iterativa:\n";
   mostrar_solucion( inicial, profundidad_iterativa( operaciones, inicial, 9), nombres );
   mostrar_estadisticas();
   
   
   system("PAUSE");
}
