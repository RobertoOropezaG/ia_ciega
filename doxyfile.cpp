// Este archivo no contiene código fuente, está solamente para generar la página inicial de la 
// documentacion en HTML. Se lo puede borrar con seguridad.
/** \mainpage Documentación de IA Ciega
 El objetivo de esta libreria es facilitar el uso de los Algoritmos de Búsqueda usados en Inteligencia Artificial de manera que una vez que se tengan definidos los estados y las operaciones sea sencillo probar los distintos algoritmos sin tener que implementarlos.
 
 \section intro Introducción
 La librería esta diseñada para permitir un uso sencillo sin sacrificar la flexibilidad. Se han usado plantillas,
 pero para llamar a las funciones no es necesario especificar los tipos de las plantillas, excepto en \c primero_mejor.
 
 La corrida debería ser muy rápida porque siempre que es posible uso parámetros por referencia, ademas, me apoyo en la STL
 y plantillas para los algorítmos y contenedores necesarios.

 \section uso Modo de uso
 
  Para traer todas las funciones al scope de trabajo, evitando poner ia::... a cada rato, puedes poner:
 \code
 using namespace ia;
 \endcode
 al principio de tu programa, después de incluir las cabeceras que necesites.
 
   Para una visión general de la libreria ve las pestañas Páginas relacionadas, Módulos, y Ejemplos, 
  Cuando estes dispuesto a usar la librería mirá el \ref uso_general y luego alguno de los \ref algoritmos, tambien mirá los \ref consejos. Recuerda ver los ejemplos para ver el uso práctico y ante cualquier duda, el más sencillo es el pastor.cpp, los mas complejos el 8puzzle.cpp y reinas.cpp.
 
 \section compatibilidad Compatibilidad y compilación
 Esta librería consiste solo de un archivo cabecera: ia_ciega.h, así que no requiere compilación de librería.
 Se usa intensivamente la STL y plantillas, pero no se usa meta-programación, así que cualquier C++ contemporaneo debería ser suficiente.
 Se ha probado con éxito usando:
 <ul>
 <li>Bloodshed Dev-Cpp 4.9.9.2 [=g++ (GCC) 3.4.2 (mingw-special)]
 <li>Borland CBuilder 6 Update Pack 4 [=Borland C++ 5.6.4]
 <li>Borland CBuilder 2006 Update Pack 2 [=Borland C++ 5.8.2]
 <li>Borland free C++Builder 5 Command Line Tools [=Borland C++ 5.5.1]
 <li>Digital Mars 8.4.8 [=Digital Mars Compiler 8.42n]
 <li>Microsoft C++ de VisualStudio .NET 2003 y Visual C++ Toolkit 2003 [=Microsoft C/C++ 13.10.3077]
 <li>Microsoft C++ de VisualStudio 2005 [=Microsoft 32-bit C/C++ Optimizing Compiler 14.00.50727.42 for 80x86]
 </ul>
 
 El uso con los compilador de Microsoft Visual C++ Toolkit/VisualStudio .NET 2003 es posible solamente si se especifican los tipos de las plantillas al momento de llamar a las funciones, como se ve en los ejemplos. Esta incompatibilidad ha sido solucionada en MS VS 2005.
 
  En el caso del compilador gratuito (MS VC++ Toolkit) no será posible usar el cronometraje automático porque tal herramienta no incluye windows.h. 
 
 No se puede usar con:
 <ul>
 <li>Borland C++ 7.0 para DOS
 <li>DJGPP
 </ul>
 Son todos los compiladores con los que se ha probado, si alguno no esta en la lista significa que todavía no se ha probado y que podría funcionar.
 \section instalacion Instalación
 La instalación es solamente copiar el archivo ia_ciega.h a una carpeta donde la encuentre el compilador, esa carpeta suele llamarse "include".
 
 \section limitaciones Características y Limitaciones 
 <ul>
 <li><b>Reentrancia: </b> En una aplicacion con más de un hilo debe evitar que el <i>mismo tipo de búsqueda (o sea la misma función de búsqueda con los mismos parámetros de plantilla)</i> este siendo realizado por dos hilos a la vez, debe considerarse también el mismo tipo de búsqueda a profundidad_limitada y profundidad_iterativa. Si se busca con las mismas funciones pero con diferentes parametros de plantilla no hay problemas de reentrancia.
 <li>La capacidad de cronometrar la ejecucion solo es posible en plataforma MS Windows, en otras debe deshabilitarse con \#define IA_NO_CRONOMETRAR
 <li>Actualmente solo se ha compilado en plataforma MS Widnows, pero debe poderse en otras ya que no se usan características específicas de ese sistema (salvo en el cronometraje).
 </ul>
 
 \section Pendientes
 
 <ul>
 <li>Crear contenedores con características adicionales para esta libreria, como ser secuencias con allocators para memoria paginada en disco (con eso sería posible su uso en búsquedas en espacios inmensos)
 <li>Hacer una comparación estadística práctica con otras librerias de búsqueda, tanto en tiempo como en memoria.
 <li>Implementar otros algorítmos de búsqueda
 </ul>
*/
 
