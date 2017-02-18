// Este archivo no contiene c�digo fuente, est� solamente para generar la p�gina inicial de la 
// documentacion en HTML. Se lo puede borrar con seguridad.
/** \mainpage Documentaci�n de IA Ciega
 El objetivo de esta libreria es facilitar el uso de los Algoritmos de B�squeda usados en Inteligencia Artificial de manera que una vez que se tengan definidos los estados y las operaciones sea sencillo probar los distintos algoritmos sin tener que implementarlos.
 
 \section intro Introducci�n
 La librer�a esta dise�ada para permitir un uso sencillo sin sacrificar la flexibilidad. Se han usado plantillas,
 pero para llamar a las funciones no es necesario especificar los tipos de las plantillas, excepto en \c primero_mejor.
 
 La corrida deber�a ser muy r�pida porque siempre que es posible uso par�metros por referencia, ademas, me apoyo en la STL
 y plantillas para los algor�tmos y contenedores necesarios.

 \section uso Modo de uso
 
  Para traer todas las funciones al scope de trabajo, evitando poner ia::... a cada rato, puedes poner:
 \code
 using namespace ia;
 \endcode
 al principio de tu programa, despu�s de incluir las cabeceras que necesites.
 
   Para una visi�n general de la libreria ve las pesta�as P�ginas relacionadas, M�dulos, y Ejemplos, 
  Cuando estes dispuesto a usar la librer�a mir� el \ref uso_general y luego alguno de los \ref algoritmos, tambien mir� los \ref consejos. Recuerda ver los ejemplos para ver el uso pr�ctico y ante cualquier duda, el m�s sencillo es el pastor.cpp, los mas complejos el 8puzzle.cpp y reinas.cpp.
 
 \section compatibilidad Compatibilidad y compilaci�n
 Esta librer�a consiste solo de un archivo cabecera: ia_ciega.h, as� que no requiere compilaci�n de librer�a.
 Se usa intensivamente la STL y plantillas, pero no se usa meta-programaci�n, as� que cualquier C++ contemporaneo deber�a ser suficiente.
 Se ha probado con �xito usando:
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
 
  En el caso del compilador gratuito (MS VC++ Toolkit) no ser� posible usar el cronometraje autom�tico porque tal herramienta no incluye windows.h. 
 
 No se puede usar con:
 <ul>
 <li>Borland C++ 7.0 para DOS
 <li>DJGPP
 </ul>
 Son todos los compiladores con los que se ha probado, si alguno no esta en la lista significa que todav�a no se ha probado y que podr�a funcionar.
 \section instalacion Instalaci�n
 La instalaci�n es solamente copiar el archivo ia_ciega.h a una carpeta donde la encuentre el compilador, esa carpeta suele llamarse "include".
 
 \section limitaciones Caracter�sticas y Limitaciones 
 <ul>
 <li><b>Reentrancia: </b> En una aplicacion con m�s de un hilo debe evitar que el <i>mismo tipo de b�squeda (o sea la misma funci�n de b�squeda con los mismos par�metros de plantilla)</i> este siendo realizado por dos hilos a la vez, debe considerarse tambi�n el mismo tipo de b�squeda a profundidad_limitada y profundidad_iterativa. Si se busca con las mismas funciones pero con diferentes parametros de plantilla no hay problemas de reentrancia.
 <li>La capacidad de cronometrar la ejecucion solo es posible en plataforma MS Windows, en otras debe deshabilitarse con \#define IA_NO_CRONOMETRAR
 <li>Actualmente solo se ha compilado en plataforma MS Widnows, pero debe poderse en otras ya que no se usan caracter�sticas espec�ficas de ese sistema (salvo en el cronometraje).
 </ul>
 
 \section Pendientes
 
 <ul>
 <li>Crear contenedores con caracter�sticas adicionales para esta libreria, como ser secuencias con allocators para memoria paginada en disco (con eso ser�a posible su uso en b�squedas en espacios inmensos)
 <li>Hacer una comparaci�n estad�stica pr�ctica con otras librerias de b�squeda, tanto en tiempo como en memoria.
 <li>Implementar otros algor�tmos de b�squeda
 </ul>
*/
 
