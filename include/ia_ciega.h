/** \file ia_ciega.h
 * \author Roberto Oropeza Gamarra
 * Contiene a toda la libreria IA Ciega
 * Mejora/Correccion/Optimizacion: Xxxxx añadio/modifico/quito yyyyy para/por zzzzz
 */

#ifndef IA_ciega_H_roberto
#define IA_ciega_H_roberto

/// \defgroup algoritmos Algoritmos de búsqueda en espacios de estados
/// \defgroup auxiliares Útiles para facilitar el uso
/// \defgroup salida Útiles para mostrar resultados
/// \defgroup grafos Búsquedas en grafos
/// \defgroup detalle_implementacion Detalles de implementación

/** defina IA_NO_CRONOMETRAR para deshabilitar el cálculo milisegundos transcurridos en las estadísticas.
 * actualmente el cronometraje funcionará solo en plataformas Windows \ingroup auxiliares*/
//#define IA_NO_CRONOMETRAR

/** defina IA_NO_ESTADISTICAS para deshabilitar el cálculo de estadísiticas al realizar los algoritmos.
 * \ingroup auxiliares*/
//#define IA_NO_ESTADISTICAS

/** defina IA_NO_REGISTRAR_SOLUCION para deshabilitar la capacidad de usar una función callback para
 * recibir las soluciones a medida que se hallen (y la capacidad de hallar todas las soluciones) \ingroup auxiliares */
//#define IA_NO_REGISTRAR_SOLUCION

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <ctime>
#include <cassert>

/** Para que podamos poner esta macro en vez del if largo, y que no tenga efecto si se ha deshabilitado el registro de 
* soluciones usando IA_NO_REGISTRAR_SOLUCION \ingroup detalle_implementacion */
#ifndef IA_NO_REGISTRAR_SOLUCION
   #define IA_IF_DEVUELVE_FALSO_REGISTRAR_SOLUCION(ruta, estado) \
                     if ( ! registrar_solucion || ! (*registrar_solucion)(ruta, estado) )
#else
   #define IA_IF_DEVUELVE_FALSO_REGISTRAR_SOLUCION(ruta, estado) \
                     ((void)0);
#endif

#ifndef IA_NO_CRONOMETRAR
   #include <windows.h>
   #define IA_INICIO_CRONOMETRO ia::detalle::cronometro = GetTickCount();
   #define IA_FIN_CRONOMETRO ia::detalle::cronometro = GetTickCount() - ia::detalle::cronometro;
#else
   #define IA_INICIO_CRONOMETRO ((void)0);
   #define IA_FIN_CRONOMETRO ((void)0);
#endif

#ifndef IA_NO_ESTADISTICAS
   #define IA_INCREMENTAR_SOLUCIONES detalle::conteo_soluciones ++;
   #define IA_INCREMENTAR_EXPANDIDOS detalle::conteo_expandidos ++;
   #define IA_INCREMENTAR_VISITADOS detalle::conteo_visitados ++;
   #define IA_INCREMENTAR_PROFUNDIDAD_MAXIMA detalle::profundidad_maxima ++;
   #define IA_ACTUALIZAR_PROFUNDIDAD_MAXIMA(X) \
                                        if ( detalle::profundidad_maxima < (X) ) detalle::profundidad_maxima = X;
   #define IA_REINICIAR_ESTADISTICAS detalle::reiniciar_estadisticas();   
   #define IA_REINICIAR_VISITADOS detalle::conteo_visitados = 0;
#else
   #define IA_INCREMENTAR_SOLUCIONES ((void)0);
   #define IA_INCREMENTAR_EXPANDIDOS ((void)0);
   #define IA_INCREMENTAR_VISITADOS ((void)0);
   #define IA_INCREMENTAR_PROFUNDIDAD_MAXIMA ((void)0);
   #define IA_ACTUALIZAR_PROFUNDIDAD_MAXIMA(X) ((void)0);
   #define IA_REINICIAR_ESTADISTICAS ((void)0);
   #define IA_REINICIAR_VISITADOS ((void)0);
#endif

/** Todo el contenido de esta libreria esta en este namespace. Para usar las funciones, tipos, etc ponga
 * la linea using namespace ia después de incluir las cabeceras, o use ia::funcion. */