*/

/** \page uso_general Uso general
 Las funciones de búsqueda estan en el namespace ::ia y son así: (las que estan en el namespace ::ia::detalle son internas y no es necesario llamarlas directamente)
 \code template< [...] >
 nombre_funcion(const Operaciones_t &operaciones, const Estado_t &inicial, [...], (bool*)registrar_solucion( [...] ) ); \endcode
 No te preocupes por lo de \c template, para llamar a las funciones no necesitas especificar ninguno de esos tipos, eso se hará automáticamente (excepto con Microsoft VC Toolkit y VC NET 2003). El primer parámetro siempre es una \ref operaciones y el segundo un \ref estados.
 \section operaciones Lista de operaciones
 Las funciones de búsqueda reciben siempre como primer parámetro un contenedor secuencial de las operaciones que
 se pueden aplicar a los estados para expandirlos. Esta secuencia puede ser una secuencia (p.e. un \c vector) de
 punteros a las funciones que se pueden usar para hacer expansiones, también puede ser una secuencia (p.e. un
 \c vector) de punteros a una clase base (normalmente \c ia::Operacion<Estado>) cuyos herederos definen al operador
 de paréntesis ( \c operator() ). Ten en cuenta que el uso de una clase base para las operaciones implica una llamada
 a una función virtual (al \c operator() ) y esto equivale a dos indirecciones, es preferible utilizar una sola clase cuyas distintas instancias ejecuten de distinta manera a su \c operator()
 
 \subsection funciones Operaciones como funciones
 La lista de operaciones para expandir estados puede ser una secuencia de punteros a funciones (en este caso las funciones son \c mi_operacion1 y \c mi_operacion2), así:
 \code
 #include "ia_ciega.h"
 using namespace std;
 struct Estado {
    // Después se indica cómo debe ser esta estructura ...
 };
 
 bool mi_operacion1( const Estado& antes, Estado& despues ) {...}
 bool mi_operacion2( const Estado& antes, Estado& despues) {...}
 // ... etc
 int main() {
     Operaciones<Estado> operaciones;
     NombresOperadores<Estado> nombres;
     operaciones.push_back( &mi_operacion1 ); 
     nombres[ &mi_operacion1] = "Mi operacion 1";
     operaciones.push_back( &mi_operacion2 ); 
     nombres[ &mi_operacion2] = "Mi operacion segunda";
     //...
     // Establecer el estado inicial
     Estado inicial(3,4);
     // llamar a la función de búsqueda deseada...
     mostrar_solucion( inicial, profundidad_iterativa( operaciones, inicial, 11), nombres );
     // etc...
 }
 \endcode
 
 Fijate cómo usamos las estructuras auxiliares \c Operaciones y \c NombresOperadores dando el parámetro Estado.
 
 Cada una de las funciones mi_operacion1, mi_operacion2, etc, reciben como primer estado el estado actual y
 deben poner en el segundo parametro es estado resultante de aplicar la operacion. Si no se puede aplicar la
 operación debe devolver falso, si se puede aplicar debe devolver verdadero. Cuando devuelve falso, no se usará
 el valor del segundo parámetro fuera de esta funcion, así que se puede dejar como basura.
 
 \subsection functores Operaciones como clases (functores)
 Existen dos maneras de trabajar: definir una clase distinta para cada operación (considere usar punteros 
 a funciones), la segunda es la que se explicará aquí, a saber, una sola clase para todas las operaciones, el 
 comportamiento específico de la operación se establece en el constructor de la clase.
  
 La lista de operaciones para expandir estados puede ser una secuencia de punteros a una clase. En la lista de operaciones
  se ponen punteros objetos de esta clase, cada instancia ejecuta el \c operator() de acuerdo a la 
  operacion que realiza, así:
  
  \code
  struct Estado {
     // Después se indica cómo debe ser esta estructura...
  };
  
  class Accion : public Operacion<Estado> {
     ... // aquí puedes poner los miembros datos de la operacion
  public:
     Accion(...) {...}
     bool operator()(const Estado_t& antes, Estado_t& despues) const {
        ... // Aplica la operacion de acuerdo a sus miembros dato
     }
     
  }
  \endcode

 
 En el constructor se ajustan los miembros datos del objeto, que son los parámetros que definen cómo se realizará la operación
 
 La clase base utilizada \c Operacion<Estado> está definida en
 esta librería y contiene un miembro dato \c nombre en el que se puede poner el nombre de la operación, así como
 debería ser mostrada en la solución, alternativamente se puede re-implementar la funcion \c get_descripción()
 para mostrar la operacion. Todo esto es opcional y solo es requerido si se utiliza la función mostrar_solucion
 de esta libreria.
 
 No es necesario heredar de \c Operacion<Estado>; pero, se herede o no, se debe declarar el \c operator() de esta manera en la clase base (en Operacion<Estado> ya esta declarada), y/o implementarla en cada clase (heredera):
 \code
 virtual bool operator()(const Estado_t& antes, Estado_t& despues) const 
 \endcode
 
 este operador debe cumplir con los requisitos especificados anteriormente para las operaciones como funciones,
 con el requerimiento adicional de que no deben modificar los miembros datos del objeto (por eso es un 
 método \c const ). Si crees que necesitas cambiar algun miembro dato ten en cuenta que el orden en que se llama
 al \c operator() es indefinido, (para que se pueda cambiar un miembro dato \c m de tipo \c Tipo este debería ser
 declarado como \c mutable, así: \c mutable \c Tipo \c m; )
 
 Las funciones de búsqueda solo utilizaran los objetos pasados en la lista de operaciones, no crearán nuevas instancias.
 
 El ejemplo de las 8 reinas (reinas.cpp) hace uso de esta técnica, dandole en el constructor la fila en que
 debe poner una reina cada instancia de una misma clase.
 
 \section estados Estado inicial
 El segundo parámetro de las funciones de búsqueda es el estado inicial desde el que se empieza la búsqueda.
 
 Un estado es una estructura (o clase) con esta interfaz:
 \code
 struct Estado {
   // ... aquí puedes poner los atributos de tu estado
   Estado() {...} // Un constructor por defecto, Normalmete no hará nada
   ... // aquí puedes poner otros constructores
   // Debe devolver true ssi es un estado meta
   bool es_meta() const {
      ...
   }
   // Un operador menor que, de orden debil estricto, para que pueda estar en un std::set
   bool operator<(const Estado& otro) const {
      ...
   }
   // Devuelve una cadena con la especificación del estado (opcional)
   friend ostream& operator<<(ostream& os, const Estado& e ) {
      os << ... ;
      return os;
   }
 };

 \endcode
 El constructor por defecto y todas estos métodos (excepto el \c operator<<) deberían terminar rápido porque son
 llamados varias veces durante las búsquedas. Los estados construidos con el constructor por defecto 
 se usan solamente para ser el segundo parámetro de las funciones que se usan para expandir estados (o 
 del \c operator() si son functores) así que su inicialización puede hacerse en la función u \c operator(),
 
 \c es_meta() se llama en todos los estados generados.
 
 \c operator< se usa para poner al estado en un \c std::set de estados visitados, debe ser un strict weak ordering
 como se define en la STL, es decir que si a < b entonces es falso que b < a, y si a < b y b < c entonces a < c. Dos
 estados A y B se considerarán equivalentes (es decir solo se explorará uno) si es falso que A < B y es falso que B < A.
 
 \c operator<< se usa en mostrar_solucion con dos parámetros, así que no afecta a la velocidad de las funciones
 de búsqueda
 
 \section registrar_solucion Uso del parámetro registrar_solucion
 Los algorítmos de búsqueda reciben, opcionalmente, un parámetro registrar_solucion que es un 
 puntero a una función que se llamará cada vez que se encuentre una solución. Es una función "callback" 
 aquí se describe cómo se usa.
 
 La funcion puede ser así
 \code
 bool registrar_solucion(const vector<X>& ruta, const Estado& estado)
 \endcode
 donde X es el tipo de las operaciones puestas inicialmente en el contenedor de operaciones que se pueden aplicar
 (que pueden ser punteros a objetos funcion (o sea, a functores) o punteros a funciones).
 
 Todas las búsquedas terminarán al encontrar la primera solución, si desea continuar la búsqueda de más soluciones debe implementar esta función y pasar un puntero a ella a la función de búsqueda que use, la búsqueda llamará a su función cada vez que halle una solución. Si desea que la búsqueda continue devuelva true, si desea terminarla devuelva false en este último caso la función de búsqueda originalmente llamada devolverá la solución que se acaba de hallar; por otra parte, si su función registrar_solucion siempre devuelve true eventualmente la función de búsqueda terminará devolviendo una ruta vacía.
 
 En el primer parámetro recibirá un vector con punteros a las operaciones que se hicieron para encontrar la meta
 recién hallada; en el segundo parámetro estará el estado meta que se halló.
 
 Por ejemplo, si las operaciones se dieron usando el tipo auxiliar Operaciones<Estado> (o sea, usando punteros a funciones) entonces ruta sera de tipo \c vector<bool(*)(const Estado&, Estado&)> , es decir un vector de punteros a las funciones que se aplicaron al estado inicial para llegar al estado meta dado como segundo parámetro.

 <hr>
 
 Vaya a la pestaña Módulos y elija Algoritmos de búsqueda para obtener ayuda 
 sobre una función de búsqueda en particular
 */
 
