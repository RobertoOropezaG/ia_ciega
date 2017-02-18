/** Halla la salida a un laberinto.

*/


#define IA_NO_CRONOMETRAR
#include "../include/ia_ciega.h"

using namespace std;
using namespace ia;
   

// se podría cargar el laberinto de un archivo de texto facilmente...
const unsigned int MAX_COLUMNAS = 73, MAX_FILAS = 47;
const unsigned int FILA_INICIO = 0, COLUMNA_INICIO = 1, FILA_FIN = 46, COLUMNA_FIN = 71;

const char* laberinto[] = {
"X XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
"X       X       X     X     X         X X   X   X   X     X   X         X",
"XXX XXXXX XXX XXX XXX X XXX X XXX XXX X X X X X X X X XXX X X X XXXXX XXX",
"X   X     X X X   X   X X     X X   X X   X   X X X   X X   X   X   X   X",
"X XXX X XXX X X XXX XXX X XXXXX XXX X XXXXXXX X XXXXXXX XXXXXXXXX X XXX X",
"X X   X     X X   X     X       X   X   X X   X       X   X       X X   X",
"X X XXXXXXXXX XXX XXXXXXXXXXXXXXX XXXXX X X XXXXXXX X XXX X XXXXXXX X X X",
"X     X X     X           X   X   X X     X     X   X     X   X   X X X X",
"X XXX X X XXX XXXXXXXXXXXXX X X XXX X XXXXXXXXX X XXXXXXX XXX X XXX X X X",
"X   X   X   X       X     X X   X   X X       X X     X   X   X     X X X",
"XXXXXXXXXXX XXXXXXX X XXX X XXXXX XXX XXX XXX X XXXXX XXXXX XXXXX XXX XXX",
"X       X   X   X   X X X X         X   X   X   X X         X   X   X   X",
"X X XXX X XXXXX X XXX X X XXX XXXXX XXX XXXXX XXX X XXXXXXXXX X XXXXX X X",
"X X   X         X     X X     X       X         X X     X   X X X   X X X",
"X XXXXXXXXXXXXXXXXXXXXX XXXXX XXXXXXXXXXXXXXXXX X XXXXX X X X X X X X X X",
"X     X         X X   X X   X X     X       X X       X X X   X   X   X X",
"XXXXX XXX XXX X X X X X X X X X XXX X XXXXX X XXXXXXX X XXX XXXXXXXXXXX X",
"X     X   X   X     X   X X   X X     X     X   X   X X   X X         X X",
"X XXXXX XXXXXXXXXXXXXXXXXXXXXXX XXXXXXX XXXXXXX X X X XXX XXX X XXXXX X X",
"X X     X     X   X           X   X   X   X   X   X X   X X   X     X   X",
"X XXX XXX XXX X X X XXXXXXX X XXX X XXXXX X XXXXX X XXX X X XXXXXXX XXXXX",
"X   X   X X   X X X       X X   X X   X X   X   X X X X X     X   X X   X",
"XXX XXX X XXXXX X X XXXXX X XXX X X X X XXX X X X X X X XXXXX X X X XXX X",
"X       X X     X   X X   X   X X   X     X X X X X X X X   X   X X   X X",
"X XXXXX X X XXXXXXXXX X XXXXXXX XXXXXXXXX X X X X X X X X X X XXXXX X X X",
"X X   X X   X           X     X           X   X X X     X X X X   X X   X",
"X XXX XXXXXXX XXXXXXXXXXX XXX XXXXXXXXXXXXXXXXX XXXXXXXXX X XXX X X XXXXX",
"X   X         X X   X   X X X         X       X   X       X X   X   X   X",
"X X XXXXX XXXXX X X XXX X X X XXX XXXXX X XXXXX X X XXXXXXX X XXXXXXX X X",
"X X     X       X X     X X X X   X   X X     X X X   X   X X         X X",
"XXXXXXX XXXXXXX X XXXXXXX X X X XXX X XXX XXX X X XXX X X X XXXXXXXXXXX X",
"X       X         X   X   X   X     X   X X     X     X X   X X     X   X",
"X XXXXXXXXXXXXXXXXXXX X XXXXXXXXX XXXXX XXX XXXXXXXXXXX XXXXX X XXX X XXX",
"X                 X   X X   X   X     X     X     X   X       X X   X   X",
"XXXXXXX XXXXXXXXX X XXX X X X X XXXXXXXXXXXXX X XXX X XXX XXXXX X XXXXX X",
"X     X   X     X X   X   X   X         X   X X     X   X     X X X X   X",
"X XXX XXX X XXXXX XXX X XXXXXXXXXXXXX X XXX X XXXXXXXXX XXXXX X X X X X X",
"X   X X   X       X   X X       X   X X       X     X   X   X   X X X X X",
"XXX X XXXXX XXXXXXX XXXXX XXX X X X X XXXXXXXXXXXXX X X XXX XXXXX X X X X",
"X   X     X     X   X X   X   X X X X X   X   X   X   X   X     X   X X X",
"X XXXXXXX XXXXX X XXX X XXX XXX XXX X X X X X X X XXXXXXX X XXXXXXX X X X",
"X X           X     X   X X X     X X   X X X   X         X X     X X X X",
"X XXXXXXXXXXXXXXXXX X XXX X XXXXX X XXXXX X XXXXXXXXXXXXXXX X XXX XXX X X",
"X   X         X     X X   X     X X   X   X   X X         X     X   X X X",
"X X X XXXXXXX X XXXXX X X XXX X XXX X X XXXXX X X XXXXX X XXXXXXXXX X X X",
"X X   X         X     X X     X     X       X     X     X             X X",
"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX X"
};