namespace ia {

using namespace std;   

/** Contiene detalles de implementación y la implementacion de los algoritmos de IA. 
 * Para usar los algoritmos de búsqueda use una de las funciones de interfaz que están en el namespace ia 
 * \see ia \ingroup detalle_implementacion */
namespace detalle {

static unsigned int conteo_visitados = 0;
static unsigned int conteo_expandidos = 0;
static unsigned int profundidad_maxima = 0;
static unsigned int conteo_soluciones = 0;
static unsigned int conteo_milisegundos = 0;
#ifndef IA_NO_CRONOMETRAR
   static DWORD cronometro = 0;
#endif
   
void reiniciar_estadisticas() {
   detalle::conteo_visitados = 0;
   detalle::conteo_expandidos = 0;
   detalle::profundidad_maxima = 0;
   detalle::conteo_soluciones = 0;
   #ifndef IA_NO_CRONOMETRAR
      detalle::cronometro = 0;
   #endif
}

}// fin del namespace detalle

/** Muestra las estadísticas de la última búsqueda realizada. Si no se ha \#definido IA_NO_ESTADISTICAS mostrará cero
 * en todos los conteos. Si no se ha \#definido IA_NO_CRONOMETRAR mostrará la cantidad de milisegundos transcurridos.
 * Los milisegundos tienen una precisión de +/- 16 milisegundos, debido a que se usa la funcion GetTickCount de la API de windows
 * y no se consulta directamente al procesador.  \ingroup salida */
void mostrar_estadisticas() {
   using namespace detalle;
   cout << "\nEstados distintos visitados: " << conteo_visitados;
   cout << "\nCantidad de expansiones realizadas: " << conteo_expandidos;
   cout << "\nProfundidad máxima alcanzada: " << profundidad_maxima;
   cout << "\nCantidad de soluciones halladas: " << conteo_soluciones;
   #ifndef IA_NO_CRONOMETRAR
      cout << "\nMilisegundos transcurridos: " << cronometro;
   #endif
   cout << "\n";
}

/** Muestra la solucion como los estados intermedios entre el estado inicial y la meta así como la operación aplicada a cada 
 * uno para obtener el siguiente, es apta cuando los operadores son objetos función
 * \param inicial Es el estado inicial que tenia el problema.
 * \param solucion Una secuencia (p.e. un vector) con las operaciones que nos llevan a la solucion
 * Cada miembro de la solucion debe tener un método get_descripción que devuelve el nombre del operador
 * Esta función muestra la cantidad de operaciones en solucion, el estado inicial y luego va aplicando las operaciones
 * dadas en solucion al estado inicial para obtener los estados sucesivos y los va mostrando \ingroup salida */
template <typename Estado_t, typename Operaciones_t>
void mostrar_solucion( const Estado_t& inicial, const Operaciones_t& solucion ) {
   if ( solucion.empty() )
      cout << "\nLa solución dada está vacía\n";
   else {
      Estado_t actual(inicial), anterior;
      cout << "\nLa solucion consta de " << solucion.size() << " operaciones:\n"
             << "Estado inicial: " << actual << "\n";
      for( typename Operaciones_t::const_iterator paso = solucion.begin(); paso != solucion.end()
                                                                                                     ; ++ paso) {
         anterior = actual;
         (*(*paso))( anterior, actual );
         cout << "***" <<(*paso)->get_descripcion() << "\t --- " << actual << "\n";
      }
   }
}

/** Muestra la solucion como las operaciones necesarias para llegar a la meta sin mostrar estados intermedios, es apta 
 * cuando los operadores son objetos funcion.
 * \param solucion Una secuencia (p.e. un vector) con las operaciones que nos llevan a la solucion
 * Cada miembro de la solucion debe tener un método get_descripción que devuelve el nombre del operador
 * \ingroup salida*/
template <typename Operaciones_t>
void mostrar_solucion( const Operaciones_t& solucion ) {
   if ( solucion.empty() )
      cout << "\nLa solución dada está vacía.\n";
   else {
      cout << "\nLa solucion consta de " << solucion.size() << " operaciones:\n";
      for( typename Operaciones_t::const_iterator paso = solucion.begin(); paso != solucion.end()
                                                                                                     ; ++ paso) 
         cout << (*paso)->get_descripcion() << "\n";
   }
}

/** Muestra la solucion como los estados intermedios entre el estado inicial y la meta así como la operación aplicada a cada 
 * uno para obtener el siguiente, es apta cuando los operadores son punteros a funciones
 * \param inicial Es el estado inicial que tenia el problema.
 * \param solucion Una secuencia (p.e. un vector) con las operaciones que nos llevan a la solucion
 * \param nombre_operadores Un mapa (p.e. NombresOperador) en que la llave es un puntero a la funcion y el valor, su nombre.
 * Esta funcion muestra la cantidad de operaciones en solucion, el estado inicial y luego va aplicando las operaciones dadas
 * en solucion al estado inicial para obtener los estados sucesivos y va mostrando el dato asociado a cada uno en el mapa
 * nombre_operadores  \ingroup salida */
template <typename Estado_t, typename Operaciones_t, typename NombreOperador_t>
void mostrar_solucion( const Estado_t& inicial, const Operaciones_t& solucion
                                , const NombreOperador_t& nombre_operadores ) {
   if ( solucion.empty() )
      cout << "\nLa solución dada está vacía\n";
   else {
      Estado_t actual(inicial), anterior;
      cout << "\nLa solucion consta de " << solucion.size() << " operaciones:\n"
             << "Estado inicial: " << actual << "\n";
      for( typename Operaciones_t::const_iterator paso = solucion.begin(); paso != solucion.end()
                                                                                                     ; ++ paso) {
         anterior = actual;
         (*(*paso))( anterior, actual );
         if ( nombre_operadores.find(*paso) != nombre_operadores.end() )
            cout << "***" <<nombre_operadores.find(*paso)->second << "\t --- " << actual << "\n";
         else
            cout << "***(Nombre de operador desconocido)" << "\t --- " << actual << "\n";
      }
   }
}


/** Muestra la solución como el estado alcanzado después de realizar las operaciones dadas al estado inicial dado, 
 * es apta tanto para operaciones como functores como con objetos función. \ingroup salida*/
template <typename Estado_t, typename Operaciones_t>
void mostrar_estado_solucion( const Estado_t& inicial, const Operaciones_t& solucion ) {
   Estado_t actual(inicial), anterior;
   for( typename Operaciones_t::const_iterator paso = solucion.begin(); paso != solucion.end()
                                                                                                  ; ++ paso) {
      anterior = actual;
      (*(*paso))(anterior,actual);
   }
   cout << "La solucion consta de " << solucion.size() << "operaciones, el estado resultante es: " 
          << actual << "\n";
}

/** Las operaciones pueden ser functores (objeto-funcion) que heredan de este, dándole como parámetro
 * de plantilla el tipo del estado con el que opera. La lista de operaciones puede ser un vector de punteros a este tipo. 
 * Nota: El uso de esta estructura como clase base implicará una llamada a un método virtual al aplicar cada operación, 
 * no se recomienda su uso si el tiempo es importante; considere crear las operaciones como distintas instancias de 
 * <i>una misma clase</i>.  \ingroup auxiliares  */
template <typename Estado_t>
struct Operacion {
   Operacion( const string& nombre_ = string() ) : nombre(nombre_) {}
   /// Debe aplicar la operacion sobre el estado 'antes' y poner el estado resultante en 'despues'.
   /// \return true ssi la operación se llevó a cabo con éxito;
   /// si devuelve false el valor que tenga el parametro 'despues' es irrelevante
   virtual bool operator()(const Estado_t& antes, Estado_t& despues) const = 0;
   virtual string& get_descripcion() {
      return nombre;
   }
   string nombre;
};

/** La lista de operaciones se puede declarar como una instancia de esta estructura,
 * que es un vector de punteros a las funciones que sirven de operadores \ingroup auxiliares */
template <typename Estado_t >
struct Operaciones : public vector< bool(*)(const Estado_t&, Estado_t&) > {
};

/** Si se tiene un vector con punteros a funciones para representar las operaciones que se pueden aplicar al realizar las
 * búsquedas, debería instanciar una estructura como esta, dando como parámetro de plantilla el tipo de estado con el que opera.
 * La llave del mapa es un puntero a una función, y su valor asociado es el nombre o descripción del operador \ingroup salida */
template <typename Estado_t> 
struct NombresOperadores : public map< bool(*)(const Estado_t&, Estado_t&), string > {
};

/** Si utiliza las búsquedas bi-direccionales con un vector con punteros a funciones para representar las operaciones que se pueden
 * aplicar al realizar las búsquedas, puede utilizar este mapa para establecer las operaciones inversas \ingroup auxiliares */
template <typename Operaciones_t>
struct OperadoresInversos : public map< typename Operaciones_t::value_type
                                                       , typename Operaciones_t::value_type > {
   /** Si solo construye los inversos de la mitad de los operadores, use este método para complementar con los operadores restantes
     * ej: siendo a,b,c operadores y A, B, C sus operadores inversos, entonces use una instancia inv de esta estructura 
     * y ponga inv[a] = A; inv[b] = B; inv[c] = C; luego llamar a inv.complementar() añadirá inv[A] = a; inv[B] = b; inv[C] = c.
     * Como este método devuelve una referencia a la misma clase se puede utilizar como rvalue,
     * por ejemplo: mostrar_solucion(...., inv.complementar() )   */
   OperadoresInversos<Operaciones_t>& complementar(){
      typedef map< typename Operaciones_t::value_type
                         , typename Operaciones_t::value_type > mapa_padre;
      map< typename Operaciones_t::value_type, typename Operaciones_t::value_type > complemento;
      for( typename mapa_padre::const_iterator it = this->begin(); it != this->end(); ++ it )
         complemento.insert( make_pair( it->second, it->first ) );
      copy( complemento.begin(), complemento.end(), inserter( *this, this->begin() ) );
      return *this;
   }
};

/** Un contenedor que se construye en base al estado inicial y a una secuencia de operadores como
 * las que devuelven las búsquedas; pero que al iterar devuelve los <i>estados</i> generados al aplicar esas operaciones.
 * Para las búsquedas bidimensionales use simplificar_bidireccional al resultado para obtener una lista 
 * como la requerida por esta clase. Es un contenedor especial, similar a un Forward Container pero que no es propietario de sus
 * elementos, dereferenciar a su iterador en realidad aplica una operación al estado actual y devuelve el nuevo estado obtenido,
 * modificando también al estado actual. Vea los comentarios de la clase interna (inner class) iterator en el código fuente para
 * mayores detalles . \sa simplificar_bidireccional \ingroup auxiliares */
template <typename Estado_t, typename Operaciones_t >
class SecuenciaEstados {
   typedef vector< typename Operaciones_t::value_type > Ruta_t;
   const Ruta_t& operaciones;
   Estado_t inicial;
public:
   SecuenciaEstados( const Estado_t& inicial_
                             , const vector< typename Operaciones_t::value_type >& operaciones_ ) 
            : inicial( inicial_ ), operaciones( operaciones_ ) {}
   class iterator;
   friend class iterator;
   /** Este iterador es el que da al contenedor SecuenciaEstados todas las características mencionadas en su descripción. 
    * \ingroup detalle_implementacion  */   
   class iterator  : public std::iterator< std::forward_iterator_tag, const Estado_t, ptrdiff_t > {
      typename Ruta_t::const_iterator it;
      Estado_t estado_actual;
   public:
      // Para generar el iterador al principio, es el utilizado por el método begin() de su clase exterior (outer class)
      iterator(Estado_t& inicial_, const Ruta_t& operaciones_) : it(operaciones_.begin()) { 
         estado_actual = inicial_; 
      }
      // Para generar el iterador al final, es el utilizado por el método end() de su clase exterior (outer class)
      iterator(const Ruta_t& operaciones_) : it(operaciones_.end() ) {
      }
      bool operator==( const iterator& x ) const {
         return it == x.it;
      }
      bool operator!=( const iterator& x ) const {
         return !( *this == x );
      }
      // Dereferenciar a este iterador devuelve el estado actual
      Estado_t operator*() const {
         return estado_actual;
      }
      // Avanzar este iterador aplica la operación que corresponde al estado actual, de acuerdo a la secuencia de operaciones
      // dada en el constructor de su outer class. Este es el operador de pre-incremento (++it), prefieralo.
      iterator& operator++() {
         Estado_t nuevo;
         bool hecho = (*(*it))(estado_actual, nuevo);
         assert( hecho );
         estado_actual = nuevo;
         ++ it;
         return *this;
      }
      // Avanzar este iterador, sirve para lo mismo que el operador de pre-incremento. Este es el operador de pos-incremento
      // (it++) y no se recomienda su uso a menos que utilice el iterador devuelto por este método (lo mismo que se recomienda 
      // con cualquier pos-incremento/decremento )
      iterator operator++(int) {
         iterator tmp=*this;
         ++*this;
         return tmp;
      }
      // Retroceder el iterador, <i>no tiene ningun efecto</i>
      iterator& operator--() {
         return *this;
      }
      // Retroceder el iterador, <i>no tiene ningun efecto</i>
      iterator operator--(int) {
         return *this;
      }
   };

