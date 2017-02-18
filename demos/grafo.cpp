#define IA_NO_CRONOMETRAR

#include "../include/ia_ciega.h"
#include <string>

using namespace std;
using namespace ia;


struct Pais {
   string nombre;
   bool es_meta() const {
      return false;
   }
   Pais(const string& nombre_) : nombre( nombre_ ) {}
   bool operator<(const Pais& otro) const {
      return nombre < otro.nombre;
   }
};


struct GrafoIterador;
typedef Grafo<Pais, GrafoIterador, int > GrafoPaises;
struct GrafoIterador : public GrafoPaises::iterator {
};


int main() {

   GrafoPaises g;
   //g.insert( make_pair(Pais(), GrafoPaises::enlaces_t() ) );
   g[Pais("Bolivia")][Pais("Chile")] = 5;

   vector<GrafoIterador> resultado = vecino_mas_proximo<Pais, GrafoIterador, int >(g, Pais("Bolivia") );
   //grafo.enlazar_bidireccional( Pais(), Pais(), 5);
   //grafo.enlazar(Pais(), Pais() );
}