*/

/** \page uso_general Uso general
 Las funciones de b�squeda estan en el namespace ::ia y son as�: (las que estan en el namespace ::ia::detalle son internas y no es necesario llamarlas directamente)
 \code template< [...] >
 nombre_funcion(const Operaciones_t &operaciones, const Estado_t &inicial, [...], (bool*)registrar_solucion( [...] ) ); \endcode
 No te preocupes por lo de \c template, para llamar a las funciones no necesitas especificar ninguno de esos tipos, eso se har� autom�ticamente (excepto con Microsoft VC Toolkit y VC NET 2003). El primer par�metro siempre es una \ref operaciones y el segundo un \ref estados.
 \section operaciones Lista de operaciones
 Las funciones de b�squeda reciben siempre como primer par�metro un contenedor secuencial de las operaciones que
 se pueden aplicar a los estados para expandirlos. Esta secuencia puede ser una secuencia (p.e. un \c vector) de
 punteros a las funciones que se pueden usar para hacer expansiones, tambi�n puede ser una secuencia (p.e. un
 \c vector) de punteros a una clase base (normalmente \c ia::Operacion<Estado>) cuyos herederos definen al operador
 de par�ntesis ( \c operator() ). Ten en cuenta que el uso de una clase base para las operaciones implica una llamada
 a una funci�n virtual (al \c operator() ) y esto equivale a dos indirecciones, es preferible utilizar una sola clase cuyas distintas instancias ejecuten de distinta manera a su \c operator()
 
 \subsection funciones Operaciones como funciones
 La lista de operaciones para expandir estados puede ser una secuencia de punteros a funciones (en este caso las funciones son \c mi_operacion1 y \c mi_operacion2), as�:
 \code
 #include "ia_ciega.h"
 using namespace std;
 struct Estado {
    // Despu�s se indica c�mo debe ser esta estructura ...
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
     // llamar a la funci�n de b�squeda deseada...
     mostrar_solucion( inicial, profundidad_iterativa( operaciones, inicial, 11), nombres );
     // etc...
 }
 \endcode
 
 Fijate c�mo usamos las estructuras auxiliares \c Operaciones y \c NombresOperadores dando el par�metro Estado.
 
 Cada una de las funciones mi_operacion1, mi_operacion2, etc, reciben como primer estado el estado actual y
 deben poner en el segundo parametro es estado resultante de aplicar la operacion. Si no se puede aplicar la
 operaci�n debe devolver falso, si se puede aplicar debe devolver verdadero. Cuando devuelve falso, no se usar�
 el valor del segundo par�metro fuera de esta funcion, as� que se puede dejar como basura.
 
 \subsection functores Operaciones como clases (functores)
 Existen dos maneras de trabajar: definir una clase distinta para cada operaci�n (considere usar punteros 
 a funciones), la segunda es la que se explicar� aqu�, a saber, una sola clase para todas las operaciones, el 
 comportamiento espec�fico de la operaci�n se establece en el constructor de la clase.
  
 La lista de operaciones para expandir estados puede ser una secuencia de punteros a una clase. En la lista de operaciones
  se ponen punteros objetos de esta clase, cada instancia ejecuta el \c operator() de acuerdo a la 
  operacion que realiza, as�:
  
  \code
  struct Estado {
     // Despu�s se indica c�mo debe ser esta estructura...
  };
  
  class Accion : public Operacion<Estado> {
     ... // aqu� puedes poner los miembros datos de la operacion
  public:
     Accion(...) {...}
     bool operator()(const Estado_t& antes, Estado_t& despues) const {
        ... // Aplica la operacion de acuerdo a sus miembros dato
     }
     
  }
  \endcode

 
 En el constructor se ajustan los miembros datos del objeto, que son los par�metros que definen c�mo se realizar� la operaci�n
 
 La clase base utilizada \c Operacion<Estado> est� definida en
 esta librer�a y contiene un miembro dato \c nombre en el que se puede poner el nombre de la operaci�n, as� como
 deber�a ser mostrada en la soluci�n, alternativamente se puede re-implementar la funcion \c get_descripci�n()
 para mostrar la operacion. Todo esto es opcional y solo es requerido si se utiliza la funci�n mostrar_solucion
 de esta libreria.
 
 No es necesario heredar de \c Operacion<Estado>; pero, se herede o no, se debe declarar el \c operator() de esta manera en la clase base (en Operacion<Estado> ya esta declarada), y/o implementarla en cada clase (heredera):
 \code
 virtual bool operator()(const Estado_t& antes, Estado_t& despues) const 
 \endcode
 
 este operador debe cumplir con los requisitos especificados anteriormente para las operaciones como funciones,
 con el requerimiento adicional de que no deben modificar los miembros datos del objeto (por eso es un 
 m�todo \c const ). Si crees que necesitas cambiar algun miembro dato ten en cuenta que el orden en que se llama
 al \c operator() es indefinido, (para que se pueda cambiar un miembro dato \c m de tipo \c Tipo este deber�a ser
 declarado como \c mutable, as�: \c mutable \c Tipo \c m; )
 
 Las funciones de b�squeda solo utilizaran los objetos pasados en la lista de operaciones, no crear�n nuevas instancias.
 
 El ejemplo de las 8 reinas (reinas.cpp) hace uso de esta t�cnica, dandole en el constructor la fila en que
 debe poner una reina cada instancia de una misma clase.
 
 \section estados Estado inicial
 El segundo par�metro de las funciones de b�squeda es el estado inicial desde el que se empieza la b�squeda.
 
 Un estado es una estructura (o clase) con esta interfaz:
 \code
 struct Estado {
   // ... aqu� puedes poner los atributos de tu estado
   Estado() {...} // Un constructor por defecto, Normalmete no har� nada
   ... // aqu� puedes poner otros constructores
   // Debe devolver true ssi es un estado meta
   bool es_meta() const {
      ...
   }
   // Un operador menor que, de orden debil estricto, para que pueda estar en un std::set
   bool operator<(const Estado& otro) const {
      ...
   }
   // Devuelve una cadena con la especificaci�n del estado (opcional)
   friend ostream& operator<<(ostream& os, const Estado& e ) {
      os << ... ;
      return os;
   }
 };

 \endcode
 El constructor por defecto y todas estos m�todos (excepto el \c operator<<) deber�an terminar r�pido porque son
 llamados varias veces durante las b�squedas. Los estados construidos con el constructor por defecto 
 se usan solamente para ser el segundo par�metro de las funciones que se usan para expandir estados (o 
 del \c operator() si son functores) as� que su inicializaci�n puede hacerse en la funci�n u \c operator(),
 
 \c es_meta() se llama en todos los estados generados.
 
 \c operator< se usa para poner al estado en un \c std::set de estados visitados, debe ser un strict weak ordering
 como se define en la STL, es decir que si a < b entonces es falso que b < a, y si a < b y b < c entonces a < c. Dos
 estados A y B se considerar�n equivalentes (es decir solo se explorar� uno) si es falso que A < B y es falso que B < A.
 
 \c operator<< se usa en mostrar_solucion con dos par�metros, as� que no afecta a la velocidad de las funciones
 de b�squeda
 
 \section registrar_solucion Uso del par�metro registrar_solucion
 Los algor�tmos de b�squeda reciben, opcionalmente, un par�metro registrar_solucion que es un 
 puntero a una funci�n que se llamar� cada vez que se encuentre una soluci�n. Es una funci�n "callback" 
 aqu� se describe c�mo se usa.
 
 La funcion puede ser as�
 \code
 bool registrar_solucion(const vector<X>& ruta, const Estado& estado)
 \endcode
 donde X es el tipo de las operaciones puestas inicialmente en el contenedor de operaciones que se pueden aplicar
 (que pueden ser punteros a objetos funcion (o sea, a functores) o punteros a funciones).
 
 Todas las b�squedas terminar�n al encontrar la primera soluci�n, si desea continuar la b�squeda de m�s soluciones debe implementar esta funci�n y pasar un puntero a ella a la funci�n de b�squeda que use, la b�squeda llamar� a su funci�n cada vez que halle una soluci�n. Si desea que la b�squeda continue devuelva true, si desea terminarla devuelva false en este �ltimo caso la funci�n de b�squeda originalmente llamada devolver� la soluci�n que se acaba de hallar; por otra parte, si su funci�n registrar_solucion siempre devuelve true eventualmente la funci�n de b�squeda terminar� devolviendo una ruta vac�a.
 
 En el primer par�metro recibir� un vector con punteros a las operaciones que se hicieron para encontrar la meta
 reci�n hallada; en el segundo par�metro estar� el estado meta que se hall�.
 
 Por ejemplo, si las operaciones se dieron usando el tipo auxiliar Operaciones<Estado> (o sea, usando punteros a funciones) entonces ruta sera de tipo \c vector<bool(*)(const Estado&, Estado&)> , es decir un vector de punteros a las funciones que se aplicaron al estado inicial para llegar al estado meta dado como segundo par�metro.

 <hr>
 
 Vaya a la pesta�a M�dulos y elija Algoritmos de b�squeda para obtener ayuda 
 sobre una funci�n de b�squeda en particular
 */
 
