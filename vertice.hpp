#ifndef VERTICE_HPP
#define VERTICE_HPP

#include <vector>
#include <set>

using namespace std;
class Vertice
{
private:
    int id;
    int cor;
    set<int> cores;
    int grau;
        int grauSaturacao;
public:
    Vertice(int id)
    {
        this->id = id;
        this->cor = -1;
        this->grau = 0;
        this->grauSaturacao = 0;
    }

    Vertice(Vertice *vertice)
    {
        this->id = vertice->getId();
        this->cor = vertice->getCor();
        this->grau = vertice->getGrau();
        this->grauSaturacao = vertice->getGrauSaturacao();
    }

    int getId() const
    {
        return id;
    }

    int getCor() const
    {
        return this->cor;
    }

    void setCor(int cor)
    {
        this->cor = cor;
    }

    int getGrau() const
    {
        return this->grau;
    }

    void incrementarGrau()
    {
        this->grau++;
    }

    void decrementarGrau()
    {
        if (this->grau > 0)
        {
            this->grau--;
        }
    }

    int getGrauSaturacao() const
    {
        return this->grauSaturacao;
    }

    void incrementarGrauSaturacao()
    {
        this->grauSaturacao++;
    }

    void pushCor(int cor)
    {
        this->cores.insert(cor);
    }

    set<int> getCores() const
    {
        return this->cores;
    }

    void limparCores()
    {
        this->cores.clear();
    }
};

#endif