struct Estado {
   //      fila     , columna 
   pair<unsigned int, unsigned int> posicion;
   Estado() {}
   Estado( pair<unsigned int, unsigned int> pos ) : posicion(pos) {}
   bool operator<( const Estado& otro ) const {
      return posicion < otro.posicion;
   }
   /// Muestra el estado en un stream
   friend ostream& operator<<( ostream& os, const Estado& e) {
      os << "(" << e.posicion.first << "," << e.posicion.second << ") ";
      return os;
   }
};

bool arriba( const Estado& antes, Estado& despues ) {
   if ( antes.posicion.first == 0 || laberinto[antes.posicion.first - 1][antes.posicion.second] != ' ' ) 
      return false;
   despues.posicion.first = antes.posicion.first - 1;
   despues.posicion.second = antes.posicion.second;
   return true;
}

bool abajo( const Estado& antes, Estado& despues ) {
   if ( antes.posicion.first == MAX_FILAS - 1 || laberinto[antes.posicion.first + 1][antes.posicion.second] != ' ' )
      return false;
   despues.posicion.first = antes.posicion.first + 1;
   despues.posicion.second = antes.posicion.second;
   return true;
}

bool izquierda( const Estado& antes, Estado& despues) {
   if ( antes.posicion.second == 0 || laberinto[antes.posicion.first][antes.posicion.second - 1] != ' ' )
      return false;
   despues.posicion.first = antes.posicion.first;
   despues.posicion.second = antes.posicion.second - 1;
   return true;
}

bool derecha( const Estado& antes, Estado& despues) {
   if ( antes.posicion.second == MAX_COLUMNAS - 1 || laberinto[antes.posicion.first][antes.posicion.second + 1] != ' ' )
      return false;
   despues.posicion.first = antes.posicion.first;
   despues.posicion.second = antes.posicion.second + 1;
   return true;
}

int main() {
   Operaciones<Estado> operaciones;
   NombresOperadores<Estado> nombres;
   OperadoresInversos< Operaciones<Estado> > inversas;
   
   operaciones.push_back( &izquierda ); nombres[ &izquierda ] = "Izq"; inversas[ &izquierda ] = &derecha;
   operaciones.push_back( &derecha ); nombres[ &derecha ] = "Der"; 
   operaciones.push_back( &arriba ); nombres[ &arriba ] = "Arr"; inversas[ &arriba ] = &abajo;
   operaciones.push_back( &abajo ); nombres[ &abajo ] = "Aba";
   
   Estado inicial( make_pair(FILA_INICIO, COLUMNA_INICIO) );
   Estado final( make_pair(FILA_FIN, COLUMNA_FIN) );

   mostrar_solucion( inicial
                      ,simplificar_bidireccional( profundidad_limitada_doble( operaciones, inicial, final, 500 ), inversas )
                      ,nombres );

   /* Si usas MS Visual Studio .NET 2003, tendrás que usar estas cuatro lineas en vez de las anteriores tres.
   pair< vector< Operaciones<Estado>::value_type >, vector< Operaciones<Estado>::value_type > > solucion;
   solucion = profundidad_limitada_doble( operaciones, final, inicial, 500);
   vector< Operaciones<Estado>::value_type> solucion_simple = simplificar_bidireccional( solucion, inversas.complementar() );
   mostrar_solucion( final, solucion_simple, nombres );*/
   
   mostrar_estadisticas();
}