/** \page consejos Consejos pr�cticos
 Recuerda poner \c using \c namespace \c ia, o empezar todos los usos de identificadores de la libreria con \c ia::
 
 Es muy importante que las funciones de operaciones o los m�todos operator() reciban el primer par�metro como "const Tipo& antes" y el segundo como "Tipo& despues" donde Tipo es el tipo de los estados. No se debe modificar el primer par�metro!.
 
 La clase (o estructura) que uses como Estado debe ocupar poca memoria, su constructor por defecto debe ser r�pido, preferiblemente vac�o.
 
 Las funciones para aplicar operaciones deben ser rapid�simos. (Si utilizas objetos-funcion, el \c operator() debe ser rapid�simo)
 
 Si utilizas functores, deben ocupar poca memoria. Si necesitas muchos datos en los functores considera ponerlos en memoria global y mantener solo una referencia a esos datos en los functores.
  
*/

/** \page preprocesador Uso de macros para deshabilitar estad�sticas y registro de soluciones.
 La librer�a puede obtener algunas m�tricas sobre la cantidad de estados visitados, expandidos, etc esto reduce un poco la velocidad de la b�squeda. Para evitar el c�lculo de m�tricas ponga la linea:
 \code #define IA_NO_ESTADISTICAS 
 \endcode
 antes de \# incluir esta librer�a.
 
 Para evitar que se tomen m�tricas de tiempo de ejecuci�n ponga la l�nea
 \code #define IA_NO_CRONOMETRAR
 \endcode
 antes de # incluir esta ibrer�a. Es necesario definir esta macro si no est� compilando en plataforma Windows.
 
 La precisi�n de las m�tricas de tiempo tienen un error de (+/-)8 milisegundos
 
 Si se definen ambas macros el c�lculo de estad�sticas no se llevar� a cabo y las b�squedas se har�n un poco mas r�pido, la definici�n de solo una tambi�n aumenta la velocidad.
 
 Las funciones de b�squeda reciben un par�metro opcional \c registrar_solucion que puede ser llamado cada vez que se halla una solucion y permite continuar la busqueda para encontrar todas las soluciones; para deshabilitar esta caracter�stica (y as� aumentar un poco la velocidad de los algor�tmos) ponga la linea: \code #define IA_NO_REGISTRAR_SOLUCION \endcode antes de \# incluir esta librer�a. De todas maneras se podr� pasar el par�metro registrar_solucion a las funciones, pero no se utilizar�, esto es as� para que el programador usuario no deba modificar su propio c�digo fuente.
*/

