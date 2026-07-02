#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <vector>
#include <iostream>
#include <fstream>

#include "vertice.hpp"
using namespace std;

class Grafo
{
private:
    int numVertices;
    int numArestas;
    string nome;
    vector<vector<int>> adjMatrix;
    vector<Vertice *> vertices;

public:
    Grafo(string nome, int nVertices, int nArestas = 0)
    {
        this->nome = nome;
        this->numVertices = nVertices;
        this->numArestas = nArestas;
        adjMatrix.resize(nVertices, vector<int>(nVertices, 0));

        for (int i = 0; i < nVertices; i++)
        {
            this->vertices.push_back(new Vertice(i));
        }
    }

    Grafo(Grafo *grafo)
    {
        this->nome = grafo->getNome() + "_copia";
        this->numVertices = grafo->getNumVertices();
        this->numArestas = grafo->getNumArestas();
        this->adjMatrix = grafo->getAdjMatrix();

        for (int i = 0; i < numVertices; i++)
        {
            this->vertices.push_back(new Vertice(grafo->getVertices()[i]));
        }
    }

    ~Grafo()
    {
        for (auto v : vertices)
            delete v;
    }

    void adicionarAresta(int u, int v)
    {
        if (u >= 0 && u < numVertices && v >= 0 && v < numVertices)
        {
            adjMatrix[u][v] = 1;
            adjMatrix[v][u] = 1;
            vertices[u]->incrementarGrau();
            vertices[v]->incrementarGrau();
        }
    }

    void removerAresta(int u, int v)
    {
        if (u >= 0 && u < numVertices && v >= 0 && v < numVertices)
        {
            adjMatrix[u][v] = 0;
            adjMatrix[v][u] = 0;
            vertices[u]->decrementarGrau();
            vertices[v]->decrementarGrau();
        }
    }

    void removerArestasDoVertice(int vertice)
    {
        if (vertice >= 0 && vertice < numVertices)
        {
            for (int v = 0; v < numVertices; v++)
            {
                if (adjMatrix[vertice][v] == 1)
                {
                    removerAresta(vertice, v);
                }
            }
        }
    }

    void imprimir()
    {
        for (int i = 0; i < numVertices; i++)
        {
            for (int j = 0; j < numVertices; j++)
            {
                cout << adjMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    static Grafo *lerGrafoDeArquivo(string nomeArquivo)
    {
        ifstream arquivo(nomeArquivo);
        if (!arquivo.is_open())
        {
            cout << "Erro: Não foi possível abrir o arquivo." << endl;
            return nullptr;
        }

        Grafo *g = nullptr;
        char tipoLinha;

        while (arquivo >> tipoLinha)
        {
            if (tipoLinha == 'c')
            {
                string linhaIgnorada;
                getline(arquivo, linhaIgnorada);
            }
            else if (tipoLinha == 'p')
            {
                string formato;
                int numVertices, numArestas;
                arquivo >> formato >> numVertices >> numArestas;

                g = new Grafo(nomeArquivo, numVertices, numArestas);
            }
            else if (tipoLinha == 'e')
            {
                int u, v;
                arquivo >> u >> v;

                if (g != nullptr)
                {
                    g->adicionarAresta(u - 1, v - 1);
                }
            }
        }

        arquivo.close();
        return g;
    }

    int getNumVertices()
    {
        return this->numVertices;
    }

    int getNumArestas()
    {
        return this->numArestas;
    }

    vector<vector<int>> &getAdjMatrix()
    {
        return this->adjMatrix;
    }

    vector<Vertice *> &getVertices()
    {
        return this->vertices;
    }

    Vertice *getVerticeById(int id)
    {
        return vertices[id];
    }

    void setVertices(vector<Vertice *> vertices)
    {
        this->vertices = vertices;
    }

    string getNome()
    {
        return this->nome;
    }

    void printarGrafo()
    {
        cout << "Grafo: " << this->nome << endl;
        cout << "Numero de Vertices: " << this->numVertices << endl;
        cout << "Numero de Arestas: " << this->numArestas << endl;
        for (const auto &vertice : vertices)
        {
            cout << "Vertice " << vertice->getId() << ": Cores = ";
            for (const auto &cor : vertice->getCores())
            {
                cout << cor << " ";
            }
            cout << ", Grau = " << vertice->getGrau() << endl;
        }
    }

    bool checarValidadedaColoracao()
    {
        auto &adj = this->getAdjMatrix();
        auto &vertices = this->getVertices();

        for (size_t i = 0; i < vertices.size(); i++)
        {
            for (size_t j = i + 1; j < vertices.size(); j++)
            {
                if (adj[i][j] == 1 &&
                    vertices[i]->getCor() != -1 &&
                    vertices[i]->getCor() == vertices[j]->getCor())
                {
                    cout << "Coloracao invalida: Vertices "
                         << i << " e " << j
                         << " tem a mesma cor.\n";
                    return false;
                }
            }
        }

        return true;
    }

    bool checarValidadedaColoracaoFracionaria()
    {
        auto &adj = this->getAdjMatrix();
        auto &vertices = this->getVertices();

        for (size_t i = 0; i < vertices.size(); i++)
        {
            for (size_t j = i + 1; j < vertices.size(); j++)
            {
                if (adj[i][j] == 1)
                {
                    set<int> coresI = vertices[i]->getCores();
                    set<int> coresJ = vertices[j]->getCores();

                    for (int cor : coresI)
                    {
                        if (coresJ.count(cor))
                        {
                            cout << "Coloracao fracionaria invalida: Vertices "
                                 << i << " e " << j
                                 << " compartilham a cor " << cor << ".\n";
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }
};

#endif