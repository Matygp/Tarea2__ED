#ifndef GENERAL_TREE_H
#define GENERAL_TREE_H

#include <string>
#include <vector>
#include <iostream>


//Estructura para almacenar la información de los libros similares.

struct SimilarBook {
    std::string title;        // Título del libro similar
    std::string isbn;         // ISBN del libro similar
    int publicationYear;      // Año de publicación del libro similar
};

//Estructura principal con los datos requeridos del dataset
 
struct BookData {
    std::string id;                           // ID único del libro
    std::string title;                        // Título principal
    std::string isbn;                         // Código ISBN
    int publicationYear;                      // Año de publicación original
    std::string language;                     // Idioma del libro
    std::string description;                  // Descripción/Sinopsis
    double averageRating;                     // Rating promedio obtenido
    int numPages;                             // Número total de páginas
    std::vector<SimilarBook> similarBooks;    // Colección de libros similares cargados
};

//Clase que representa un nodo individual dentro del árbol general.
 
class TreeNode {
public:
    BookData data;                    // Información del libro en el nodo actual
    std::vector<TreeNode*> children;  // Lista dinámica de punteros a nodos hijos

    // Constructor estructurado
    TreeNode(BookData data) : data(data) {}
    
    // Destructor recursivo para garantizar la liberación segura de memoria dinámica
    ~TreeNode() {
        for (TreeNode* child : children) {
            delete child;
        }
    }
};

//Clase de control del Árbol General

class GeneralTree {
private:
    TreeNode* root; // Puntero hacia el nodo raíz

    //Implementación del recorrido Preorden.
    void preOrderList(TreeNode* node) {
        if (node == nullptr) return;

        //Procesar la raíz del subárbol actual e imprime el ID del libro
        std::cout << node->data.id << std::endl;

        //Visita recursivamente cada uno de sus hijos de izquierda a derecha
        for (TreeNode* child : node->children) {
            preOrderList(child);
        }
    }

    //Función para eliminar y liberar memoria de los nodos del árbol
    TreeNode* pruneRatings(TreeNode* node, double r) {
        if (node == nullptr) return nullptr;
        
        //Primero, filtrar los hijos de abajo hacia arriba (postorden)
        std::vector<TreeNode*> validChildren;
        for (TreeNode* child : node->children) {
            TreeNode* processedChild = pruneRatings(child, r);
            if (processedChild != nullptr) {
                validChildren.push_back(processedChild);
            }
        }
        node->children = validChildren;

        //Luego, evaluar el nodo actual tras limpiar su descendencia
        if (node->data.averageRating <= r) {
            delete node; //Llama al destructor del nodo (libera memoria)
            return nullptr;
        }
        return node;
    }

public:
   
    // Constructor por defecto
    GeneralTree() : root(nullptr) {}
    
    // Destructor principal
    ~GeneralTree() { 
        delete root; // Desencadena de forma segura la destrucción total de los nodos
    }

    //Lista los IDs de los libros siguiendo un recorrido preorden.

    void listar() {
        if (this->root != nullptr) {
            preOrderList(this->root);
        }
    }

    //Llama a función privada pruneRatings pasándole la raíz del árbol
    void borrar_ratings(double r) {
        this->root = pruneRatings(this->root, r);
    }
};

#endif 