/** \page consejos Consejos prácticos
 Recuerda poner \c using \c namespace \c ia, o empezar todos los usos de identificadores de la libreria con \c ia::
 
 Es muy importante que las funciones de operaciones o los métodos operator() reciban el primer parámetro como "const Tipo& antes" y el segundo como "Tipo& despues" donde Tipo es el tipo de los estados. No se debe modificar el primer parámetro!.
 
 La clase (o estructura) que uses como Estado debe ocupar poca memoria, su constructor por defecto debe ser rápido, preferiblemente vacío.
 
 Las funciones para aplicar operaciones deben ser rapidísimos. (Si utilizas objetos-funcion, el \c operator() debe ser rapidísimo)
 
 Si utilizas functores, deben ocupar poca memoria. Si necesitas muchos datos en los functores considera ponerlos en memoria global y mantener solo una referencia a esos datos en los functores.
  
*/

/** \page preprocesador Uso de macros para deshabilitar estadísticas y registro de soluciones.
 La librería puede obtener algunas métricas sobre la cantidad de estados visitados, expandidos, etc esto reduce un poco la velocidad de la búsqueda. Para evitar el cálculo de métricas ponga la linea:
 \code #define IA_NO_ESTADISTICAS 
 \endcode
 antes de \# incluir esta librería.
 
 Para evitar que se tomen métricas de tiempo de ejecución ponga la línea
 \code #define IA_NO_CRONOMETRAR
 \endcode
 antes de # incluir esta ibrería. Es necesario definir esta macro si no está compilando en plataforma Windows.
 
 La precisión de las métricas de tiempo tienen un error de (+/-)8 milisegundos
 
 Si se definen ambas macros el cálculo de estadísticas no se llevará a cabo y las búsquedas se harán un poco mas rápido, la definición de solo una también aumenta la velocidad.
 
 Las funciones de búsqueda reciben un parámetro opcional \c registrar_solucion que puede ser llamado cada vez que se halla una solucion y permite continuar la busqueda para encontrar todas las soluciones; para deshabilitar esta característica (y así aumentar un poco la velocidad de los algorítmos) ponga la linea: \code #define IA_NO_REGISTRAR_SOLUCION \endcode antes de \# incluir esta librería. De todas maneras se podrá pasar el parámetro registrar_solucion a las funciones, pero no se utilizará, esto es así para que el programador usuario no deba modificar su propio código fuente.
*/

