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


int main(){

}