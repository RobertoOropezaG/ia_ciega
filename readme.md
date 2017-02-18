# IAciega
Esta es la librearia IA ciega
version 0.9 de Roberto Oropeza Gamarra

La documentacion completa esta en html/index.html y refman.pdf

## 1. Documentación de IA Ciega
El objetivo de esta libreria es facilitar el uso de los Algoritmos de Búsqueda usados en Inteligencia Artificial de manera que una vez que se tengan definidos los estados y las operaciones sea sencillo probar los distintos algoritmos sin tener que implementarlos.

### 1.1. Introducción
La librería esta diseñada para permitir un uso sencillo sin sacrificar la flexibilidad. Se han usado plantillas, pero para llamar a las funciones no es necesario especificar los tipos de las plantillas, excepto en primero_mejor.

### 1.2. Modo de uso
Para traer todas las funciones al scope de trabajo, evitando poner `ia::...` cada vez, puedes poner:

    using namespace ia;
al principio de tu programa, después de incluir las cabeceras que necesites.

 Recuerda ver los ejemplos para ver el uso práctico y ante cualquier duda, el más sencillo es pastor.cpp, los más complejos son 8puzzle.cpp y reinas.cpp.
