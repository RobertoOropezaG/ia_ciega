#define IA_NO_CRONOMETRAR
#include "../include/ia_ciega.h"


#include <set>
#include <list>
#include <map>
#include <string>
#include <iostream>

using namespace std;
using namespace ia;

enum Color {ROJO, AMARILLO, VERDE, AZUL, NARANJA, CAFE, MORADO, BLANCO, TOTAL_COLORES };
enum Pais {COLOMBIA, VENEZUELA, GUYANA, SURINAM,GUAYANA, ECUADOR, PERU, BRASIL,BOLIVIA,CHILE, ARGENTINA, PARAGUAY, URUGUAY};

//typedef string Pais;
typedef map< Pais, Color> Mapa;
typedef map< Pais, set< Pais > > Adyacencia;
Adyacencia adyacencia;

// indica con cuántos colores deseamos pintar el mapa, se le asigna valores en main()
int colores_utilizables;

struct Estado {
   Mapa coloreados;
   Estado() {};
   bool operator<( const Estado& otro ) const {
      return lexicographical_compare(coloreados.begin(), coloreados.end(), otro.coloreados.begin(), otro.coloreados.end() );
   }
   bool es_meta() const {
      return coloreados.size() == adyacencia.size();
   }
   friend ostream& operator<<(ostream& os, const Estado& estado ) {
      static const char* nombre_pais[] = {"Colombia", "Venezuela", "Guyana", "Surinam", "Guayana francesa", "Ecuador", "Peru", "Brasil", "Bolivia", "Chile", "Argentina", "Paraguay", "Uruguay"};
      static const char* nombre_color[] = {"Rojo", "Amarillo", "Verde", "Azul", "Naranja", "Cafe", "Morado", "Blanco"};
      for ( Mapa::const_iterator it = estado.coloreados.begin(); it != estado.coloreados.end(); ++ it )
         cout << nombre_pais[it->first] << ": " << nombre_color[it->second] << "\n";
      cout << "------------\n";
      return os;
   }
};

// Una operacion consiste en colorear determinado país con cualquier color aceptable de entre los disponibles
struct Colorear : public Operacion<Estado>  {
   Pais pais;
   Colorear() {}
   Colorear( Pais pais_ ) : pais( pais_ ) {
   }
   
   bool operator()(const Estado& antes, Estado& despues ) const {
      Mapa::iterator ultimo;
      if ( antes.coloreados.size() == (int)pais ) {
         despues.coloreados = antes.coloreados;
         ultimo = despues.coloreados.insert( make_pair(pais,(Color)0) ).first;
      }else if ( antes.coloreados.size() == (int)pais + 1) {
         despues.coloreados = antes.coloreados;
         ultimo = despues.coloreados.find(pais);
         ultimo->second = (Color)((int)ultimo->second +1 );
      }else
         return false;

      bool aceptable;
      do {
         aceptable = true;
         // Iteramos por todos los paises adyacentes
         for( set<Pais>::const_iterator it = adyacencia[pais].begin(); it != adyacencia[pais].end(); ++ it ) {
            //cout << *it;
            Mapa::const_iterator it_pais = antes.coloreados.find(*it);
            // Si el pais adyacente esta coloreado con el mísmo color que tenemos seleccionado...
            if ( it_pais != antes.coloreados.end() && it_pais->second == ultimo->second ) {
               // ... el color no es aceptable y probamos con el siguiente color
               //cout << "inaceptable";
               aceptable = false;
               ultimo->second = (Color)((int)ultimo->second + 1);
               break; // salimos del for
            }
         }         
      }while (ultimo->second < colores_utilizables && ! aceptable);
      return ultimo->second < colores_utilizables && aceptable;
   }
};

bool registrar_solucion( const vector<Colorear*>&, const Estado& meta) {
   cout << meta;
   return false;
}

int main() {
   cout << "1";
   adyacencia[COLOMBIA].insert(VENEZUELA); adyacencia[COLOMBIA].insert(BRASIL);
   adyacencia[COLOMBIA].insert(PERU); adyacencia[COLOMBIA].insert(ECUADOR);
   adyacencia[VENEZUELA].insert(GUYANA); adyacencia[VENEZUELA].insert(BRASIL);
   adyacencia[GUYANA].insert(SURINAM); adyacencia[GUYANA].insert(BRASIL);
   adyacencia[SURINAM].insert(GUAYANA); adyacencia[SURINAM].insert(BRASIL);
   adyacencia[GUAYANA].insert(BRASIL);
   adyacencia[ECUADOR].insert(PERU);
   adyacencia[PERU].insert(BRASIL); adyacencia[PERU].insert(BOLIVIA); adyacencia[PERU].insert(CHILE);
   adyacencia[BRASIL].insert(URUGUAY); adyacencia[BRASIL].insert(ARGENTINA); adyacencia[BRASIL].insert(PARAGUAY); adyacencia[BRASIL].insert(BOLIVIA);
   adyacencia[BOLIVIA].insert(PARAGUAY); adyacencia[BOLIVIA].insert(ARGENTINA); adyacencia[BOLIVIA].insert(CHILE);
   adyacencia[CHILE].insert(ARGENTINA);
   adyacencia[ARGENTINA].insert(PARAGUAY); adyacencia[ARGENTINA].insert(URUGUAY);

   cout << "2";
   // ahora las adyacencias inversas... pero no a mano...
   for( Adyacencia::iterator it_origen = adyacencia.begin(); it_origen != adyacencia.end(); ++ it_origen )
      for( set<Pais>::iterator it_destino = it_origen->second.begin(); it_destino != it_origen->second.end(); ++ it_destino)
         adyacencia[*it_destino].insert( it_origen->first );
   cout << "3" << endl ;
   // Generamos las operaciones, esto es una lista consistente en Colorear construido con cada pais...
   list<Colorear*> operaciones;
   for( Adyacencia::iterator it = adyacencia.begin(); it != adyacencia.end(); ++ it )
      operaciones.push_back( new Colorear(it->first) );
   cout << "4" << endl;
   // Ahora buscamos la solución así: Empezamos con la cantidad mínima de colores, e intentamos pintar todo
   // el mapa, si no es posible añadimos un color más y volvemos a intentar hasta hallar la solucion o agotar los colores...
   bool resuelto = false;
   vector<Colorear*> solucion;
   Estado inicial;
   colores_utilizables =8;
   
   //profundidad_iterativa( operaciones, inicial, 60, &registrar_solucion);
   preferencia_amplitud(operaciones, inicial, &registrar_solucion);
   /*for( colores_utilizables = 1; colores_utilizables <= TOTAL_COLORES && ! resuelto ; ++ colores_utilizables ) {
      cout << "NUEVO";
      //profundidad_limitada( operaciones, inicial, 15, &registrar_solucion );
      preferencia_amplitud( operaciones,inicial , &registrar_solucion);
   }*/
}

   vector<Pais> ciudades;
   //typedef map<Pais, map< Iterador, Costo > > Pais;
   map<Pais, map<Pais*, Costo> > grafo;

   profundidad_limitada( grafo,
   