/** \page rationale Rationale
<b>Se usan otras funciones para llamar a las que estan en el namespace ia::detalle</b> porque las que estan en detalle necesitan m�s par�metros que los que el llamador quisiera llenar con informaci�n de su propio problema, por otra parte el paso de par�metros en la primera llamada a las funciones de detalle deben cumplir varios requerimientos que al llamador solamente le dificultarian su uso.

<b>Se prefiere usar plantillas y no usar funciones virtuales para la realizaci�n de operaciones y la comparaci�n de estados</b> porque una llamada a una funci�n virtual es un poco mas lenta que la llamada a funciones no-virtuales. Esto tiene m�s importancia porque la funci�n virtual ser�a llamada en un ciclo interno durante las b�squedas

<b>Se usa el lenguaje C++</b> porque tiene las siguientes caracter�sticas necesarias para esta librer�a, y no se encuentran en otros lenguajes: 1) Plantillas, no solo a nivel de contenedores, si no de operadores, por ejemplo para usar la misma funci�n con functores y funciones. 2) Preprocesador, para poder deshabilitar capacidades y as� ganar velocidad. 3) Generaci�n de c�digo eficiente, debido a que el c�digo es ejecutado directamente por el hardware. 4) Librerias estandarizadas con variedad de estructuras de datos, algoritmos ortogonales a las estructuras de datos.

<b>Se utilizan macros del preprocesador para deshabilitar el c�lculo de estad�sticas y registro de soluciones</b> porque de esta forma una vez deshabilitadas permitir�n un funcionamiento tan r�pido como si nunca hubiera habido posibilidad de utilizar tales caracter�sticas. (No es necesario evaluar un booleano para tomar la decisi�n de usarlas).

Para ver detalles espec�ficos de la implementacion consulte la siguiente secci�n \ref detalles y el c�digo fuente, que tiene muchos comentarios explicativos que no aparecen en este manual.
*/

