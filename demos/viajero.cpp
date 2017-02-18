/** Resuelve el problema del agente viajero.
*/
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>
#include <map>
#include <set>

#define IA_NO_CRONOMETRAR
#include "../include/ia_ciega.h"

using namespace std;
using namespace ia;

const string ORIGEN = "Tarija";
const string DESTINO = "Pando";

// Destinos son parejas de ciudad colindante y la longitud de la carretera a ella
typedef map<string, int> Destinos;
// a cada ciudad se le asocia un conjunto de destinos
typedef map< string, Destinos > Mapa;
Mapa mapa;

// A cada ciudad se le asocia su distancia lineal a la ciudad meta
Mapa distancias;

// Esta es la estructura estado, simplemente indica en qué ciudad estamos
struct Ciudad {
   string nombre;
   bool operator<( const Ciudad& otro ) const {
      return nombre < otro.nombre;
   }
   bool es_meta() const {
      return nombre == DESTINO;
   }
   friend ostream& operator<<(ostream& os, const Ciudad& ciudad ) {
      os << "(" << ciudad.nombre << ") ";
      return os;
   }
};

// Esta clase hace las operaciones, o sea, los viajes
struct Viajar : public Operacion<Ciudad> {
   string destino; // destino del viaje
   Viajar() {}
   Viajar(string destino_) : destino( destino_ ) {
   }
   bool operator()(const Ciudad& antes, Ciudad& despues) const {
      // verificamos que hay carretera a la ciudad a la que nos toca probar
      if ( mapa[antes.nombre].find( destino ) == mapa[antes.nombre].end() )
         return false;
      despues.nombre = destino;
      return true;
   }
};

// Este es el functor que decide qué ciudad elegir
struct Elige_mejor_ciudad {
   bool operator()(const pair<Ciudad, vector<Viajar*> >& izq, const pair<Ciudad, vector<Viajar*> >& der) const {
      // preferimos la ciudad cuya distancia lineal al destino sea menor
      return distancias[DESTINO][izq.first.nombre] <= distancias[DESTINO][der.first.nombre];
   }
};

// Para mostrar los mapas creados, realmente no es necesario...
void mostrar_mapa( Mapa& mapa ) {
   cout << "-----------\n";
   for( Mapa::iterator it_partida = mapa.begin(); it_partida != mapa.end(); ++ it_partida )
         for( Destinos::iterator it_destino = it_partida->second.begin()
                                                    ; it_destino != it_partida->second.end(); ++ it_destino )
            cout << "De " << it_partida->first << " a " << it_destino->first << " hay " 
                   << it_destino->second << "kms\n";
            
}

int main() {
   // a cada origen vs. destino se le asigna la distancia
   mapa["Pando"]["La Paz"] = 540;
   mapa["Pando"]["Beni"] = 430;
   mapa["Beni"]["La Paz"] = 610;
   mapa["Beni"]["Santa Cruz"] = 640;
   mapa["La Paz"]["Oruro"] = 229;
   mapa["Santa Cruz"]["Cochabamba"] = 540;
   mapa["Santa Cruz"]["Chuquisaca"] = 680;
   mapa["Cochabamba"]["Oruro"] = 228;
   mapa["Cochabamba"]["Potosi"] = 532;
   mapa["Cochabamba"]["Chuquisaca"] = 366;
   mapa["Oruro"]["Potosi"] = 335;
   mapa["Potosi"]["Chuquisaca"] = 166;
   mapa["Potosi"]["Tarija"] = 334;
   mapa["Chuquisaca"]["Tarija"] = 549;
   // Se generan los caminos inversos 
   for( Mapa::iterator it_partida = mapa.begin(); it_partida != mapa.end(); ++ it_partida )
      for( Destinos::iterator it_destino = it_partida->second.begin(); it_destino != it_partida->second.end(); ++ it_destino )
         mapa[ it_destino->first ][it_partida->first] = it_destino->second;
   
   mostrar_mapa(mapa);
   
   // Cargar las distancias lineales hasta la ciudad de pando
   distancias["Pando"]["La Paz"] = 500;
   distancias["Pando"]["Beni"] = 300;
   distancias["Pando"]["Oruro"] = 600;
   distancias["Pando"]["Santa Cruz"] = 800;
   distancias["Pando"]["Cochabamba"] = 500;
   distancias["Pando"]["Potosi"] = 1000;
   distancias["Pando"]["Chuquisaca"] = 1100;
   distancias["Pando"]["Tarija"] = 1200;
   
   // Creamos las operaciones posibles, esto es, todos los destinos posibles
   vector<Viajar*> operaciones;
   for( Mapa::iterator it_ciudad = mapa.begin(); it_ciudad != mapa.end(); ++ it_ciudad )
      operaciones.push_back( new Viajar(it_ciudad->first) );
   
   // Por fin empezamos la búsqueda...
   Ciudad inicial;
   inicial.nombre = ORIGEN;
   mostrar_solucion( inicial, profundidad_limitada(operaciones, inicial, 15) );
   mostrar_solucion( inicial, primero_mejor<Elige_mejor_ciudad>( operaciones, inicial ) );
   mostrar_solucion( inicial, preferencia_amplitud( operaciones, inicial) );
}