/** \page rationale Rationale
<b>Se usan otras funciones para llamar a las que estan en el namespace ia::detalle</b> porque las que estan en detalle necesitan más parámetros que los que el llamador quisiera llenar con información de su propio problema, por otra parte el paso de parámetros en la primera llamada a las funciones de detalle deben cumplir varios requerimientos que al llamador solamente le dificultarian su uso.

<b>Se prefiere usar plantillas y no usar funciones virtuales para la realización de operaciones y la comparación de estados</b> porque una llamada a una función virtual es un poco mas lenta que la llamada a funciones no-virtuales. Esto tiene más importancia porque la función virtual sería llamada en un ciclo interno durante las búsquedas

<b>Se usa el lenguaje C++</b> porque tiene las siguientes características necesarias para esta librería, y no se encuentran en otros lenguajes: 1) Plantillas, no solo a nivel de contenedores, si no de operadores, por ejemplo para usar la misma función con functores y funciones. 2) Preprocesador, para poder deshabilitar capacidades y así ganar velocidad. 3) Generación de código eficiente, debido a que el código es ejecutado directamente por el hardware. 4) Librerias estandarizadas con variedad de estructuras de datos, algoritmos ortogonales a las estructuras de datos.

<b>Se utilizan macros del preprocesador para deshabilitar el cálculo de estadísticas y registro de soluciones</b> porque de esta forma una vez deshabilitadas permitirán un funcionamiento tan rápido como si nunca hubiera habido posibilidad de utilizar tales características. (No es necesario evaluar un booleano para tomar la decisión de usarlas).

Para ver detalles específicos de la implementacion consulte la siguiente sección \ref detalles y el código fuente, que tiene muchos comentarios explicativos que no aparecen en este manual.
*/

