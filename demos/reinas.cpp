/* Resuelve el problema de las ocho reinas, mostrando todas las soluciones posibles:
   Colocar ocho reinas en un tablero de ajedrez de manera que ninguna este amenazada por otra.
   En vez de una función por operador, se usa un objeto función en cuyo constructor se establece
   en que fila tiene que poner la reina.
*/
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

//#define IA_NO_CRONOMETRAR

#include "../include/ia_ciega.h"

using namespace std;
using namespace ia;

// Tamaño del tablero (= cantidad de reinas)
const unsigned int TAM = 8;

// v[3].first = fila donde está la 4ta reina, v[3].second = columna donde esta la 4ta reina
typedef vector< pair<unsigned int, unsigned int> > VectorReinas;

// Este es el estado
struct Tablero {
   // La posición en que está cada reina
   VectorReinas reinas;
   Tablero() {}
   bool es_meta() const {
      return reinas.size() == TAM; 
   }
   bool operator<(const Tablero& otro) const {
      return lexicographical_compare( reinas.begin(), reinas.end(), otro.reinas.begin(), otro.reinas.end() );
   }
   friend ostream& operator<<(ostream& os, const Tablero& t ) {
      os << "[ ";
      for ( VectorReinas::const_iterator r = t.reinas.begin(); r != t.reinas.end(); ++ r )
         os << "(" << r->first << "," << r->second << ") ";
      os << "]";
      return os;
   }
};

// Pone una reina en la fila dada a su constructor, si ya hay, trata de moverla hacia la derecha
class PonerReina : public Operacion<Tablero> {
public:
   unsigned int fila; // Fila en la que se debe poner la reina
   PonerReina(const unsigned int& fila_) : fila( fila_ ) {
      ostringstream os; os << "Mover reina fila: " << fila; nombre = os.str();
   }
   // Mueve la fila-sima reina al siguiente cuadrado de la derecha
   bool operator()(const Tablero& antes, Tablero& despues ) const {
      if ( antes.reinas.size() == fila ) {
         despues.reinas = antes.reinas;
         despues.reinas.push_back( make_pair(fila,0) );
      }else if ( antes.reinas.size() == fila + 1) {
         despues.reinas = antes.reinas;
         despues.reinas.back().second ++;
      }else
         return false;
      // Ir recorriendo la ultima reina a la derecha hasta encontrar una casilla no amenazada
      bool amenazada;
      
      pair<unsigned int, unsigned int>& ultima = despues.reinas.back();
      do {
         amenazada = false;
         for ( VectorReinas::const_iterator reina = despues.reinas.begin(); reina != despues.reinas.end() - 1;  ++ reina )
            if ( reina->second == ultima.second // misma columna
                  || reina->second - reina->first == ultima.second - ultima.first //misma diagonal
                  || ( TAM - reina->second) - reina->first == ( TAM - ultima.second) - ultima.first ) { // misma diagonal inversa
               amenazada = true;
               ultima.second ++;
               break; // salimos del ciclo for
            }
      }while( amenazada && ultima.second < TAM);
      return ultima.second < TAM && ! amenazada;      
   }
};

typedef vector< PonerReina* > Ruta;

vector<Tablero> estados_meta;

bool registrar(const Ruta&, const Tablero& tablero) {
   estados_meta.push_back( tablero );
   return true;
}

int main() {
   typedef vector< PonerReina* > Operaciones;
   Operaciones operaciones;
   for ( int fila = 0; fila < TAM; ++ fila )
      operaciones.push_back( new PonerReina( fila ) );
   
   Tablero inicial;
   
   cout << "\nPor profundidad limitada:\n";

   profundidad_limitada( operaciones, inicial, 25, &registrar);
   
   mostrar_estadisticas();
   cout << "\nSe encontraron " << estados_meta.size() << " soluciones:\n";
   for( unsigned int i = 0; i < estados_meta.size(); ++ i )
      cout << estados_meta[i] << "\n";
   estados_meta.clear();

   // Liberar la memoria del vector de operaciones
   for ( int fila = 0; fila < TAM; ++ fila )
      delete operaciones[fila];
}
