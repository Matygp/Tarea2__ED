#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include "GeneralTree.h"
#include "tinyxml2.h" 

namespace fs = std::filesystem;

//Función encargada de parsear el archivo XML individual usando TinyXML2.
BookData procesarArchivoXML(const std::string& rutaArchivo) {
    BookData libro;
    tinyxml2::XMLDocument doc;

    // Intentar abrir el archivo mediante TinyXML2
    if (doc.LoadFile(rutaArchivo.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error al leer el archivo XML: " << rutaArchivo << std::endl;
        return libro; 
    }

    // Buscamos el elemento <book> manejando posibles wrappers como <GoodreadsResponse>
    tinyxml2::XMLElement* rootElement = doc.FirstChildElement("book");
    if (!rootElement) {
        tinyxml2::XMLElement* wrapper = doc.FirstChildElement();
        if (wrapper) {
            rootElement = wrapper->FirstChildElement("book");
        }
    }

    // Si el archivo no contiene la estructura esperada, salimos de forma segura
    if (!rootElement) return libro;

    //Aqui extraemos los atributos del libro principal
    if (rootElement->FirstChildElement("id")) {
        libro.id = rootElement->FirstChildElement("id")->GetText();
    }
    
    if (rootElement->FirstChildElement("title")) {
        const char* titleText = rootElement->FirstChildElement("title")->GetText();
        libro.title = titleText ? titleText : "";
    }
    
    if (rootElement->FirstChildElement("isbn")) {
        const char* isbnText = rootElement->FirstChildElement("isbn")->GetText();
        libro.isbn = isbnText ? isbnText : "";
    }
    
    if (rootElement->FirstChildElement("publication_year")) {
        rootElement->FirstChildElement("publication_year")->QueryIntText(&libro.publicationYear);
    } else {
        libro.publicationYear = 0;
    }
    
    if (rootElement->FirstChildElement("language_code")) {
        const char* langText = rootElement->FirstChildElement("language_code")->GetText();
        libro.language = langText ? langText : "";
    }
    
    if (rootElement->FirstChildElement("description")) {
        const char* descText = rootElement->FirstChildElement("description")->GetText();
        libro.description = descText ? descText : "";
    }
    
    if (rootElement->FirstChildElement("average_rating")) {
        rootElement->FirstChildElement("average_rating")->QueryDoubleText(&libro.averageRating);
    } else {
        libro.averageRating = 0.0;
    }
    
    if (rootElement->FirstChildElement("num_pages")) {
        rootElement->FirstChildElement("num_pages")->QueryIntText(&libro.numPages);
    } else {
        libro.numPages = 0;
    }

    //Generación de sub arbol para libros similares 
    tinyxml2::XMLElement* similarRoot = rootElement->FirstChildElement("similar_books");
    if (similarRoot) {
        for (tinyxml2::XMLElement* simElem = similarRoot->FirstChildElement("book"); 
             simElem != nullptr; 
             simElem = simElem->NextSiblingElement("book")) {
             
             SimilarBook simLibro;
             
             if (simElem->FirstChildElement("title")) {
                 const char* simTitle = simElem->FirstChildElement("title")->GetText();
                 simLibro.title = simTitle ? simTitle : "";
             }
             if (simElem->FirstChildElement("isbn")) {
                 const char* simIsbn = simElem->FirstChildElement("isbn")->GetText();
                 simLibro.isbn = simIsbn ? simIsbn : "";
             }
             if (simElem->FirstChildElement("publication_year")) {
                 simElem->FirstChildElement("publication_year")->QueryIntText(&simLibro.publicationYear);
             } else {
                 simLibro.publicationYear = 0;
             }
             
             libro.similarBooks.push_back(simLibro); // Almacenamiento en el vector del nodo
        }
    }

    return libro;
}

int main() {
    GeneralTree miArbol;
    
    //Ruta absoluta del sistema de archivos en Windows (Asegurarse de colocar la ruta que esta en su sistema, no es igual para todos)
    std::string rutaCarpeta = "C:\\Users\\matia\\OneDrive\\Desktop\\Tarea2__ED\\books_xml\\books_xml"; 

    std::vector<TreeNode*> todosLosNodos;

    // Escaneamos los archivos
    if (fs::exists(rutaCarpeta) && fs::is_directory(rutaCarpeta)) {
        for (const auto& entry : fs::directory_iterator(rutaCarpeta)) {
            std::string ext = entry.path().extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

            if (ext == ".xml") {
                BookData data = procesarArchivoXML(entry.path().string());
                
                if (!data.id.empty()) {
                    TreeNode* nuevoNodo = new TreeNode(data);
                    todosLosNodos.push_back(nuevoNodo);
                }
            }
        }
    } else {
        std::cerr << "No se pudo acceder a la ruta especificada, revise las carpetas." << std::endl;
        return 1;
    }

    // Construimos el arbol general
    if (!todosLosNodos.empty()) {
        TreeNode* raiz = todosLosNodos[0]; // El primer libro de la lista actúa como raíz
        
        // Enlazamos jerárquicamente el resto de los 9,999 libros como sus hijos directos
        for (size_t i = 1; i < todosLosNodos.size(); ++i) {
            raiz->children.push_back(todosLosNodos[i]);
        }
        
        miArbol.setRoot(raiz);
        std::cout << "Arbol construido correctamente, se cargaron " << todosLosNodos.size() << " libros." << std::endl;
    } else {
        std::cout << "No se encontraron archivos XML procesables." << std::endl;
        return 0;

    }

    // Llamamos a la función listar()
    std::cout << "Listado completo de IDs:" << std::endl;
    miArbol.listar(); // Imprime la lista que ya viste en pantalla
    std::cout << "Presione la tecla ENTER para proceder con la poda por rating" << std::endl;
    std::cin.get(); // Se utiliza para que el sistema espere a apretar ENTER para asi continuar con las demas funciones
    //Esto se utilizo dado que si se ejecutaba sin esto al ser un gran volumen de datos se llenaba el buffer de mi computadora 
    //y se terminaba la ejecución sin realizar las demas funciones

    // Llamamos a la función borrar_ratings(r)
    double ratingCorte = 1.6; // Le damos el rating para eliminar los libros
    std::cout << "Eliminando recursivamente nodos con calificacion menor o igual a " << ratingCorte <<std::endl;
    miArbol.borrar_ratings(ratingCorte); // Poda y libera la memoria dinámica de los descartados
    std::cout << "\n Presione la tecla ENTER para ver el listado final de verificacion" << std::endl;
    std::cin.get();

    // Verificamos que se halla realizado bien  la poda
    std::cout << "Lista despues de la poda:"<<std::endl;
    miArbol.listar(); // Muestra el árbol limpio, verificando el éxito del borrado
    std::cout << "\n Presione la tecla ENTER para continuar con precursores" << std::endl;
    std::cin.get();

    //Llamamos a la función precursores()
    std::cout << "IDs de libros cuyos libros similares son todos de anos posteriores:" << std::endl;
    miArbol.precursores(); // Procesa y filtra las colecciones de libros similares

    return 0;
}