/** \page preguntas Preguntas Hechas Frecuentemente
(En realidad nadie hizo preguntas, pero estas respuestas dan información importante que no cabe en otros sitios de este manual)
<ul>
<li> <i> ¿No sería más fácil hacer mi propia función de búsqueda en vez de aprender a usar toda esta librería? </i>.
No creo. Son algoritmos difíciles de implementar, mas bíen sería bueno empezar usando esta librería (u otra) y, después de comprobar que tus estados y operaciones funcionan, crear tus propias funciones de búsqueda. No tienes que aprender a usar toda la libreria, por ejemplo si no has usado mucho functores o la STL puedes limitarte a usar una función por cada operación.
<li><i> ¿Tengo que leer todo el código fuente para aprender a usar esta librería? </i> No, basta con leer los comentarios del código fuente, estos se presentan en forma más legible en la documentacion HTML incluida en el CD de la librería.
<li> <i> He visto el programa fuente y se usan características avanzadas de C++ (plantillas, STL, functores, etc.) ¿Necesito aprender todo eso para usar esta librería? </i> No. Mas bien hay que empezar por los ejemplos. Especialmente el de los cántaros, el laberinto y el pastor usan muy poco de la STL y lo mínimo de plantillas, si tienes problemas con eso te aconsejo hacer uso de la libreria empezando sobre uno de los ejemplos, en vez de empezar en blanco.
<li><i> ¿Puedo usar este programa en un producto comercial?</i> Si. Pero si tu producto incluye el código fuente tienes que incluir el código fuente de esta librería y también el archivo licencia_1_0.txt en la instalación y no borrar mi nombre del código fuente (tu producto puede utilizar otra licencia, la mia solo hara referencia a mi porción de código). Puedes incluir tu nombre en ia_ciega.h si añadiste algo a la librería, pero en ningún caso debes hacer creer que tú hiciste el código que yo hice ¿Obvio no?. Si tu producto está solo en forma binaria (sin código fuente) no es necesario que incluyas el archivo de licencia ni que aparezca mi nombre, una pequeña mención en "Acerca de" sería un bonito detalle de tu parte, tampoco debes hacer creer que tú hiciste la parte que yo hice.
</ul>

*/

