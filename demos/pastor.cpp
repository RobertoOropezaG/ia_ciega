/* Resuelve el problema del pastor, el lobo, la oveja y el pasto:
   El hombre (pastor), el lobo, la oveja y el pasto (forraje) están en la orilla izquierda del rio.
   El pastor tiene una canoa en que solo cabe él y una cosa más (ya sea el lobo, la oveja o el forraje)
   Si deja solos al lobo y a la oveja, muere la oveja; 
   Si deja solos a la oveja y al forraje, chau forraje.
   Debe llevar TODO, sano y salvo, a la otra orilla.
*/

#include <string>
#include <vector>
#include <iostream>

#include "../include/ia_ciega.h"

using namespace std;
using namespace ia;

enum Orilla {IZQ = 0, DER = 1};

struct Estado {
   // Si hombre vale DER el hombre esta en la orilla derecha, 
   // si oveja vale IZQ la oveja esta en la orilla izquierda, etc.
   Orilla hombre, lobo, oveja, forraje, bote;

   Estado() {}
   Estado( Orilla hombre_, Orilla lobo_, Orilla oveja_, Orilla forraje_, Orilla bote_ )
   : hombre(hombre_), lobo( lobo_ ), oveja( oveja_ ), forraje( forraje_ ), bote( bote_ ) {
   }
   // Devuelve true ssi este estado es válido, es decir, ssi nadie se comerá a nadie
   bool valido() const {
      return !( ( lobo == oveja && hombre != lobo ) || (oveja == forraje && hombre != oveja) );
   }
   // Devuelve true ssi es un estado meta
   bool es_meta() const {
      return hombre == DER && lobo == DER && oveja == DER && forraje == DER
            && bote == DER;
   }
   // Un operador de orden debil estricto, para que pueda estar en un std::set
   bool operator<(const Estado& otro) const {
      // formamos el patron de bits correspondiente a ambos estados y los comparamos
      return ((((bote<<1 | forraje)<<1 | oveja)<<1 | lobo)<<1 | hombre)
             < ((((otro.bote<<1 | otro.forraje  )<<1 | otro.oveja)<<1 | otro.lobo)<<1 | otro.hombre);
   }
   // Devuelve una cadena con la especificación del estado
   // Solo es necesario si se quiere mostrar las soluciones con estados intermedios
   friend ostream& operator<<(ostream& os, const Estado& e ) {
      os << "(" << (e.hombre ? "Hd" : "Hi") << (e.lobo ? ", Ld" : ", Li") << (e.oveja ? ", Od" : ", Oi") << (e.forraje ? ", Fd" : ", Fi") << ")";
      return os;
   }
};


// El hombre solito a la derecha
bool h_der( const Estado& antes, Estado& despues) {
   if ( antes.hombre == DER || antes.bote == DER )
      return false;
   despues = Estado( DER, antes.lobo, antes.oveja, antes.forraje, DER );
   return despues.valido();
}

// El hombre solito a la izquierda
bool h_izq( const Estado& antes, Estado& despues) {
   if ( antes.hombre == IZQ || antes.bote == IZQ )
      return false;
   despues = Estado( IZQ, antes.lobo, antes.oveja, antes.forraje, IZQ );
   return despues.valido();
}

// el hombre y el lobo a la orilla derecha
bool hl_der(const Estado& antes, Estado& despues) {
   if ( antes.hombre == DER || antes.lobo == DER || antes.bote == DER )
      return false;
   despues = Estado( DER, DER, antes.oveja, antes.forraje, DER );
   return despues.valido();
}

bool hl_izq(const Estado& antes, Estado& despues) {
   if ( antes.hombre == IZQ || antes.lobo == IZQ || antes.bote == IZQ )
      return false;
   despues = Estado( IZQ, IZQ, antes.oveja, antes.forraje, IZQ );
   return despues.valido();
}

// El hombre y la oveja a la orilla derecha
bool ho_der(const Estado& antes, Estado& despues) {
   if ( antes.hombre == DER || antes.oveja == DER || antes.bote == DER )
      return false;
   despues = Estado( DER, antes.lobo, DER, antes.forraje, DER );
   return despues.valido();
}

bool ho_izq( const Estado& antes, Estado& despues) {
   if ( antes.hombre == IZQ || antes.oveja == IZQ || antes.bote == IZQ )
      return false;
   despues = Estado( IZQ, antes.lobo, IZQ, antes.forraje, IZQ );
   return despues.valido();
}

// El hombre y el forraje a la orilla derecha
bool hf_der( const Estado& antes, Estado& despues) {
   if ( antes.hombre == DER || antes.forraje == DER || antes.bote == DER )
      return false;
   despues = Estado( DER, antes.lobo, antes.oveja, DER, DER );
   return despues.valido();
}

bool hf_izq( const Estado& antes, Estado& despues) {
   if ( antes.hombre == IZQ || antes.forraje == IZQ || antes.bote == IZQ )
      return false;
   despues = Estado( IZQ, antes.lobo, antes.oveja, IZQ, IZQ );
   return despues.valido();
}


int main() {
   Operaciones<Estado> operaciones;
   NombresOperadores<Estado> nombres;
   operaciones.push_back( &ho_der ); nombres[&ho_der] = "Hombre y oveja a la derecha";
   operaciones.push_back( &ho_izq ); nombres[&ho_izq] = "Hombre y oveja a la izquierda";
   operaciones.push_back( &hf_der ); nombres[&hf_der] = "Hombre y forraje a la derecha";
   operaciones.push_back( &hf_izq ); nombres[&hf_izq] = "Hombre y forraje a la izquierda";
   operaciones.push_back( &h_der ); nombres[&h_der] = "Hombre a la derecha";
   operaciones.push_back( &h_izq ); nombres[&h_izq] = "Hombre a la izquierda";
   operaciones.push_back( &hl_der ); nombres[&hl_der] = "Hombre y lobo a la derecha";
   operaciones.push_back( &hl_izq ); nombres[&hl_izq] = "Hombre y lobo a la izquierda";
   
   Estado inicial(IZQ,IZQ,IZQ,IZQ,IZQ); // Todos en la orilla izquierda
   
   mostrar_solucion(inicial, preferencia_amplitud(operaciones, inicial), nombres );
   mostrar_estadisticas();
   
   mostrar_solucion(inicial, profundidad_limitada(operaciones, inicial, 18 ), nombres );
   mostrar_estadisticas();

   mostrar_solucion(inicial, profundidad_iterativa(operaciones, inicial, 18 ), nombres );
   mostrar_estadisticas();

   system("PAUSE");
}
