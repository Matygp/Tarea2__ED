# Entregable 2: XML

**Autores:** Matías García, Vicente Miranda  

## Acerca del Proyecto
Este proyecto implementa una estructura de datos jerárquica no lineal (**Árbol General / k-ario**) en C++ para procesar, gestionar y analizar un dataset masivo de libros extraído de la API de Goodreads en formato XML.
El sistema es capaz de construir dinámicamente el árbol en memoria principal, donde cada nodo almacena la información detallada de un libro y sus recomendaciones (libros similares), permitiendo ejecutar algoritmos de filtrado y búsqueda basados en recorridos específicos.

---

## Características y Funcionalidades

1. **Parser XML Integrado:** Lectura robusta de archivos `.xml` mediante la librería `tinyxml2`, aislando y extrayendo de forma segura etiquetas y bloques.
2. **Identificación de Libros Precursores:** Algoritmo de búsqueda que recorre el árbol para identificar libros que fueron publicados estrictamente antes que todas sus recomendaciones.
3. **Poda por Calificación (Rating):** Sistema de limpieza profunda que elimina del catálogo todos los libros con un rating igual o inferior a un umbral definido por el usuario.

## Decisiones Arquitectónicas
* **Encapsulamiento:** La clase `GeneralTree` oculta sus punteros internos y motores recursivos. Las validaciones de memoria y limpieza se administran desde métodos públicos (wrappers) por seguridad.
* **Recorrido Post-Orden para la Poda:** La función `pruneRatings` evalúa los nodos desde las hojas hacia la raíz. Esto garantiza que no existan fugas de memoria (*memory leaks*) al desconectar una rama, asegurando que los punteros hijos se liberen antes de destruir al padre.
* **Recorrido Pre-Orden para Búsqueda:** La función `findPrecursors` evalúa a cada padre antes de saltar a los hijos, optimizando la lectura top-down.
  
## Estructura del Repositorio
* `main.cpp`: Motor de ejecución y lógica de parseo XML.
* `GeneralTree.h`: Definición e implementación de la clase del árbol k-ario y las estructuras de datos (`BookData`, `SimilarBook`).
* `tinyxml2.h` / `tinyxml2.cpp`: Librería externa y ligera utilizada para el procesamiento del DOM del XML.

---

## Compilación y ejecución
#### compilar:

Para variable rutaCarpeta que encontrara en el main asegurarse de colocar la ruta a la carpeta de books_xml que esta en su sistema, por ejemplo en nuestro caso: 

"C:\\Users\\matia\\OneDrive\\Desktop\\Tarea2__ED\\books_xml\\books_xml"

```bash
g++ main.cpp tinyxml2.cpp -o tarea2_ed
```
#### ejecutar:
```bash
./tarea2_ed