/** \page detalles Detalles de implementacion
En esta página se explican detalles útiles solamente a aquellos que quieran modificar o mejorar esta librería, o quienes esten interesados en sus detalles. Se supone un conocimiento práctico de los contenedores estandar de C++ (parte de la STL) y una lectura al resto de este documento, especialmente la seccion Rationale.

La librería esta dividida en tres partes conceptuales: la interfaz, los algoritmos y los auxiliares (tanto estructuras de datos como funciones).

<ol>
<li>La <b>interfaz</b> comprende todo aquello a lo que el programador usuario puede acceder, esto es, funciones y tipos que se encuentran en el namespace ::ia, se puede usar todas las capacidades de esta libreria con solo ese conocimiento. Pertenecen a esta parte ia::profundidad_limitada_doble, ia::profundidad_iterativa, ia::mostrar_solucion, el tipo ia::Operacion, etc.


<li>Los <b>algoritmos</b> se encuentran ocultos en el namespace ia::detalle y hay uno por cada método de búsqueda que aparece en la interfaz. Pertenecen a esta parte ia::detalle::profundidad_limitada_doble, ia::detalle::profundidad_iterativa


<li>Los <b>auxiliares</b> tambien estan en el namespace ia::detalle y son utilizados tanto por los algoritmos como por la interfaz. Tenga en cuenta que en el resto de este documento se ha usado el término auxiliares haciendo referencia a ciertas funciones/tipos de la <i>interfaz</i> que no pertenecen a esta categoría conceptual.
</ol>

\section detalle_interfaz Detalles sobre la interfaz
El uso de la interfaz se ha explicado en el resto de este documento, aquí se explica cómo esta implementada.

Las funciones de búsqueda de la interfaz tienen el propósito de que los algoritmos reciban la menor cantidad de parámetros posibles, y permitir que el compilador elija el tipo de los parámetros de plantilla necesarios, así como a adaptar las llamadas de acuerdo a la macro IA_NO_REGISTRAR_SOLUCION. 

Tómese el código de la funcion de interfaz preferencia_amplitud como ejemplo
\code
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
\endcode 

En esas lineas, en base a los parámetros recibidos se inicializan las variables requeridas por las funciones de algoritmos, se inician los contadores estadísticos y el cronometraje

\code
   if ( inicial.es_meta() ) {// El algoritmo interno no verifica que el estado inicial sea el estado meta
      IA_IF_DEVUELVE_FALSO_REGISTRAR_SOLUCION( Ruta_t(), inicial )
         ; // no-operacion
      return vector<typename Operaciones_t::value_type>();
   }
\endcode   
La macro IA_IF_DEVUELVE_FALSO_REGISTRAR_SOLUCION ejecuta a la funcion apuntada por registrar_solucion si esto esta habilitado, de otra forma no hace nada.

En caso de que el estado inicial sea el estado meta, y el algoritmo interno no revise esta situacion con el estado inicial, son las funciones de la interfaz las que se encargan de llamar a la funcion registrar_solucion, como se ve en el ejemplo.

\code
   Ruta_t solucion = detalle::preferencia_amplitud<Estado_t, Operaciones_t>(operaciones, principio
#ifndef IA_NO_REGISTRAR_SOLUCION
                                                         , registrar_solucion
#endif
                                                         );
   IA_FIN_CRONOMETRO;
   return solucion;
}
\endcode

finalmente llama a la función de algoritmo interna con los parámetros que ella requiere, usando al preprocesador envian el parámetro registrar_solucion solamente si no esta definida la macro IA_NO_REGISTRAR_SOLUCION. Termina por devolver lo mismo que la función interna.

\section detalle_algoritmo Detalles sobre la implementación
 Siempre se ha minimizado el uso de variables locales en los algoritmos recursivos.
 
 Internamente se utiliza el contenedor vector<> para almacenar datos que no se necesita ordenar, porque todas las inserciones y borrados se realizan al final y en esto es el más eficiente, salvo cuando se necesita aumentar su capacidad interna, sin embargo esto ocurre rara vez. La otra alternativa hubiera sido el uso de list, pero internamente es una lista <i>doblemente</i> enlazada lo cual hace que las inserciones y eliminaciones sean más lentas que en un vector. (Los vectores de la STL cambian de tamaño dinámicamente pero no se encuentran en el heap). Se utilizan set y multiset cuando se necesita que los elementos esten ordenados.
 
 Es un hecho que un código eficiente en memoria o tiempo tiende a ser menos legible, se ha intentado mejorar la legibilidad a lo largo de la libreria, sin embargo sí se han usado ciertas expresiones que ameritan explicación:
 
 Una técnica usada a lo largo de la librería para evitar el copiado de datos de contenedores es la siguiente, en vez de hacer \code
 contenedor1 = contenedor2; \endcode
 se ha utilizado el método swap de uno de los contenedores, de esta manera \code
 contenedor1.swap( contenedor2 ); \endcode
 esto es menos legible y puede parecer menos eficiente ya que equivaldría a: \code
 aux = contenedor1;
 contenedor1 = contenedor2;
 contenedor2 = aux; \endcode
 Sin embargo la especificación de la STL indica que lo que en realidad se intercambia con el método swap no son los elementos de los contenedores, sino simplemente los iteradores de principio y fin de ambos (es decir seis asignaciones de tipos atómicos en vez de copiar todos los elementos de un contenedor a otro). El hecho de que el segundo contenedor quede con los datos del primero resulta ser irrelevante debido a que no se vuelve a utilizar tal contenedor en los algoritmos, o porque lo que corresponde, en los algoritmos, es vaciarlo. La alternativa hubiera implicado: 1) que en algun momento se hubiera tenido una copia de un contenedor, ocupando el doble de memoria de la requerida. 2) se hubiera requerido que los estados necesiten un copy-constructor.
 
 Esta otra expresión tampoco es muy clara, se usa para ejecutar una operacion a un estado para obtener el estado sucesor: \code
 if ( ! (*(*operacion))(estado, resultante) )
    ...; \endcode
 Esto es lo que hace posible que la misma función funcione con functores y funciones, en el caso de un contenedor de punteros a funciones implica la dereferencia de un iterador (llamado 'operacion'), de un puntero a función y la llamada a la función apuntada, en el caso de un contenedor de punteros a functores implica la dereferencia de ese mismo iterador, de un puntero a un objeto y la llamada al \c operator() del objeto apuntado. En ambos casos la negacion implica que el cuerpo del \c if se ejecutará si NO es posible realizar la operación en el estado dado, en caso contrario el estado resultante estará almacenado en \c resultante.
 
 Esta otra expresion se usa para notificar a la funcion callback de una solucion hallada: \code
 if ( ! registrar_solucion || ! (*registrar_solucion)(ruta, actual) )
    ...; \endcode
 Aprovechamos el short-circuit de las expresiones booleanas de C++ para invocar al callback solamente si no es nulo y realizar el cuerpo del if solamente si no había funcion callback o habiendo sido ejecutada devolvió falso.
 \section estilo Una nota sobre el estilo
 El código fuente debe ser legible. Muchas lineas de código exceden el ancho de la pantalla (o de la página) se ha preferido cortar las lineas bajo esta premisa básica: una linea que lógicamente debería ser parte de la anterior SIEMPRE empezará con un símbolo de puntuación. Aunque esto no es lo que más a menudo se usa en otros programas, ayuda mucho a entender rápidamente el código, comparese este pedazo de código, que sigue el esquéma típico (no usado en esta librería): \code
  template < typename Estado_t, typename Operaciones_t>
  vector< typename Operaciones_t::value_type> profundidad_limitada(const Operaciones_t& operaciones, 
      const Estado_t& inicial, const unsigned int& limite_profundidad, bool(*registrar_solucion) ( 
      const vector<typename Operaciones_t::value_type>&, const Estado_t& ) = NULL )  
   {
      IA_REINICIAR_ESTADISTICAS;
      set<Estado_t> visitados;
      visitados.insert( inicial );
      ... 
   }\endcode 
  con el mísmo código siguiendo el esquema usado en esta librería \code
  template < typename Estado_t, typename Operaciones_t>
  vector< typename Operaciones_t::value_type> profundidad_limitada(const Operaciones_t& operaciones
               , const Estado_t& inicial
               , const unsigned int& limite_profundidad
               , bool(*registrar_solucion)( const vector<typename Operaciones_t::value_type>&
                                          , const Estado_t& ) = NULL ) {
      IA_REINICIAR_ESTADISTICAS;
      set<Estado_t> visitados;
      visitados.insert( inicial ); 
   ...
   }\endcode
   En el de más abajo resulta evidente que los parámetros de tipo const vector<>& y const Estado corresponden a la funcion recibida \c registrar_solucion, en el de más arriba darse cuenta de eso requiere un análisis de las lineas anteriores.
   El mismo caso se presenta en las sentencias for: \code
   for( typename Operaciones_t::const_iterator paso = solucion.begin(); paso != solucion.end();
        ++ paso)
       anterior = actual;
   actual = X; \endcode
   
   ¿ ++paso era parte del cuerpo del for?, aquí se ve mas claramente que ++paso no es el cuerpo del for, especialmente por el punto y coma: \code
   for( typename Operaciones_t::const_iterator paso = solucion.begin(); paso != solucion.end()
                                                                      ; ++ paso)
       anterior = actual;
   actual = X; \endcode
*/

