#include "CargaCSV.h"
struct TrieNode {
    unordered_map<char, TrieNode*> children;
    vector<Pelicula*> peliculas;

    TrieNode() {}
};

class Trie {
private:
    TrieNode* root;

    void insert(TrieNode* node, const string& key,Pelicula& pelicula, size_t depth) {
        if (depth - 1 == key.size()) {
            node->peliculas.push_back(&pelicula);
            return;
        }

        char index = key[depth];
        if (index == ' ') {
            depth += 1;
            index = key[depth];
        }
        if (!node->children[index]) {
            node->children[index] = new TrieNode();
        }

        insert(node->children[index], key, pelicula, depth + 1);
    }

    void search(TrieNode* node, const string& key, size_t depth, vector<Pelicula*>& resultados) const {
        if (!node) return;

        // Si la profundidad es igual al tamaño de la clave, buscar todas las películas en los subárboles
        if (depth - 1 == key.size()) {
            collectAll(node, resultados);
            return;
        }

        char index = key[depth];
        if (index == ' ') {
            depth += 1;
            index = key[depth];
        }
        if (node->children.find(index) != node->children.end()) {
            search(node->children[index], key, depth + 1, resultados);
        }
        // Continuar buscando en todos los subárboles
        for (auto& child : node->children) {
            if (child.first != index) {
                search(child.second, key, depth, resultados);
            }
        }
    }

    void collectAll(TrieNode* node, vector<Pelicula*>& resultados) const {
        if (!node->peliculas.empty()) {
            resultados.insert(resultados.end(), node->peliculas.begin(), node->peliculas.end());
        }
        for (auto& child : node->children) {
            collectAll(child.second, resultados);
        }
    }

public:
    Trie() : root(new TrieNode()) {}

    void insert(const string& key,Pelicula& pelicula) {
        insert(root, key, pelicula, 0);
    }

    vector<Pelicula*> search(const string& key) const {
        vector<Pelicula*> resultados;
        search(root, key, 0, resultados);
        return resultados;
    }
};