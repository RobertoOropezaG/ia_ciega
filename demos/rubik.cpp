#include "../include/ia_ciega.h"

#include <iostream>
#include <cstdlib>

using namespace std;

const int MAX_CUADRITOS = 9 * 6;

struct Cubo {
   static const unsigned char VERDE =  1;
   static const unsigned char AZUL =   2;
   static const unsigned char NARANJA= 4;
   static const unsigned char BLANCO = 8;
   static const unsigned char AMARILLO=16;
   static const unsigned char ROJO =   32;
   unsigned char colores[MAX_CUADRITOS];
   bool operator<( const Cubo& otro ) {
      return lexicographical_compare( colores, colores + MAX_CUADRITOS, otro.colores, otro.colores + MAX_CUADRITOS);
   }
};

int main() {
   cout << (int)Cubo::VERDE << " " << (int)Cubo::AZUL << " " << (int)Cubo::NARANJA << " "
        << (int)Cubo::BLANCO << " " << (int)Cubo::AMARILLO << " " << (int)Cubo::ROJO;
   system("PAUSE");
}