/** \example cantaros.cpp
 Usa punteros a funciones para especificar las operaciones realizables sobre los estados. Demuestra cómo poner nombres a las operaciones que estan dadas como punteros a funciones.
 
 Lo único que usa de la STL es el tipo \c vector y el \c map, sobre el vector solo usa el metodo \c push_back que sirve para aumentar un valor más al vector, y del \c map usa el operador corchetes ( \c operator[] ), que hace que un \c map se pueda usar más o menos como un vector.
 
 Utiliza las búsquedas de preferencia en profundidad, profundidad limitada, preferencia por amplitud y profundidad iterativa.
 
  Resuelve el problema de los cántaros, en el que se tienen dos cántaros, no aforados (sin marcas de medición), El cántaro A tiene 3 litros de capacidad, y B 4 litros. Tambien hay una pila abierta, y vale mojar el piso. Al principio ambos cántaros estan vacíos. Hay que llenar el cantaro B con exactamente 2 litros.
 */

/** \example pastor.cpp
 Usa punteros a funciones para especificar las operaciones realizables sobre los estados. Demuestra cómo poner nombres a las operaciones que estan dadas como punteros a funciones.
 
 Lo único que usa de la STL es el tipo \c vector y el \c map, sobre el vector solo usa el metodo \c push_back que sirve para aumentar un valor más al vector, y del \c map usa el operador corchetes ( \c operator[] ), que hace que un \c map se pueda usar más o menos como un vector.
 
 Utiliza las búsquedas de preferencia por amplitud, profundidad limitada y profundidad iterativa.

 Resuelve el problema del pastor, el lobo, la oveja y el pasto:
   El hombre (pastor), el lobo, la oveja y el pasto (forraje) están en la orilla izquierda del rio.
   El pastor tiene una canoa en que solo cabe él y una cosa más (ya sea el lobo, la oveja o el forraje)
   Si deja solos al lobo y a la oveja, muere la oveja; 
   Si deja solos a la oveja y al forraje, se acaba el forraje.
   Debe llevar TODO, sano y salvo, a la otra orilla.
 */
 