/** \page preguntas Preguntas Hechas Frecuentemente
(En realidad nadie hizo preguntas, pero estas respuestas dan informaci�n importante que no cabe en otros sitios de este manual)
<ul>
<li> <i> �No ser�a m�s f�cil hacer mi propia funci�n de b�squeda en vez de aprender a usar toda esta librer�a? </i>.
No creo. Son algoritmos dif�ciles de implementar, mas b�en ser�a bueno empezar usando esta librer�a (u otra) y, despu�s de comprobar que tus estados y operaciones funcionan, crear tus propias funciones de b�squeda. No tienes que aprender a usar toda la libreria, por ejemplo si no has usado mucho functores o la STL puedes limitarte a usar una funci�n por cada operaci�n.
<li><i> �Tengo que leer todo el c�digo fuente para aprender a usar esta librer�a? </i> No, basta con leer los comentarios del c�digo fuente, estos se presentan en forma m�s legible en la documentacion HTML incluida en el CD de la librer�a.
<li> <i> He visto el programa fuente y se usan caracter�sticas avanzadas de C++ (plantillas, STL, functores, etc.) �Necesito aprender todo eso para usar esta librer�a? </i> No. Mas bien hay que empezar por los ejemplos. Especialmente el de los c�ntaros, el laberinto y el pastor usan muy poco de la STL y lo m�nimo de plantillas, si tienes problemas con eso te aconsejo hacer uso de la libreria empezando sobre uno de los ejemplos, en vez de empezar en blanco.
<li><i> �Puedo usar este programa en un producto comercial?</i> Si. Pero si tu producto incluye el c�digo fuente tienes que incluir el c�digo fuente de esta librer�a y tambi�n el archivo licencia_1_0.txt en la instalaci�n y no borrar mi nombre del c�digo fuente (tu producto puede utilizar otra licencia, la mia solo hara referencia a mi porci�n de c�digo). Puedes incluir tu nombre en ia_ciega.h si a�adiste algo a la librer�a, pero en ning�n caso debes hacer creer que t� hiciste el c�digo que yo hice �Obvio no?. Si tu producto est� solo en forma binaria (sin c�digo fuente) no es necesario que incluyas el archivo de licencia ni que aparezca mi nombre, una peque�a menci�n en "Acerca de" ser�a un bonito detalle de tu parte, tampoco debes hacer creer que t� hiciste la parte que yo hice.
</ul>

*/

