#ifndef COLOCAO_HPP
#define COLOCAO_HPP

#include <vector>
#include <algorithm>
#include <stack>
#include <set>

#include "grafo.hpp"
#include "vertice.hpp"

using namespace std;

// enum para identificar a heurística de coloração fracionária a ser usada
enum ICH
{
    SIMPLES,
    SIMPLES_CG,
    LARGEST_FIRST,
    LARGEST_FIRST_CG,
    SMALLEST_LAST,
    SMALLEST_LAST_CG,
    DSATUR,
    DSATUR_CG
};
/**
 * @brief Estrutura para armazenar o resultado da coloração
 * 
 * @param numCores Número de cores usadas na coloração
 * @param numIteracoes Número de iterações realizadas na coloração fracionária
 * @param limiteFracionario Limite fracionário obtido na coloração fracionária
 */
struct ColoracaoResult
{
    int numCores;
    int numIteracoes;
    double limiteFracionario;
};

/**
 * @brief Classe para realizar colorações em grafos
 * 
 */
class Coloracao
{
public:
    /**
     * @brief Verifica se uma cor está disponível para um vértice, dado seus vizinhos
     * 
     * @param cor Cor a ser verificada
     * @param vizinhos Vizinhos do vértice
     * @return true Se a cor estiver disponível
     * @return false Se a cor não estiver disponível
     */
    bool corDisponivel(int cor, const vector<Vertice *> &vizinhos)
    {
        for (auto vizinho : vizinhos)
        {
            if (vizinho->getCor() == cor)
            {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Realiza a coloração simples de um grafo, seguindo uma ordem de vértices
     * 
     * @param G Grafo a ser colorido
     * @param ordem Ordem dos vértices para coloração
     * @return int Número de cores usadas na coloração
     */
    int coloracaoSimples(Grafo &G, const vector<int> &ordem)
    {
        int numCores = 0;
        auto &adj = G.getAdjMatrix();

        for (int id : ordem)
        {
            Vertice *v = G.getVerticeById(id);

            vector<Vertice *> vizinhos;
            for (int j = 0; j < G.getNumVertices(); j++)
            {
                if (adj[id][j] == 1)
                    vizinhos.push_back(G.getVerticeById(j));
            }

            int cor = 1;
            while (!corDisponivel(cor, vizinhos))
                cor++;

            v->setCor(cor);
            numCores = max(numCores, cor);
        }

        return numCores;
    }

    /**
     * @brief Realiza a coloração com a heurística do maior grau primeiro
     * 
     * @param G Grafo a ser colorido
     * @param subgrafo Subgrafo a ser colorido
     * @return int Número de cores usadas na coloração
     */
    int coloracaoLargestFirst(Grafo &G, vector<int> subgrafo = {})
    {
        int n = G.getNumVertices();
        auto &adj = G.getAdjMatrix();

        if (subgrafo.empty())
        {
            for (int i = 0; i < n; i++)
                subgrafo.push_back(i);
        }

        vector<int> grauInduzido(n, 0);

        for (int id : subgrafo)
        {
            int g = 0;
            for (int viz : subgrafo)
            {
                if (adj[id][viz] == 1)
                    g++;
            }
            grauInduzido[id] = g;
        }

        sort(subgrafo.begin(), subgrafo.end(), [&](int a, int b)
             { 
                 if (grauInduzido[a] == grauInduzido[b]) 
                 {
                     return a < b; 
                 }
                 return grauInduzido[a] > grauInduzido[b]; });

        return coloracaoSimples(G, subgrafo);
    }

    /**
     * @brief Realiza a coloração com a heurística do menor grau último
     * 
     * @param G Grafo a ser colorido
     * @param subgrafo Subgrafo a ser colorido
     * @return int Número de cores usadas na coloração
     */
    int coloracaoSmallestLast(Grafo &G, vector<int> subgrafo = {})
    {
        int n = G.getNumVertices();
        auto &adj = G.getAdjMatrix();

        if (subgrafo.empty())
        {
            for (int i = 0; i < n; i++)
                subgrafo.push_back(i);
        }

        vector<int> grau(n, 0);
        vector<bool> ativo(n, false);
        vector<int> ordem;

        for (int id : subgrafo)
        {
            ativo[id] = true;
        }

        for (int id : subgrafo)
        {
            int g = 0;
            for (int viz : subgrafo)
            {
                if (adj[id][viz] == 1)
                    g++;
            }
            grau[id] = g;
        }

        int tamanhoSubgrafo = subgrafo.size();
        for (int k = 0; k < tamanhoSubgrafo; k++)
        {
            int menor = -1;

            for (int id : subgrafo)
            {
                if (ativo[id] && (menor == -1 || grau[id] < grau[menor]))
                {
                    menor = id;
                }
            }

            ativo[menor] = false;
            ordem.push_back(menor);

            for (int viz : subgrafo)
            {
                if (adj[menor][viz] == 1 && ativo[viz])
                {
                    grau[viz]--;
                }
            }
        }

        reverse(ordem.begin(), ordem.end());

        return coloracaoSimples(G, ordem);
    }

    /**
     * @brief Realiza a coloração com a heurística do DSATUR
     * 
     * @param G Grafo a ser colorido
     * @param subgrafo Subgrafo a ser colorido
     * @return int Número de cores usadas na coloração
     */
    int coloracaoDSATUR(Grafo &G, vector<int> subgrafo = {})
    {
        int n = G.getNumVertices();
        auto &adj = G.getAdjMatrix();

        if (subgrafo.empty())
        {
            for (int i = 0; i < n; i++)
                subgrafo.push_back(i);
        }

        vector<int> grauInduzido(n, 0);
        for (int id : subgrafo)
        {
            int g = 0;
            for (int viz : subgrafo)
            {
                if (adj[id][viz] == 1)
                    g++;
            }
            grauInduzido[id] = g;
        }

        vector<int> cor(n, -1);
        vector<int> saturacao(n, 0);
        vector<set<int>> coresViz(n);

        int numCores = 0;

        for (size_t iter = 0; iter < subgrafo.size(); iter++)
        {
            int escolhido = -1;

            for (int i : subgrafo)
            {
                if (cor[i] != -1)
                    continue;

                bool atualizar = false;

                if (escolhido == -1)
                {
                    atualizar = true;
                }
                else if (saturacao[i] > saturacao[escolhido])
                {
                    atualizar = true;
                }
                else if (saturacao[i] == saturacao[escolhido])
                {
                    if (grauInduzido[i] > grauInduzido[escolhido])
                    {
                        atualizar = true;
                    }
                    else if (grauInduzido[i] == grauInduzido[escolhido])
                    {
                        if (i < escolhido)
                        {
                            atualizar = true;
                        }
                    }
                }

                if (atualizar)
                {
                    escolhido = i;
                }
            }

            int c = 1;
            while (coresViz[escolhido].count(c))
                c++;

            cor[escolhido] = c;
            G.getVerticeById(escolhido)->setCor(c);
            numCores = max(numCores, c);

            for (int j : subgrafo)
            {
                if (adj[escolhido][j] == 1 && cor[j] == -1)
                {
                    if (coresViz[j].insert(c).second)
                        saturacao[j]++;
                }
            }
        }

        return numCores;
    }

    /**
     * @brief Realiza a coloração por grupos de cores
     * 
     * @param G Grafo a ser colorido
     * @param subgrafo Subgrafo a ser colorido
     * @return int Número de cores usadas na coloração
     */
    int coloracaoColorGroup(Grafo &G, vector<int> subgrafo = {})
    {
        int n = G.getNumVertices();

        if (subgrafo.empty())
        {
            for (int i = 0; i < n; i++)
                subgrafo.push_back(i);
        }

        int maxCor = 0;
        for (int id : subgrafo)
            maxCor = max(maxCor, G.getVerticeById(id)->getCor());

        vector<vector<int>> grupos(maxCor + 1);

        for (int id : subgrafo)
        {
            Vertice *v = G.getVerticeById(id);
            if (v->getCor() != -1)
                grupos[v->getCor()].push_back(id);
        }

        grupos.erase(
            remove_if(grupos.begin(), grupos.end(),
                      [](const vector<int> &g)
                      { return g.empty(); }),
            grupos.end());

        sort(grupos.begin(), grupos.end(),
             [](const vector<int> &a, const vector<int> &b)
             { return a.size() < b.size(); });

        vector<int> ordem;
        ordem.reserve(subgrafo.size());

        for (auto &grupo : grupos)
            for (int id : grupo)
                ordem.push_back(id);

        for (auto v : G.getVertices())
            v->setCor(-1);

        return coloracaoSimples(G, ordem);
    }

    /**
     * @brief Realiza a coloração fracionária de um grafo
     * 
     * @param G Grafo a ser colorido
     * @param ich Tipo de heurística a ser utilizada
     * @return ColoracaoResult Resultado da coloração fracionária
     */
    ColoracaoResult coloracaoFracionaria(Grafo &G, ICH ich)
    {
        int n = G.getNumVertices();
        auto &adj = G.getAdjMatrix();

        vector<vector<int>> C;

        int k = 1;
        double limiteAtual = numeric_limits<double>::infinity();
        bool continuar = true;

        while (continuar)
        {
            vector<vector<int>> C_temp = C;
            vector<bool> emU(n, true);

            for (int id = 0; id < n; id++)
            {
                for (auto &S : C_temp)
                {
                    bool conflito = false;
                    for (int idS : S)
                    {
                        if (id == idS || adj[id][idS] == 1)
                        {
                            conflito = true;
                            break;
                        }
                    }

                    if (!conflito)
                    {
                        S.push_back(id);
                        emU[id] = false;
                        break;
                    }
                }
            }

            vector<int> ordemU;
            for (int id = 0; id < n; id++)
                if (emU[id])
                    ordemU.push_back(id);

            for (auto v : G.getVertices())
            {
                v->setCor(-1);
            }

            vector<vector<int>> Ck;
            if (!ordemU.empty())
            {
                switch (ich)
                {
                case SIMPLES:
                    coloracaoSimples(G, ordemU);
                    break;
                case SIMPLES_CG:
                    coloracaoSimples(G, ordemU);
                    coloracaoColorGroup(G, ordemU);
                    break;
                case LARGEST_FIRST:
                    coloracaoLargestFirst(G, ordemU);
                    break;
                case LARGEST_FIRST_CG:
                    coloracaoLargestFirst(G, ordemU);
                    coloracaoColorGroup(G, ordemU);
                    break;
                case SMALLEST_LAST:
                    coloracaoSmallestLast(G, ordemU);
                    break;
                case SMALLEST_LAST_CG:
                    coloracaoSmallestLast(G, ordemU);
                    coloracaoColorGroup(G, ordemU);
                    break;
                case DSATUR:
                    coloracaoDSATUR(G, ordemU);
                    break;
                case DSATUR_CG:
                    coloracaoDSATUR(G, ordemU);
                    coloracaoColorGroup(G, ordemU);
                    break;
                }

                int maxCorU = 0;
                for (int id : ordemU)
                    maxCorU = max(maxCorU, G.getVerticeById(id)->getCor());
                Ck.resize(maxCorU);

                for (int id : ordemU)
                {
                    int cor = G.getVerticeById(id)->getCor();

                    if (cor >= 1)
                    {
                        Ck[cor - 1].push_back(id);
                    }
                }
            }

            double novoLimite = (double)(C_temp.size() + Ck.size()) / k;

            if (novoLimite < limiteAtual)
            {
                limiteAtual = novoLimite;
                C = C_temp;

                for (auto &s : Ck)
                    C.push_back(s);
                k++;
            }
            else
            {
                continuar = false;
            }
        }

        for (auto v : G.getVertices())
        {
            v->limparCores();
        }

        for (size_t c = 0; c < C.size(); c++)
        {
            for (int id : C[c])
            {
                G.getVerticeById(id)->pushCor(c + 1);
            }
        }
        ColoracaoResult resultado;
        resultado.numCores = C.size();
        resultado.numIteracoes = k - 1;
        resultado.limiteFracionario = limiteAtual;
        return resultado;
    }
};

#endif