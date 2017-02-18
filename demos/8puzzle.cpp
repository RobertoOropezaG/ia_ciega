/* Resuelve el 8-puzzle
*/

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cassert>

//#define IA_NO_CRONOMETRAR
#include "../include/ia_ciega.h"

using namespace std;
using namespace ia;

const int ANCHO = 3;
const int ALTO = 3;

// matrizFicha[2][1] = 6 quiere decir que en la fila 3 columna 2 está la ficha #6
typedef int MatrizFicha[ALTO][ANCHO];

// Este es el estado
struct Puzzle {
   MatrizFicha ficha;
   Puzzle() {}
   Puzzle(bool tonta) { // El parametro tonto hace que se pongan las fichas en el puzzle
      int numero = 0;
      for ( int fila = 0; fila < ALTO; ++ fila )
         for ( int col = 0; col < ANCHO; ++ col )
            ficha[fila][col] = ++numero;
      ficha[ALTO-1][ANCHO-1] = 0; // La ultima queda vacía
   }
   bool operator<(const Puzzle& otro) const {
      return lexicographical_compare( ficha[0], ficha[0] + ALTO * ANCHO, otro.ficha[0], otro.ficha[0] + ALTO * ANCHO);
   }
   bool es_meta() const {
      int numero = 0;
      for ( int fila = 0; fila < ALTO; ++ fila )
         for ( int col = 0; col < ANCHO; ++ col )
            if ( ficha[fila][col] != ++numero && numero != ANCHO*ALTO)
               return false;
      return true;
   }
   friend ostream& operator<<(ostream& os, const Puzzle& t ) {
      os << "\n";
      for( int fila = 0; fila < ALTO; ++ fila ) {
         os << "[";
         for( int col = 0; col < ANCHO; ++ col )
            os << setw(4) << (unsigned int)t.ficha[fila][col];
         os << "]\n";
      }
      return os;
   }
};

struct Mover : public Operacion<Puzzle> {
   enum Direccion {ARRIBA, DERECHA, ABAJO, IZQUIERDA};
   Direccion dir;
   unsigned int fila_orig, col_orig; // posicion desde la que se mueve la ficha
   int fila_des, col_des; // posición a la que llega
   // Para inicializar se dice la posición inicial de la ficha y la dirección del movimento
   Mover( const unsigned int& fila_, const unsigned int& col_, const Direccion& dir_ ) : fila_orig( fila_ ), col_orig( col_ ), dir( dir_ ) {
      switch ( dir ) {
         case ARRIBA:
            assert( fila_orig != 0 );
            fila_des = fila_orig - 1;            col_des = col_orig;
            break;
         case DERECHA:
            assert( col_orig != ANCHO - 1 );
            fila_des = fila_orig;                col_des = col_orig + 1;
            break;
         case ABAJO:
            assert( fila_orig != ALTO - 1 );
            fila_des = fila_orig + 1;          col_des = col_orig;
            break;
         case IZQUIERDA:
            assert( col_orig != 0 );
            fila_des = fila_orig;               col_des = col_orig - 1;
            break;
      }
   }
   bool operator()( const Puzzle& antes, Puzzle& despues ) const {
      if ( antes.ficha[fila_orig][col_orig] == 0 || antes.ficha[fila_des][col_des] != 0 )
         return false;
      despues = antes;
      despues.ficha[fila_des][col_des] = antes.ficha[fila_orig][col_orig];
      despues.ficha[fila_orig][col_orig] = 0;
      return true;
   }
};


int main() {
   Puzzle a, b(true);
   typedef vector<Mover*> Movimientos;
   Movimientos operaciones;

   // Generar las operaciones
   for( int fila = 0; fila < ALTO; ++ fila )
      for( int col = 0; col < ANCHO; ++ col ) {
         if ( fila != 0 )
            operaciones.push_back( new Mover( fila, col, Mover::ARRIBA ) );
         if ( col != ANCHO - 1 )
            operaciones.push_back( new Mover( fila, col, Mover::DERECHA ) );
         if ( fila != ALTO - 1 )
            operaciones.push_back( new Mover( fila, col, Mover::ABAJO ) );
         if ( col != 0 )
            operaciones.push_back( new Mover( fila, col, Mover::IZQUIERDA ) );
      }

   Puzzle ordenado(true), inicial;
   
   // Mezclar el puzzle
   cout << "Mezclando...\n";
   Movimientos mezcolanza( operaciones );
   for ( unsigned long i = 0; i < 100; ++ i )
      copy( operaciones.begin(), operaciones.end(), back_inserter( mezcolanza ) );
   set<Puzzle> intermedios; // se usa para que no se retroceda al mezclar
   intermedios.insert( ordenado );
   for( int paso = 0; paso < 10; ++ paso ) {
      random_shuffle(mezcolanza.begin(), mezcolanza.end() );
      for ( int i = 0; i < mezcolanza.size(); ++ i )
         if ( (*mezcolanza[i])(ordenado, inicial) && intermedios.find(inicial) == intermedios.end() ) {
            intermedios.insert( inicial );
            ordenado = inicial;
         }
   }
   cout << "Se mezclo con " << intermedios.size() << " movidas\n";
   intermedios.clear();
   cout << inicial;
   
   mostrar_solucion( inicial, profundidad_limitada(operaciones, inicial,30) );
   mostrar_estadisticas();
}