/** \page detalles Detalles de implementacion
En esta p�gina se explican detalles �tiles solamente a aquellos que quieran modificar o mejorar esta librer�a, o quienes esten interesados en sus detalles. Se supone un conocimiento pr�ctico de los contenedores estandar de C++ (parte de la STL) y una lectura al resto de este documento, especialmente la seccion Rationale.

La librer�a esta dividida en tres partes conceptuales: la interfaz, los algoritmos y los auxiliares (tanto estructuras de datos como funciones).

<ol>
<li>La <b>interfaz</b> comprende todo aquello a lo que el programador usuario puede acceder, esto es, funciones y tipos que se encuentran en el namespace ::ia, se puede usar todas las capacidades de esta libreria con solo ese conocimiento. Pertenecen a esta parte ia::profundidad_limitada_doble, ia::profundidad_iterativa, ia::mostrar_solucion, el tipo ia::Operacion, etc.


<li>Los <b>algoritmos</b> se encuentran ocultos en el namespace ia::detalle y hay uno por cada m�todo de b�squeda que aparece en la interfaz. Pertenecen a esta parte ia::detalle::profundidad_limitada_doble, ia::detalle::profundidad_iterativa


<li>Los <b>auxiliares</b> tambien estan en el namespace ia::detalle y son utilizados tanto por los algoritmos como por la interfaz. Tenga en cuenta que en el resto de este documento se ha usado el t�rmino auxiliares haciendo referencia a ciertas funciones/tipos de la <i>interfaz</i> que no pertenecen a esta categor�a conceptual.
</ol>

\section detalle_interfaz Detalles sobre la interfaz
El uso de la interfaz se ha explicado en el resto de este documento, aqu� se explica c�mo esta implementada.

Las funciones de b�squeda de la interfaz tienen el prop�sito de que los algoritmos reciban la menor cantidad de par�metros posibles, y permitir que el compilador elija el tipo de los par�metros de plantilla necesarios, as� como a adaptar las llamadas de acuerdo a la macro IA_NO_REGISTRAR_SOLUCION. 

T�mese el c�digo de la funcion de interfaz preferencia_amplitud como ejemplo
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

En esas lineas, en base a los par�metros recibidos se inicializan las variables requeridas por las funciones de algoritmos, se inician los contadores estad�sticos y el cronometraje

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

finalmente llama a la funci�n de algoritmo interna con los par�metros que ella requiere, usando al preprocesador envian el par�metro registrar_solucion solamente si no esta definida la macro IA_NO_REGISTRAR_SOLUCION. Termina por devolver lo mismo que la funci�n interna.

\section detalle_algoritmo Detalles sobre la implementaci�n
 Siempre se ha minimizado el uso de variables locales en los algoritmos recursivos.
 
 Internamente se utiliza el contenedor vector<> para almacenar datos que no se necesita ordenar, porque todas las inserciones y borrados se realizan al final y en esto es el m�s eficiente, salvo cuando se necesita aumentar su capacidad interna, sin embargo esto ocurre rara vez. La otra alternativa hubiera sido el uso de list, pero internamente es una lista <i>doblemente</i> enlazada lo cual hace que las inserciones y eliminaciones sean m�s lentas que en un vector. (Los vectores de la STL cambian de tama�o din�micamente pero no se encuentran en el heap). Se utilizan set y multiset cuando se necesita que los elementos esten ordenados.
 
 Es un hecho que un c�digo eficiente en memoria o tiempo tiende a ser menos legible, se ha intentado mejorar la legibilidad a lo largo de la libreria, sin embargo s� se han usado ciertas expresiones que ameritan explicaci�n:
 
 Una t�cnica usada a lo largo de la librer�a para evitar el copiado de datos de contenedores es la siguiente, en vez de hacer \code
 contenedor1 = contenedor2; \endcode
 se ha utilizado el m�todo swap de uno de los contenedores, de esta manera \code
 contenedor1.swap( contenedor2 ); \endcode
 esto es menos legible y puede parecer menos eficiente ya que equivaldr�a a: \code
 aux = contenedor1;
 contenedor1 = contenedor2;
 contenedor2 = aux; \endcode
 Sin embargo la especificaci�n de la STL indica que lo que en realidad se intercambia con el m�todo swap no son los elementos de los contenedores, sino simplemente los iteradores de principio y fin de ambos (es decir seis asignaciones de tipos at�micos en vez de copiar todos los elementos de un contenedor a otro). El hecho de que el segundo contenedor quede con los datos del primero resulta ser irrelevante debido a que no se vuelve a utilizar tal contenedor en los algoritmos, o porque lo que corresponde, en los algoritmos, es vaciarlo. La alternativa hubiera implicado: 1) que en algun momento se hubiera tenido una copia de un contenedor, ocupando el doble de memoria de la requerida. 2) se hubiera requerido que los estados necesiten un copy-constructor.
 
 Esta otra expresi�n tampoco es muy clara, se usa para ejecutar una operacion a un estado para obtener el estado sucesor: \code
 if ( ! (*(*operacion))(estado, resultante) )
    ...; \endcode
 Esto es lo que hace posible que la misma funci�n funcione con functores y funciones, en el caso de un contenedor de punteros a funciones implica la dereferencia de un iterador (llamado 'operacion'), de un puntero a funci�n y la llamada a la funci�n apuntada, en el caso de un contenedor de punteros a functores implica la dereferencia de ese mismo iterador, de un puntero a un objeto y la llamada al \c operator() del objeto apuntado. En ambos casos la negacion implica que el cuerpo del \c if se ejecutar� si NO es posible realizar la operaci�n en el estado dado, en caso contrario el estado resultante estar� almacenado en \c resultante.
 
 Esta otra expresion se usa para notificar a la funcion callback de una solucion hallada: \code
 if ( ! registrar_solucion || ! (*registrar_solucion)(ruta, actual) )
    ...; \endcode
 Aprovechamos el short-circuit de las expresiones booleanas de C++ para invocar al callback solamente si no es nulo y realizar el cuerpo del if solamente si no hab�a funcion callback o habiendo sido ejecutada devolvi� falso.
 \section estilo Una nota sobre el estilo
 El c�digo fuente debe ser legible. Muchas lineas de c�digo exceden el ancho de la pantalla (o de la p�gina) se ha preferido cortar las lineas bajo esta premisa b�sica: una linea que l�gicamente deber�a ser parte de la anterior SIEMPRE empezar� con un s�mbolo de puntuaci�n. Aunque esto no es lo que m�s a menudo se usa en otros programas, ayuda mucho a entender r�pidamente el c�digo, comparese este pedazo de c�digo, que sigue el esqu�ma t�pico (no usado en esta librer�a): \code
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
  con el m�smo c�digo siguiendo el esquema usado en esta librer�a \code
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
   En el de m�s abajo resulta evidente que los par�metros de tipo const vector<>& y const Estado corresponden a la funcion recibida \c registrar_solucion, en el de m�s arriba darse cuenta de eso requiere un an�lisis de las lineas anteriores.
   El mismo caso se presenta en las sentencias for: \code
   for( typename Operaciones_t::const_iterator paso = solucion.begin(); paso != solucion.end();
        ++ paso)
       anterior = actual;
   actual = X; \endcode
   
   � ++paso era parte del cuerpo del for?, aqu� se ve mas claramente que ++paso no es el cuerpo del for, especialmente por el punto y coma: \code
   for( typename Operaciones_t::const_iterator paso = solucion.begin(); paso != solucion.end()
                                                                      ; ++ paso)
       anterior = actual;
   actual = X; \endcode
*/

