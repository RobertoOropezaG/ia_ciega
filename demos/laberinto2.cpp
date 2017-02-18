/** Halla la salida a un laberinto.
*/

#define IA_NO_CRONOMETRAR
#include "../include/ia_ciega.h"

using namespace std;
using namespace ia;
   
/// Luego se ajustan estos datos de acuerdo al std in
unsigned int MAX_COLUMNAS = 0, MAX_FILAS = 0;
unsigned int FILA_INICIO = 0, COLUMNA_INICIO = 0, FILA_FIN = 0, COLUMNA_FIN = 0;

typedef vector<string> Laberinto;
Laberinto laberinto;

ostream& operator<<(ostream& os, const Laberinto& lab) {
   for( unsigned int i = 0; i < lab.size(); ++ i )
      os << lab[i] << "\n";
   os << "\n"; 
   return os;
}

struct Estado {
   //      fila     , columna 
   pair<unsigned int, unsigned int> posicion;
   Estado() {}
   Estado( pair<unsigned int, unsigned int> pos ) : posicion(pos) {}
   bool operator<( const Estado& otro ) const {
      return posicion < otro.posicion;
   }
   /// Muestra  el estado en un stream
   friend ostream& operator<<( ostream& os, const Estado& e) {
      os << "(" << e.posicion.first << "," << e.posicion.second << ") ";
      return os;
   }
   /// Pone un punto en la posición de representada por este estado en el laberinto dado
   friend Laberinto& operator<<( Laberinto& lab, const Estado& e) {
      lab[e.posicion.first][e.posicion.second] = '+';
      return lab;
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

// Carga un laberinto de un archivo de texto. Es un poco larga porque verificamos que el archivo tenga el formato correcto
void cargar_laberinto() {
   string linea, linea_anterior;
   int cuenta = 0;
   while ( getline(cin, linea) ) {
      if ( ! cuenta ) { // si es la primera linea, buscamos el punto inicio
         for( unsigned int c = 0; c < linea.size(); ++ c )
            if ( linea[c] == ' ' )
               COLUMNA_INICIO = c;
      }else if ( linea.size() == 0 ){
         while ( getline(cin,linea) )
            if (linea.size() > 0 ) {
               cerr << "ERROR: La linea " << cuenta + 1 << " esta en blanco.\n";
               exit(1);
            }
         break;
      }else if ( linea.size() != linea_anterior.size() ) {
         cerr << "ERROR: La linea " << cuenta + 1 << " tiene una cantidad distinta de caracteres.\n" << "(anterior:" << linea_anterior.size() << " esta:" << linea.size() << "\n";
         exit(1);
      }
      laberinto.push_back( linea );
      linea_anterior = linea;
      cuenta++;
   }
   // buscamos el punto de la meta
   for ( unsigned int c = 0; c < linea_anterior.size(); ++ c )
      if ( linea_anterior[c] == ' ' ) {
         COLUMNA_FIN = c;
         break;
      }
   MAX_COLUMNAS = linea_anterior.size();
   MAX_FILAS = cuenta;
   FILA_FIN = MAX_FILAS - 1;
   FILA_INICIO = 0;
   if ( MAX_COLUMNAS < 3 || MAX_FILAS < 3 ) {
      cout << "Es un laberinto demasiado pequeño!!";
      exit(1);
   }else if ( COLUMNA_INICIO == 0 || COLUMNA_FIN == 0 ) {
      cout << "No se encontró el punto de partida o la meta";
      exit(1);
   }
}

int main() {
   cargar_laberinto();
   
   Operaciones<Estado> operaciones;
   NombresOperadores<Estado> nombres;
   OperadoresInversos< Operaciones<Estado> > inversas;
   
   operaciones.push_back( &izquierda ); inversas[ &izquierda ] = &derecha;
   operaciones.push_back( &derecha ); 
   operaciones.push_back( &arriba ); inversas[ &arriba ] = &abajo;
   operaciones.push_back( &abajo ); 
   
   Estado final( make_pair(FILA_INICIO, COLUMNA_INICIO) );
   Estado inicial( make_pair(FILA_FIN, COLUMNA_FIN) );
   
   cerr << "Buscando...";
   pair< vector< Operaciones<Estado>::value_type >, vector< Operaciones<Estado>::value_type > > solucion;
   try {
      // borland se cuelga con 1019 y da con 1018
      solucion = profundidad_limitada_doble( operaciones, inicial, final, 6000);
   }catch(...){
      cerr << "Excepcion";
   }
   cerr << solucion.first.size() << "," << solucion.second.size() << "\n";
   cerr << "Finalizado\n";
   vector< Operaciones<Estado>::value_type> solucion_simple = simplificar_bidireccional( solucion, inversas.complementar() );
   cerr << "Se simplifico la solucion bi-direccional a uni-direccional:\n";
   cout << solucion_simple.size() << "\n";

   SecuenciaEstados<Estado, Operaciones<Estado> > estados_intermedios(inicial, solucion_simple );
   cerr << "Se obtuvieron los estado intermedios\n";

   mostrar_solucion( inicial, solucion_simple, nombres );
   int x = 0;
   for( SecuenciaEstados<Estado, Operaciones<Estado> >::iterator it = estados_intermedios.begin(); it != estados_intermedios.end(); ++ it ) {
      cout << x << "-";
      laberinto << *it;
      cout << x ++ << " ";;
   }
   cerr << "Se anoto la solucion en el laberinto\n";
   cout << "\n\n";
   cout << laberinto;
}    
                   