/** \example 8puzzle.cpp
 Usa functores (objetos-función) para especificar las operaciones sobre los estados. Cada movimiento (operacion) es una instancia de la clase Mover.
 
 Ademas de usar \c vector y \c map y \c set de la STL (\c set es un conjunto ordenado en que no pueden repetirse los elementos) usa los algoritmos \c lexicographical_compare que compara dos contenedores a modo de diccionario, es decir que si ambos son identicos solo que uno de los contenedores tiene menos elementos, este está primero. Tambien se usan \c copy y \c back_inserter que juntos añaden el contenido del primer contenedor al final del segundo. \c random_shuffle desordena de manera aleatoria todo el contenido de un contenedor.

 Utiliza la búsqueda de profundidad limitada
 */

/** \example reinas.cpp
 Usa functores (objetos-función) para especificar las operaciones sobre los estados. También demuestra como utilizar el parámetro opcional \c registrar_solucion para encontrar todas las soluciones al problema.
 
 De la STL usa \c vector y \c pair (pair es una estructura simple con dos miembros de, el primero se llama \c first y el segundo \c second)
 
 Utiliza la búsqueda por profundidad limitada
*/

/** \example laberinto.cpp
 Usa punteros a funciones para encontrar la solucion a un laberinto hard-coded en el programa, demuestra el uso de la búsqueda bidireccional especificando los operadores inversos y dejando que el auxiliar \c OperadoresInversos los genere, tambien demuestra a \c simplificar_bidireccional para convertir una solución de dos caminos a una de un solo sentido.
 
 De la STL usa \c vector y \c pair. Tambien la función auxiliar \c make_pair que permite construir un pair sin utilizar variables auxiliares.
 
 Utiliza búsqueda bidireccional
*/
/** \example laberinto2.cpp
 Usa punteros a funciones para encontrar la solucion a un laberinto ingresado por entrada estandar, poniendo
 la solucion en salida estandar.  Demuestra el uso del contenedor \c SecuenciaEstados para convertir las operaciones devueltas como solucion en los estados intermedios
 
 Una buena manera de usarlo es poner el laberinto en un archivo de texto. Tal archivo debe usar el caracter # para las paredes, la entrada del laberinto es el único espacio de la primera fila y la salida el único espacio en en la última fila. La primera y última columna consisten de solo paredes (#)
 
 Si el laberinto esta en el archivo laberinto.txt y se quiere la solucion en el arcivo solucion.txt
 en la linea de comandos del sistema ponga:
 \code
 c:\...> type laberinto.txt | laberinto2.exe > solucion.txt
 \endcode
 El archivo de solucion consiste en el mismo laberinto pero con el caracter + marcando el camino.
*/

/** \example viajero.cpp
 Demuestra el uso de \c primero_mejor y un functor (\c Elige_mejor_ciudad) para hallar el camino entre dos ciudades. Las operaciones son functores (objetos-función) instancias de la clase Viajar.
 
 Ademas de la búsqueda de primero el mejor, usa las búsquedas por profundidad limitada y preferencia amplitud.
*/