/** \example cantaros.cpp
 Usa punteros a funciones para especificar las operaciones realizables sobre los estados. Demuestra c�mo poner nombres a las operaciones que estan dadas como punteros a funciones.
 
 Lo �nico que usa de la STL es el tipo \c vector y el \c map, sobre el vector solo usa el metodo \c push_back que sirve para aumentar un valor m�s al vector, y del \c map usa el operador corchetes ( \c operator[] ), que hace que un \c map se pueda usar m�s o menos como un vector.
 
 Utiliza las b�squedas de preferencia en profundidad, profundidad limitada, preferencia por amplitud y profundidad iterativa.
 
  Resuelve el problema de los c�ntaros, en el que se tienen dos c�ntaros, no aforados (sin marcas de medici�n), El c�ntaro A tiene 3 litros de capacidad, y B 4 litros. Tambien hay una pila abierta, y vale mojar el piso. Al principio ambos c�ntaros estan vac�os. Hay que llenar el cantaro B con exactamente 2 litros.
 */

/** \example pastor.cpp
 Usa punteros a funciones para especificar las operaciones realizables sobre los estados. Demuestra c�mo poner nombres a las operaciones que estan dadas como punteros a funciones.
 
 Lo �nico que usa de la STL es el tipo \c vector y el \c map, sobre el vector solo usa el metodo \c push_back que sirve para aumentar un valor m�s al vector, y del \c map usa el operador corchetes ( \c operator[] ), que hace que un \c map se pueda usar m�s o menos como un vector.
 
 Utiliza las b�squedas de preferencia por amplitud, profundidad limitada y profundidad iterativa.

 Resuelve el problema del pastor, el lobo, la oveja y el pasto:
   El hombre (pastor), el lobo, la oveja y el pasto (forraje) est�n en la orilla izquierda del rio.
   El pastor tiene una canoa en que solo cabe �l y una cosa m�s (ya sea el lobo, la oveja o el forraje)
   Si deja solos al lobo y a la oveja, muere la oveja; 
   Si deja solos a la oveja y al forraje, se acaba el forraje.
   Debe llevar TODO, sano y salvo, a la otra orilla.
 */
 