   // Crea el iterador que empieza con el estado inicial dado y que aplicará las operaciones dadas
   const iterator begin() {
      return iterator( inicial, operaciones );
   }
   // Comparado (con ==) con otro iterador de esta misma clase <i>y construido con la misma secuencia de operaciones</i> 
   // devuelve true ssi ya no hay más operaciones por aplicar
   const iterator end() {
      return iterator( operaciones );
   }
   // Devuelve la cantidad de operaciones dadas en el constructor
   unsigned int size() { 
      return operaciones.size(); 
   }
};

/** Recibe una solución bidireccional y devuelve una solucion secuencial.
 * \param solucion Solucion devuelta por un algorítmo de búsqueda bidireccional
 * \param inversos Mapa con que asocia a cada operación-adelante una operación-atras
 * \return Devuelve una solucion como la que se hubiera hallado utilizando un algorítmo no bidireccional */
template<typename Operaciones_t>
vector<typename Operaciones_t::value_type> 
simplificar_bidireccional( const pair< vector<typename Operaciones_t::value_type>
                                , vector<typename Operaciones_t::value_type> >& solucion
                                , const OperadoresInversos<Operaciones_t>& inversos) {
   vector<typename Operaciones_t::value_type> respuesta( solucion.first );
   for( typename Operaciones_t::const_reverse_iterator it_operacion = solucion.second.rbegin()
                                             ; it_operacion != solucion.second.rend(); ++ it_operacion )
      respuesta.push_back( inversos.find( *it_operacion)->second  );
   return respuesta;
}

// Las funciones y estructuras de datos que estan en el namespace detalle no deben ser usadas directamente. 
// Use el namespace ia solamente
namespace detalle {

/// Alias para contener varios estados cada uno con las operaciones realizadas para llegar a él 
/// (o sea, su ruta) \ingroup detalle_implementacion
template <typename Estado_t, typename Operaciones_t> 
struct EstadoYRuta: public vector<pair< Estado_t, vector< typename Operaciones_t::value_type> > > {
};

/** Algorítmo de búsqueda por amplitud, no es recursivo.
* \param operaciones Todas las operaciones disponibles
* \param expansibles Lista de estados que hay que expandir inicialmente, normalmente el estado inicial y una ruta vacía
* \param registrar_solucion Callback que se llamará cada vez que se halle una solución
* \ingroup detalle_implementacion*/
template <typename Estado_t, typename Operaciones_t>
vector<typename Operaciones_t::value_type> preferencia_amplitud(const Operaciones_t& operaciones 
                           , detalle::EstadoYRuta<Estado_t, Operaciones_t>& expansibles
#ifndef IA_NO_REGISTRAR_SOLUCION
                           , bool(*registrar_solucion)( const vector<typename Operaciones_t::value_type>& 
                                                                ,const Estado_t&)
#endif
                         ){
   set<Estado_t> visitados;
   detalle::EstadoYRuta<Estado_t, Operaciones_t> nuevos;
   // 'resultante' es el nodo recien descubierto al aplicar una operacion
   typedef set<Estado_t> Visitados_t;
   // auxiliar que nos sirve para saber si un estado generado ya se lo había visitado antes
   pair< typename Visitados_t::iterator, bool > insertado;
   while ( ! expansibles.empty() ) {
      IA_INCREMENTAR_PROFUNDIDAD_MAXIMA;
      for( typename detalle::EstadoYRuta<Estado_t, Operaciones_t>::iterator estado 
                                                                                         =expansibles.begin()
                                                    ; estado != expansibles.end(); ++ estado ) {
         // Aplicar todas las operaciones posibles, poniendo los nuevos nodos en 'nuevos'
         IA_INCREMENTAR_EXPANDIDOS;
         for( typename Operaciones_t::const_iterator operacion = operaciones.begin()
                                             ; operacion != operaciones.end(); ++ operacion ) {
            Estado_t resultante;
            if ( ! (*(*operacion))(estado->first, resultante) )
               continue; // Si no se puede aplicar la operacion, intentamos la siguiente
            insertado = visitados.insert( resultante );
            if ( ! insertado.second )
               continue; // si el recien descubierto ya había sido visitado, pasamos a la siguiente operacion
            IA_INCREMENTAR_VISITADOS;
            // colocamos en nuevos al recien descubierto, asociando la misma ruta que su nodo padre...
            nuevos.push_back( make_pair(resultante, estado->second) );
            // ... y le aumentamos a su ruta la operación que acabamos de realizar
            nuevos.back().second.push_back( *operacion );
            if ( resultante.es_meta() ) { // si se encontró un estado meta...
               IA_INCREMENTAR_SOLUCIONES;
               // devolvemos la ruta a no ser que el callback registrar_solucion nos pida continuar la búsqueda
               IA_IF_DEVUELVE_FALSO_REGISTRAR_SOLUCION(nuevos.back().second, nuevos.back().first)
                  return nuevos.back().second; 
            }
         }
      }
      // equivale a: expansibles = nuevos; nuevos.clear(), vea las detalles de implementacion
      expansibles.swap( nuevos );  nuevos.clear();
   }
   // al finalizar toda la busqueda devolvemos una solucion vacia.
   return vector<typename Operaciones_t::value_type>();
}

/** Algoritmo de búsqueda por profundidad, es recursivo. Además de los parámetros mencionados, si no se ha deshabilitado
* la obtención de estadísticas, existe un parámetro más llamado profundidad, utilizado para obtener la profundidad máxima que
* alcanza la búsqueda (esto se hace utilizando macros)
* \param operaciones Todas las operaciones disponibles
* \param visitados Estados que ya se han visitado
* \param ruta Ruta (operaciones realizadas) hasta el estado que se esta explorando
* \param actual Estado que se esta explorando
* \param finalizar Vale true ssi es necesario terminar el algoritmo (p.e. el callback indica que se debe detener la búsqueda)
* \param registrar_solucion Callback llamado cada vez que se encuentra una solucion \ingroup detalle_implementacion */
template <typename Estado_t, typename Operaciones_t, typename Ruta_t>
Ruta_t preferencia_profundidad(const Operaciones_t& operaciones
               , set<Estado_t>& visitados, Ruta_t& ruta
               , const Estado_t& actual
#ifndef IA_NO_ESTADISTICAS
               , unsigned int profundidad // la profundidad actual solo se usa para obtener estadísticas
#endif
               , bool& finalizar
#ifndef IA_NO_REGISTRAR_SOLUCION
               , bool(*registrar_solucion)(const vector<typename Operaciones_t::value_type>&
                                                    ,const Estado_t& ) 
#endif
               ){
   typedef set<Estado_t> Visitados_t;
   
   if ( actual.es_meta() ) { // Si se encontró un estado meta
      IA_INCREMENTAR_SOLUCIONES;
      // devolvemos la ruta a no ser que el callback registrar_solucion nos pida continuar la búsqueda
      IA_IF_DEVUELVE_FALSO_REGISTRAR_SOLUCION(ruta, actual) {
         finalizar = true;
         return ruta;
      }
   }
   IA_ACTUALIZAR_PROFUNDIDAD_MAXIMA( profundidad );
   IA_INCREMENTAR_EXPANDIDOS;
   for( typename Operaciones_t::const_iterator operacion = operaciones.begin()
                                               ; operacion != operaciones.end(); ++ operacion ) {
      // 'resultante' es el nodo recien descubierto al aplicar una operacion
      Estado_t resultante;
      if ( ! (*(*operacion))(actual, resultante ) )
         continue; // Si no se puede aplicar esta operacion, intentamos la siguiente
      // auxiliar que nos sirve para saber si un estado generado ya se lo había visitado antes
      pair<typename Visitados_t::iterator, bool> insertado = visitados.insert( resultante );
      if ( ! insertado.second )
         continue; // Si el recién descubierto ya había sido visitado, pasamos a la siguiente operación
      IA_INCREMENTAR_VISITADOS;
      ruta.push_back( *operacion ); // la ruta actual consta ademas de la operación que acabamos de hacer
      // Exploramos la rama correspondiente al estado recien generado (porque es un tipo de depth-first-search)
      Ruta_t hallada = detalle::preferencia_profundidad<Estado_t, Operaciones_t, Ruta_t>(operaciones
                     , visitados, ruta, resultante
#ifndef IA_NO_ESTADISTICAS
                     , profundidad + 1
#endif
                     , finalizar
#ifndef IA_NO_REGISTRAR_SOLUCION
                     , registrar_solucion
#endif
                     );
      if ( finalizar ) // finalizar puede haber sido puesta en verdadero al explorar la rama recien creada, en tal caso...
         return hallada; // ... dejaríamos de buscar devolviendo la solución hallada (que podria estar en blanco)
      ruta.pop_back(); // la operación hecha para llegar a este estado no forma parte de la ruta al siguiente estado a explorar
      visitados.erase( insertado.first );
   }
   // Hemos recorrido todo el (sub)arbol y no hallamos solucion...
   return Operaciones_t();
}

/** Algoritmo de búsqueda por profundidad limitada, es recursivo pero solo tiene una variable Estado_t en el stack,
* el resto son estáticas.
* \param operaciones Todas las operaciones disponibles
* \param visitados Estados que ya se han visitado
* \param ruta Ruta (operaciones realizadas) hasta el estado que se esta explorando
* \param actual Estado que se esta explorando
* \param profundidad Profundidad en la que se encuentra el estado actual
* \param limite_profundidad Profundidad hasta la cual puede explorarse el árbol de estados
* \param finalizar Vale true ssi es necesario terminar el algoritmo (p.e. el callback indica que se debe detener la búsqueda)
* \param registrar_solucion Callback llamado cada vez que se encuentra una solucion \ingroup detalle_implementacion */
template <typename Estado_t, typename Operaciones_t, typename Ruta_t >
Ruta_t profundidad_limitada(const Operaciones_t& operaciones
               , set<Estado_t>& visitados, Ruta_t& ruta
               , const Estado_t& actual, unsigned int profundidad
               , const unsigned int& limite_profundidad
               , bool& finalizar
#ifndef IA_NO_REGISTRAR_SOLUCION
               , bool(*registrar_solucion)(const vector<typename Operaciones_t::value_type>&
                                                    ,const Estado_t& ) 
#endif
               ){
   typedef set<Estado_t> Visitados_t;
   
   if ( actual.es_meta() ) { // Si se encontró un estado meta
      IA_INCREMENTAR_SOLUCIONES;
      // devolvemos la ruta a no ser que el callback registrar_solucion nos pida continuar la búsqueda
      IA_IF_DEVUELVE_FALSO_REGISTRAR_SOLUCION(ruta, actual) {
         finalizar = true;
         return ruta;
      }
   }
   IA_ACTUALIZAR_PROFUNDIDAD_MAXIMA( profundidad );
   if ( profundidad == limite_profundidad ) // si ya se exploró hasta el límite...
      return Ruta_t(); // ...devolvemos una ruta vacía
   IA_INCREMENTAR_EXPANDIDOS;
   for( typename Operaciones_t::const_iterator operacion = operaciones.begin()
                                               ; operacion != operaciones.end(); ++ operacion ) {
      // 'resultante' es el nodo recien descubierto al aplicar una operacion
      Estado_t resultante;
      if ( ! (*(*operacion))(actual, resultante ) )
         continue; // Si no se puede aplicar esta operacion, intentamos la siguiente
      // auxiliar que nos sirve para saber si un estado generado ya se lo había visitado antes
      pair<typename Visitados_t::iterator, bool> insertado = visitados.insert( resultante );
      if ( ! insertado.second )
         continue; // Si el recién descubierto ya había sido visitado, pasamos a la siguiente operación
      IA_INCREMENTAR_VISITADOS;
      ruta.push_back( *operacion ); // la ruta actual consta ademas de la operación que acabamos de hacer
      // Exploramos la rama correspondiente al estado recien generado (porque es un tipo de depth-first-search)
      Ruta_t hallada = detalle::profundidad_limitada<Estado_t, Operaciones_t, Ruta_t>(operaciones
                     , visitados, ruta, resultante, profundidad + 1, limite_profundidad, finalizar
#ifndef IA_NO_REGISTRAR_SOLUCION
                     , registrar_solucion
#endif
                     );
      if ( finalizar ) // finalizar puede haber sido puesta en verdadero al explorar la rama recien creada, en tal caso...
         return hallada; // ... dejaríamos de buscar devolviendo la solución hallada (que podria estar en blanco)
      ruta.pop_back(); // la operación hecha para llegar a este estado no forma parte de la ruta al siguiente estado a explorar
      visitados.erase( insertado.first );
   }
   // Hemos recorrido todo el (sub)arbol y no hallamos solucion...
   return Operaciones_t();
}

/** Alias para tener varios estados ordenados de acuerdo a cierto criterio (dado por Comparador_t), cada uno asociado 
 * con las operaciones realizadas para llegar a él (o sea, su ruta). Como es un set es necesario que comparador_t 
 * sea un functor que sea un strict-weak-ordering (como se define para la STL), es importante que diferencie correctamente
 * un estado de otro (o sea que si no( a < b ) y no( b < a ) entonces a y b representan al mismo estado)
 * \ingroup detalle_implementacion */
template <typename Estado_t, typename Ruta_t, typename Comparador_t>
struct SetEstadoYRuta : public set< pair< Estado_t, Ruta_t >, Comparador_t > {
};

/** Algoritmo de búsqueda por profundidad limitada doble, es recursivo pero solo tiene una variable Estado_t en el stack, el
 * resto son estáticas. 
 * \param operaciones Todas las operaciones disponibles
 * \param visitados Conjunto de estados que ya han sido visitados
 * \param ruta Lista de operaciones realizadas para llegar al estado actual que debe explorarse
 * \param actual Estado actual que debe explorarse
 * \param profundidad Profundidad del nodo correspondiente al estado actual
 * \param limite_profundidad Máxima profundidad permisible
 * \param finalizar Vale true ssi la funcion debe terminar ya, p.e. porque el callback así lo indica
 * \param atajos Nodos que se hallan en el nivel más profundo en el arbol alterno (el que se recorre en el otro sentido)
 * \param nuevos_atajos Nodos que se hallan en el nivel más profundo de este árbol
 * \param registrar_solucion Callback llamado cada vez que se encuentra una solución \ingroup detalle_implementacion */
template <typename Estado_t, typename Operaciones_t, typename Ruta_t, typename Comparador_t >
pair<Ruta_t, bool> profundidad_limitada_doble(const Operaciones_t& operaciones
               , set<Estado_t>& visitados
               , Ruta_t& ruta
               , const Estado_t& actual
               , unsigned int profundidad
               , const unsigned int& limite_profundidad
               , bool& finalizar
               , SetEstadoYRuta<Estado_t, Ruta_t, Comparador_t>& atajos
               , SetEstadoYRuta<Estado_t, Ruta_t, Comparador_t>& nuevos_atajos
#ifndef IA_NO_REGISTRAR_SOLUCION
               , bool(*registrar_solucion)(vector< typename Operaciones_t::value_type>&
                                                    , const Estado_t& ) 
#endif
               ) {
   typedef set<Estado_t> Visitados_t;
   const Ruta_t ruta_vacia;
   IA_ACTUALIZAR_PROFUNDIDAD_MAXIMA( profundidad );
   if ( atajos.find( make_pair(actual, Ruta_t()) ) != atajos.end() ) { // Si estado actual esta en los nodos inferiores del arbol alterno...
      // ... la solución se ha hallado
      IA_INCREMENTAR_SOLUCIONES;
      // devolvemos la ruta a no ser que el callback registrar_solucion nos pida continuar la búsqueda
      IA_IF_DEVUELVE_FALSO_REGISTRAR_SOLUCION(ruta, actual) {
         finalizar = true;
         return make_pair(ruta,true);
      }
   }
   if ( profundidad == limite_profundidad ) { // Si hemos llegado a la máxima profundidad permitida ...
      // ...insertamos este nodo para que al recorrer el arbol alterno sea tomado en cuenta como uno de los del nivel inferior
      nuevos_atajos.insert( make_pair( actual, ruta ) );
      return make_pair(Ruta_t(),false);
   }
   IA_INCREMENTAR_EXPANDIDOS;
   for( typename Operaciones_t::const_iterator operacion = operaciones.begin()
                                                          ; operacion != operaciones.end(); ++ operacion ) {
      // 'resultante' es el estado generado con esta operación
      Estado_t resultante;
      if ( ! (*(*operacion))(actual, resultante ) )
         continue; // Si no se puede aplicar esta operacion al estado actual, intentamos con la siguiente operacion
      pair<typename Visitados_t::iterator, bool> insertado = visitados.insert( resultante );
      if ( ! insertado.second )
         continue; // Si el estado descubierto ya había sido visitado, intentamos con la siguiente operacion
      IA_INCREMENTAR_VISITADOS;
      ruta.push_back( *operacion ); // la ruta consiste en la misma del estado padre mas la operacion recién aplicada
      pair<Ruta_t,bool> hallada=profundidad_limitada_doble<Estado_t,Operaciones_t,Ruta_t,Comparador_t>
                                                         (operaciones, visitados, ruta, resultante, profundidad + 1
                                                           ,limite_profundidad
                                                           , finalizar, atajos, nuevos_atajos, NULL);
      if ( finalizar ) // finalizar puede haber sido puesta en verdadero al explorar la rama recien creada, en tal caso...
         return hallada; // ... dejaríamos de buscar devolviendo la solución hallada (que podria estar en blanco)
      ruta.pop_back(); // la operación hecha para llegar a este estado no forma parte de la ruta al siguiente estado a explorar
      visitados.erase( insertado.first );
   }   
   return make_pair( Ruta_t(), false);
}

/** Alias para tener varios estados ordenados de acuerdo a cierto criterio (dado por Comparador_mejor_t), cada uno asociado
 * con las operaciones realizadas para llegar a él (o sea, su ruta). Ya que es un multiset es necesario que comparador_t
 * sea un functor que sea un strict-weak-ordering (como se define para la STL), sin embargo no es necesario que si no(a < b)
 * y no(b < a) entonces a y b representen al mismo estado, por otra parte el resultado del functor debe ser determinístico
 * \ingroup detalle_implementacion */
template<typename Estado_t, typename Ruta_t, typename Comparador_mejor_t>
struct MultisetEstadoYRuta : public multiset< pair< Estado_t, Ruta_t>, Comparador_mejor_t > {
};

/** Algoritmo de búsqueda por primero el mejor (A*), no es recursivo
 * \param operaciones Todas las operaciones disponibles
 * \param visitados Conjunto de estados visitados
 * \param expansibles Conjunto de estados que no se han explorado
 * \param registrar_solucion Callback llamado cada vez que se encuentra una solución \ingroup detalle_implementacion */
template <typename Comparador_mejor_t, typename Ruta_t, typename Estado_t
                ,typename Operaciones_t >
Ruta_t primero_mejor(const Operaciones_t& operaciones
                           , set<Estado_t>& visitados
                           , detalle::MultisetEstadoYRuta<Estado_t,Ruta_t,Comparador_mejor_t>& expansibles
#ifndef IA_NO_REGISTRAR_SOLUCION
                           , bool(*registrar_solucion)( const vector<typename Operaciones_t::value_type>& 
                                                                 , const Estado_t&)
#endif
                           ) {
   typedef set<Estado_t> Visitados_t;
   while ( ! expansibles.empty() ) {
      Estado_t actual( expansibles.begin()->first );
      Ruta_t ruta;
      // Para evitar copiar toda la ruta. En la siguiente linea quito al elemento afectado
      // En principio la ruta de los estado que se crearán son la misma que la de su padre
      ruta.swap( *const_cast<Ruta_t*>(&expansibles.begin()->second) ); 
      expansibles.erase( expansibles.begin() );
      IA_ACTUALIZAR_PROFUNDIDAD_MAXIMA( ruta.size() );
      IA_INCREMENTAR_EXPANDIDOS;
      for( typename Operaciones_t::const_iterator operacion = operaciones.begin()
                                           ; operacion != operaciones.end(); ++ operacion ) {         
         Estado_t resultante;
         if ( ! (*(*operacion))(actual, resultante) )
            continue; // Si no se puede aplicar esta operacion, intentamos la siguiente
         // auxiliar para saber si un estado ya se había visitado antes
         pair< typename set<Estado_t>::iterator, bool > insertado = visitados.insert( resultante );
         if ( ! insertado.second )
            continue; // Si ya se habia visitado este estado, intentamos la siguiente operacion
         IA_INCREMENTAR_VISITADOS;
         ruta.push_back( *operacion ); // añadimos a la ruta de este nuevo nodo la operacion aplicada
         expansibles.insert( make_pair( resultante, ruta ) );
         if ( resultante.es_meta() ) { // Si hemos encontrado un estado meta...
            IA_INCREMENTAR_SOLUCIONES;
            // devolvemos la ruta a no ser que el callback registrar_solucion nos pida continuar la búsqueda
            IA_IF_DEVUELVE_FALSO_REGISTRAR_SOLUCION(ruta, resultante)
               return ruta;
         }
         ruta.pop_back(); //quitamos a la ruta esta operacion, porque no formará parte de la del siguiente hijo del nodo padre         
      }
   }
   // Finalmente devolver una ruta vacia, ya sea porque no se encontro ninguna solucion o nos hicieron continuar con el callback
   return Ruta_t();
}

/** Functor auxiliar que compara un par <estado, ruta> tomando en cuenta solamente el estado. Se usa en la búsqueda bidireccional
 * \ingroup detalle_implementacion */
template<typename Estado_t, typename T>
struct ComparaEstadosIgnorandoRutas {
   bool operator()(const pair<Estado_t, T >& izq, const pair<Estado_t, T >& der) const{
      return izq.first < der.first;
   }
};

} // fin del namespace detalle (por ultima vez)

/** Funcion para buscar una o mas soluciones usando el método de preferencia por amplitud.
 * \param operaciones Son todas las operaciones que se pueden aplicar para explorar el espacio de estados
 * \param inicial Es el estado inicial desde el que se empieza la búsqueda
 * \param registrar_solucion Función callback que se llamará cada que se encuentre una solucion (opcional) \ref registrar_solucion
 * \return un vector con las operaciones de la ruta a la solucion, pero si no se halló solución
 * devuelve el vector vacio. OJO: tambien devuelve vacio si el estado inicial es el estado meta, sin embargo el callback
 * registrar_solucion si sera llamado aunque el estado inicial sea el estado meta,
 
 * \ingroup algoritmos */
template <typename Estado_t, typename Operaciones_t>
vector< typename Operaciones_t::value_type> preferencia_amplitud(const Operaciones_t& operaciones
            , const Estado_t& inicial 
            , bool(*registrar_solucion)( const vector<typename Operaciones_t::value_type>& 
                                                  ,const Estado_t& ) = NULL ) {
   typedef vector< typename Operaciones_t::value_type > Ruta_t;
   IA_REINICIAR_ESTADISTICAS;
   detalle::EstadoYRuta<Estado_t, Operaciones_t> principio;
   principio.push_back( make_pair( inicial, Ruta_t() ) );
   
   IA_INICIO_CRONOMETRO;
   if ( inicial.es_meta() ) {// El algoritmo interno no verifica que el estado inicial sea el estado meta, lo hacemos aquí
      IA_IF_DEVUELVE_FALSO_REGISTRAR_SOLUCION( Ruta_t(), inicial )
         /* no-operacion*/;
      return vector<typename Operaciones_t::value_type>();
   }
   Ruta_t solucion = detalle::preferencia_amplitud<Estado_t, Operaciones_t>(operaciones, principio
#ifndef IA_NO_REGISTRAR_SOLUCION
                                                                                                        , registrar_solucion
#endif
                                                                                                       );
   IA_FIN_CRONOMETRO;
   return solucion;
}

/** Funcion para buscar una o mas soluciones usando el método de preferencia por amplitud usando un grafo de operaciones. 
 */

/** Función para buscar una o más soluciones usando el método de preferencia por profundidad (depth-first-search).
 * \param operaciones Son todas las operaciones que se pueden aplicar para explorar el espacio de estados
 * \param inicial Es el estado inicial desde el que se empieza la búsqueda
 * \param registrar_solucion Función que se llamará cuando se encuentre una solucion (opcional) \ref registrar_solucion
 * \return un vector con las operaciones de la ruta a la solucion, pero si no se halló solución
 * devuelve el vector vacio (OJO: también devuelve vacio si el estado inicial es el estado meta).
 * \ingroup algoritmos */
template < typename Estado_t, typename Operaciones_t>
vector< typename Operaciones_t::value_type> preferencia_profundidad(const Operaciones_t& operaciones
               , const Estado_t& inicial
               , bool(*registrar_solucion)( const vector<typename Operaciones_t::value_type>&
                                                    , const Estado_t& ) = NULL ) {
   IA_REINICIAR_ESTADISTICAS;
   set<Estado_t> visitados;
   visitados.insert( inicial );
   typedef vector<typename Operaciones_t::value_type> Ruta_t;
   Ruta_t ruta;
   bool finalizar = false;
   IA_INICIO_CRONOMETRO;
   Ruta_t solucion = detalle::preferencia_profundidad<Estado_t, Operaciones_t, Ruta_t>
                                          (operaciones, visitados, ruta, inicial
#ifndef IA_NO_ESTADISTICAS
                                            , 0
#endif
                                            , finalizar
#ifndef IA_NO_REGISTRAR_SOLUCION
                                            , registrar_solucion 
#endif
                                          );
   IA_FIN_CRONOMETRO;
   return solucion;
}

/** Funcion para buscar una o mas soluciones usando el método de profundidad limitada.
 * \param operaciones Son todas las operaciones que se pueden aplicar para explorar el espacio de estados
 * \param inicial Es el estado inicial desde el que se empieza la búsqueda
 * \param limite_profundidad Es la profundidad máxima a la que puede llegar el árbol de búsqueda
 * \param registrar_solucion Funcion que se llamará cuando se encuentre una solucion (opcional) \ref registrar_solucion
 * \return un vector con las operaciones de la ruta a la solucion, pero si no se halló solución
 * devuelve el vector vacio (OJO: tambien devuelve vacio si el estado inicial es el estado meta),
 * \ingroup algoritmos */
template < typename Estado_t, typename Operaciones_t>
vector< typename Operaciones_t::value_type> profundidad_limitada(const Operaciones_t& operaciones
               , const Estado_t& inicial
               , const unsigned int& limite_profundidad
               , bool(*registrar_solucion)( const vector<typename Operaciones_t::value_type>&
                                                    , const Estado_t& ) = NULL ) {
   IA_REINICIAR_ESTADISTICAS;
   set<Estado_t> visitados;
   visitados.insert( inicial );
   typedef vector<typename Operaciones_t::value_type> Ruta_t;
   Ruta_t ruta;
   bool finalizar = false;
   IA_INICIO_CRONOMETRO;
   Ruta_t solucion = detalle::profundidad_limitada<Estado_t, Operaciones_t, Ruta_t>
                                          (operaciones, visitados, ruta, inicial, 0, limite_profundidad, finalizar
#ifndef IA_NO_REGISTRAR_SOLUCION
                                            , registrar_solucion
#endif
                                          );
   IA_FIN_CRONOMETRO;
   return solucion;
}

/** Funcion para buscar una o mas soluciones usando el método de profundidad iterativa.
 * \param operaciones Son todas las operaciones que se pueden aplicar para explorar el espacio de estados
 * \param inicial Es el estado inicial desde el que se empieza la búsqueda
 * \param limite_profundidad Es la profundidad máxima a la que se explorará, iterando desde 1 hasta limite_profundidad
 * \param registrar_solucion Funcion que se llama cada que se encuentre una solucion (opcional) \ref registrar_solucion
 * \return un vector con las operaciones de la ruta a la solucion, pero si no se halló solución
 * devuelve el vector vacio (OJO: tambien devuelve vacio si el estado inicial es el estado meta),
 * \ingroup algoritmos */
template <typename Estado_t, typename Operaciones_t>
vector<typename Operaciones_t::value_type> profundidad_iterativa(const Operaciones_t& operaciones
               , const Estado_t& inicial
               , const unsigned int& limite_profundidad
               , bool(*registrar_solucion)( const vector<typename Operaciones_t::value_type>&
                                                    , const Estado_t&) = NULL ) {
   IA_REINICIAR_ESTADISTICAS;
   set<Estado_t> visitados;
   typedef vector<typename Operaciones_t::value_type> Ruta_t;
   Ruta_t ruta, solucion;
   unsigned int limite_actual = 0;
   bool finalizar = false;
   IA_INICIO_CRONOMETRO;
   do {
      IA_REINICIAR_VISITADOS;
      limite_actual ++;
      solucion = detalle::profundidad_limitada( operaciones, visitados, ruta, inicial, 0, limite_actual
                                                             , finalizar
#ifndef IA_NO_REGISTRAR_SOLUCION
                                                             , registrar_solucion
#endif
                                                           );
      if ( ! solucion.empty() )
         break;
   }while( limite_actual < limite_profundidad );
   IA_FIN_CRONOMETRO;
   return solucion; // Es vació ssi no se halló la solucion
}

/** Busca en el espacio de estados usando el algoritmo primero mejor. \ingroup algoritmos
 * Es necesario dar el primer parámetro de la plantilla, debe ser el tipo de un functor con un \c operator(),
 * ese tipo podría ser así: \code
 * struct Elige_mejor {
 *   bool operator()( const pair< Estado, vector<Op> >& izq, const pair< Estado, vector<Op> >& der ) {
 *     if ( izq. ... der. ... ) 
          return true;
 *     else 
          return false;
 *   }
 * }; \endcode
 * El método \c operator() debe devolver true ssi el estado representado por el parámetro izq debe ser explorado antes que el
 * estado representado por der, además debe ser un strict weak ordering como lo define la STL, con la salvedad que <i> no es 
 * necesario que si no(a < b) y no(b < a) entonces a y b representan el mismo estado</i>. Los parametros izq y der son
 * \c std::pair cuyo miembro \c first es el estado y el miembro \c second es la secuencia de operaciones realizadas al estado inicial para
 * llegar al estado en cuestión,
 * Dado el functor definido más arriba, se podría llamar a la funcion así: \code
 * primero_mejor<Elige_mejor>( operaciones, inicial ); \endcode
 * \param operaciones Es una secuencia con todas las operaciones que se pueden aplicar a los estados 
 * \param inicial Es el estado inicial desde el que se empieza la búsqueda
 * \param registrar_solucion Funcion que se llama cada que se encuentra una solucion (opcional) \ref registrar_solucion
 * \return Las operaciones aplicadas para llegar al estado meta encontrado. */
template <typename Comparador_mejor_t, typename Estado_t, typename Operaciones_t>
vector<typename Operaciones_t::value_type> primero_mejor(const Operaciones_t& operaciones
               , const Estado_t& inicial
               , bool(*registrar_solucion)( const vector<typename Operaciones_t::value_type>&
                                                    , const Estado_t&) = NULL ) {  
   typedef vector<typename Operaciones_t::value_type> Ruta_t;
   IA_REINICIAR_ESTADISTICAS;
   set<Estado_t> visitados;
   visitados.insert( inicial );
   detalle::MultisetEstadoYRuta<Estado_t, Ruta_t, Comparador_mejor_t> expansibles;
   IA_INICIO_CRONOMETRO;
   expansibles.insert( make_pair( inicial, Ruta_t() ) );  // Si el compilador se queja en esta linea, probablemente la...
                                                                      //...estructura Comparador_mejor_t (que elige el mejor estado)...
                                                                      //...tiene al operator() con los parámetros del tipo herrado, vea...
                                                                      //...el ejemplo viajero.cpp si usa functores para las operaciones
   Ruta_t solucion = detalle::primero_mejor<Comparador_mejor_t, Ruta_t, Estado_t, Operaciones_t>
                                                  (operaciones, visitados ,expansibles
#ifndef IA_NO_REGISTRAR_SOLUCION
                                                   , registrar_solucion 
#endif
                                                  );
   IA_FIN_CRONOMETRO;
   return solucion;
}

/** Busca en el espacio de estados usando un algoritmo similar al de profundidad limitada, pero empezando desde el
* estado inicial y el estado final. Este no busca
* cualquier estado que sea meta (no usa \c es_meta() ), sino el estado 'final' dado, para eso empieza la búsqueda
* por los dos lados (inicial y final) y considera la solución hallada a la secuencia de operaciones realizadas
* para llegar de inicial a un estado intermedio X y desde 'final' al mismo estado intermedio X. \ingroup algoritmos
* \return Un \c std::pair de rutas, la primera (\c first) consiste en las operaciones que se emplearon desde 'inicial'
* a un estado intermedio X, la segunda (\c second) consiste en las operaciones que se emplearon para llegar al mismo 
* estado X, pero partiendo desde 'final'
* \param operaciones Es una secuencia con todas las operaciones que se pueden aplicar a los estados
* \param inicial Es el estado desde el que se buscará al estado final
* \param final Es el estado final buscado
* \param limite_profundidad Es la profundidad máxima a la que puede llegar el árbol de búsqueda total
* \param registrar_solucion Función que se llama cada que se encuentra una solución (opcional) \ref registrar_solucion */
template <typename Estado_t, typename Operaciones_t >
pair< vector<typename Operaciones_t::value_type>, vector<typename Operaciones_t::value_type> >
profundidad_limitada_doble(const Operaciones_t& operaciones, const Estado_t& inicial
                                    , const Estado_t& final, const unsigned int& limite_profundidad
                                    , bool(*registrar_solucion)(vector< typename Operaciones_t::value_type>&
                                                                         ,const Estado_t& ) = NULL ) {
   typedef detalle::SetEstadoYRuta<Estado_t, Operaciones_t
                                                    ,detalle::ComparaEstadosIgnorandoRutas<Estado_t, Operaciones_t> 
                                                > Atajos_t;
   Atajos_t atajos_adelante, atajos_atras;
   atajos_atras.insert( make_pair(final, Operaciones_t()) );
   set<Estado_t> visitados_vacio;
   Operaciones_t ruta_vacia;
   pair< vector<typename Operaciones_t::value_type> , bool> adelante, atras;
   bool finalizar = false;
   unsigned int limite_actual = 0;
   do{
      limite_actual ++;
      atajos_adelante.clear();
      adelante = detalle::profundidad_limitada_doble(operaciones, visitados_vacio, ruta_vacia, inicial
                                                                     , 0, limite_actual, finalizar, atajos_atras
                                                                     , atajos_adelante
#ifndef IA_NO_REGISTRAR_SOLUCION
                                                                     , registrar_solucion
#endif
                                                                     );
      if ( ! finalizar) {
         atajos_atras.clear();
         atras = detalle::profundidad_limitada_doble< Estado_t, Operaciones_t, Operaciones_t>
                                    (operaciones, visitados_vacio, ruta_vacia, final, 0, limite_actual, finalizar
                                      ,atajos_adelante, atajos_atras
#ifndef IA_NO_REGISTRAR_SOLUCION
                                      ,registrar_solucion
#endif
                                     );
      }
   }while ( !finalizar && limite_actual < limite_profundidad );
   if ( ! finalizar )
      return make_pair(vector<typename Operaciones_t::value_type>()
                               ,vector<typename Operaciones_t::value_type>() );
   // Obtener la ruta del lado opuesto al que se llegó
   vector<typename Operaciones_t::value_type>& ruta_conocida 
                                                                          = atras.first.empty() ? adelante.first : atras.first;
   Estado_t estado = atras.first.empty() ? inicial : final ;
   Estado_t nuevo;
   for( typename Operaciones_t::const_iterator it_operacion = ruta_conocida.begin()
                                           ; it_operacion != ruta_conocida.end(); ++ it_operacion ) {
      bool resultado = (*(*it_operacion))(estado, nuevo);
      assert( resultado );
      estado = nuevo;
   }
   const Atajos_t& atajos = atras.first.empty() ? atajos_atras : atajos_adelante ;
   typename Atajos_t::const_iterator it_atajo = atajos.find( make_pair( estado, Operaciones_t() ) );
   assert( it_atajo != atajos.end() );
   if ( atras.first.empty() )
      return make_pair( adelante.first, it_atajo->second );
   else
      return make_pair( it_atajo->second, atras.first );
}





/*                        GRAFOS                  */



/** Declaracion forward para usar el nombre del tipo Enlaces al definir el Grafo \ingroup grafos*/
template < typename Nodo_t, typename Iterador_t, typename Costo_t = int >
struct Enlaces;
   
/** La estructura del grafo. Internamente se almacena como un mapa de nodos, cada uno asociado a sus enlaces, de tipo Enlaces.
 * Los parametros de plantilla son estos:
 * \param Nodo_t Es el tipo del nodo o vértice con el que el grafo debe funcionar. Es un tipo creado por el usuario de la librería para su propio dominio de problema
 * \param Iterador_t Es el tipo de iterador utilizado por el grafo. Debe ser especificado por el usuario como se indica más abajo
 * \param Costo_t Es el tipo del costo de cada enlace, por defecto es un entero (int)
 * \ingroup grafos */
template < typename Nodo_t, typename Iterador_t, typename Costo_t = int>
struct Grafo : public map<Nodo_t, Enlaces<Nodo_t, Iterador_t, Costo_t> > {
   /// Un valor de este tipo esta asociado a cada nodo y permite acceder a sus vecinos y conocer los valores asociados al enlace (costo)
   typedef Enlaces<Nodo_t, Iterador_t, Costo_t> Enlaces_t;
   /// El operator[] permite acceder a los enlaces de un nodo de manera intuitiva
   Enlaces_t& operator[]( const Nodo_t& origen );
   /// El destructor libera recursos utilizados internamente: elimina los punteros a los iteradores que se crearon en los enlaces
   ~Grafo() {
      typename Grafo<Nodo_t, Iterador_t, Costo_t>::iterator it_nodo;
      typename  Enlaces_t::iterator it_enlace;
      for( it_nodo = this->begin(); it_nodo != this->end(); ++ it_nodo )
         for( it_enlace = it_nodo->second.begin(); it_enlace != it_nodo->second.end(); ++ it_enlace )
            delete it_enlace->second;
   }
};

/** La estructura de los enlaces del grafo. \ingroup grafos
*/
template < typename Nodo_t, typename Iterador_t, typename Costo_t >
struct Enlaces : public multimap< Costo_t, const Iterador_t* > {
   typedef Grafo<Nodo_t, Iterador_t, Costo_t> Grafo_t;
   typedef multimap<Costo_t, const Iterador_t* > Padre_t;
   typedef typename multimap<Costo_t, const Iterador_t* >::iterator iterator;
   Grafo_t& grafo;
   Enlaces( Grafo<Nodo_t, Iterador_t, Costo_t>& g ) : grafo(g) {}
   typename Padre_t::iterator begin() { return Padre_t::begin(); }
   typename Padre_t::iterator end() { return Padre_t::end(); }
   struct Auxiliar_costo_asignable_t;
   friend struct Auxiliar_costo_asignable_t;
   /** Se utiliza para que el usuario pueda asignar el costo a un enlace de manera intuitiva \ingroup detalle_implementacion */
   struct Auxiliar_costo_asignable_t {
      typename Padre_t::iterator par_costo;
      Padre_t& outer;
      Auxiliar_costo_asignable_t( Enlaces& enlaces, typename Enlaces::iterator par_costo_ ) : outer(enlaces), par_costo( par_costo_) {}
      void operator=( const Costo_t& costo ) {
         if (par_costo->first == costo)
            return;
         const Iterador_t* iterador_destino = par_costo->second;
         outer.erase( par_costo );
         typename Padre_t::value_type nuevo(costo, iterador_destino);
         outer.insert( nuevo);
      }
      operator Costo_t() {
         return par_costo->first;
      }
   };
   Auxiliar_costo_asignable_t operator[]( Nodo_t const & destino ){
      typename Grafo_t::iterator it_destino = grafo.find(destino);
      if ( it_destino == grafo.end() ) {
         typename Grafo_t::Enlaces_t nuevo(*this);
         typename Grafo_t::value_type valor(destino, nuevo );
         it_destino = (grafo.insert(valor)).first;
      }
      Iterador_t* iterador = new Iterador_t(*(Iterador_t*)(&it_destino));
      //Se usa this->insert porque Digital Mars esta roto
      iterator it_buscando;
      for( it_buscando = begin(); it_buscando != end(); ++ it_buscando ) {
         const Nodo_t& probando = (*it_buscando->second)->first;
         if (  ! (probando < destino) && ! (destino < probando) )
            break;
      }
      iterator it_modificado;
      if ( it_buscando == end() ) {
         typename Enlaces::value_type nuevo( make_pair(Costo_t(), iterador ) );
         it_modificado = this->insert( nuevo );
      }else
         it_modificado = it_buscando;
      Auxiliar_costo_asignable_t resultado(*this, it_modificado);// , iterador );
      return resultado;
   }
};

template< typename Nodo_t, typename Iterador_t, typename Costo_t>
typename Grafo<Nodo_t, Iterador_t, Costo_t>::Enlaces_t& Grafo<Nodo_t, Iterador_t, Costo_t>::operator[]( const Nodo_t& origen ) {
      // Se usa this->find para contentar a Digital Mars
      typename Grafo<Nodo_t, Iterador_t, Costo_t>::iterator it_origen = this->find( origen );
      if ( it_origen == this->end() ) {
         typename Grafo<Nodo_t, Iterador_t, Costo_t>::value_type valor(origen, Enlaces_t(*this));
         it_origen = this->insert( valor).first;
      }
      typename Grafo<Nodo_t, Iterador_t, Costo_t>::value_type nuevo(it_origen->first, Enlaces_t(*this) );
      return (*((this->insert(nuevo)).first)).second; 
}

template< typename Nodo_t, typename Iterador_t, typename Costo_t >
vector<Iterador_t> vecino_mas_proximo( Grafo<Nodo_t, Iterador_t, Costo_t>& grafo, typename Grafo<Nodo_t, Iterador_t, Costo_t>::iterator& it_nodo ) {
}

template< typename Nodo_t, typename Iterador_t, typename Costo_t >
vector<Iterador_t> vecino_mas_proximo( Grafo<Nodo_t, Iterador_t, Costo_t>& grafo, const Nodo_t& nodo ) {
   typedef Grafo<Nodo_t, Iterador_t, Costo_t> Grafo_t;
   typename Grafo_t::iterator it_nodo = grafo.find(nodo);
   if ( it_nodo == grafo.end() )
      ;
   if ( it_nodo->first.es_meta() )
      ;
   typename Grafo_t::Enlaces_t::iterator it_enlace;
   for( it_enlace = it_nodo->second.begin(); it_enlace != it_nodo->second.end(); ++it_enlace) {
      vecino_mas_proximo<Nodo_t, Iterador_t, Costo_t>(grafo, *it_enlace->second );
   }
}

} // fin del namespace ia

#endif