/** \example 8puzzle.cpp
 Usa functores (objetos-funci�n) para especificar las operaciones sobre los estados. Cada movimiento (operacion) es una instancia de la clase Mover.
 
 Ademas de usar \c vector y \c map y \c set de la STL (\c set es un conjunto ordenado en que no pueden repetirse los elementos) usa los algoritmos \c lexicographical_compare que compara dos contenedores a modo de diccionario, es decir que si ambos son identicos solo que uno de los contenedores tiene menos elementos, este est� primero. Tambien se usan \c copy y \c back_inserter que juntos a�aden el contenido del primer contenedor al final del segundo. \c random_shuffle desordena de manera aleatoria todo el contenido de un contenedor.

 Utiliza la b�squeda de profundidad limitada
 */

/** \example reinas.cpp
 Usa functores (objetos-funci�n) para especificar las operaciones sobre los estados. Tambi�n demuestra como utilizar el par�metro opcional \c registrar_solucion para encontrar todas las soluciones al problema.
 
 De la STL usa \c vector y \c pair (pair es una estructura simple con dos miembros de, el primero se llama \c first y el segundo \c second)
 
 Utiliza la b�squeda por profundidad limitada
*/

/** \example laberinto.cpp
 Usa punteros a funciones para encontrar la solucion a un laberinto hard-coded en el programa, demuestra el uso de la b�squeda bidireccional especificando los operadores inversos y dejando que el auxiliar \c OperadoresInversos los genere, tambien demuestra a \c simplificar_bidireccional para convertir una soluci�n de dos caminos a una de un solo sentido.
 
 De la STL usa \c vector y \c pair. Tambien la funci�n auxiliar \c make_pair que permite construir un pair sin utilizar variables auxiliares.
 
 Utiliza b�squeda bidireccional
*/
/** \example laberinto2.cpp
 Usa punteros a funciones para encontrar la solucion a un laberinto ingresado por entrada estandar, poniendo
 la solucion en salida estandar.  Demuestra el uso del contenedor \c SecuenciaEstados para convertir las operaciones devueltas como solucion en los estados intermedios
 
 Una buena manera de usarlo es poner el laberinto en un archivo de texto. Tal archivo debe usar el caracter # para las paredes, la entrada del laberinto es el �nico espacio de la primera fila y la salida el �nico espacio en en la �ltima fila. La primera y �ltima columna consisten de solo paredes (#)
 
 Si el laberinto esta en el archivo laberinto.txt y se quiere la solucion en el arcivo solucion.txt
 en la linea de comandos del sistema ponga:
 \code
 c:\...> type laberinto.txt | laberinto2.exe > solucion.txt
 \endcode
 El archivo de solucion consiste en el mismo laberinto pero con el caracter + marcando el camino.
*/

/** \example viajero.cpp
 Demuestra el uso de \c primero_mejor y un functor (\c Elige_mejor_ciudad) para hallar el camino entre dos ciudades. Las operaciones son functores (objetos-funci�n) instancias de la clase Viajar.
 
 Ademas de la b�squeda de primero el mejor, usa las b�squedas por profundidad limitada y preferencia amplitud.
*/