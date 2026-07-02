# Análise de Heurísticas de Coloração Inteira aplicadas a uma Heurística de Coloração Fracionária em Grafos
## Autor: Venicius Feitosa da Silva
Este repositório contém o código-fonte desenvolvido para o Trabalho de Conclusão de Curso em Sistemas de Informação (UFC - Quixadá). O sistema implementa, analisa e compara a heurística original **Color Group** com algoritmos clássicos de coloração de vértices e coloração fracionária.

O projeto foi totalmente desenvolvido em C++ e oferece uma interface via linha de comando (CLI) para execução em lote de instâncias DIMACS, validação de soluções e exportação de resultados analíticos.

## Funcionalidades Implementadas

O sistema permite:
* Leitura de grafos padronizados no formato DIMACS (arquivos `.col`).
* Comparação direta entre heurísticas clássicas isoladas e combinadas com o método **Color Group**.
* Cálculo do limite fracionário e número de iterações para Coloração Fracionária.
* Processamento em lote de todas as instâncias de um diretório.
* Exportação automatizada de métricas para arquivos `.csv` (`result_[pasta]_.csv`).
* Validação automática de segurança (`checarValidadedaColoracao` e `checarValidadedaColoracaoFracionaria`) para garantir que nenhum vértice adjacente compartilhe a mesma cor indevidamente.

## Heurísticas de Coloração

A classe `Coloracao` (`coloracao.hpp`) contém as seguintes abordagens:

1. **Simples (Gulosa)**
2. **Largest First (LF)**: Ordenação pelo maior grau induzido.
3. **Smallest Last (SL)**: Remoção iterativa dos vértices de menor grau.
4. **DSATUR**: Prioridade baseada no grau de saturação.
5. **Color Group (CG)**: Heurística proposta que agrupa vértices por cores pré-atribuídas e reordena os grupos para otimizar a coloração final.
6. **Coloração Fracionária**: Avaliação de limites utilizando conjuntos independentes maximais gerados a partir das ordenações acima.

## Estrutura do Código

* `main.cpp`: Ponto de entrada que inicializa a interface interativa.
* `menu.hpp`: Controlador do fluxo de execução, exibição no terminal e geração de CSV.
* `grafo.hpp`: Representação do grafo via Matriz de Adjacência, lógica de leitura de arquivos e funções de validação de corretude.
* `vertice.hpp`: Entidade vértice armazenando ID, cor atual, grau, grau de saturação e um conjunto (`set`) de cores para soluções fracionárias.
* `coloracao.hpp`: Motor algorítmico contendo todas as implementações das heurísticas inteiras e fracionárias.

## Como Executar

### Pré-requisitos
* Compilador C++ (ex: `g++`).
* Uma pasta chamada `instancias/` no mesmo diretório do executável, contendo os arquivos `.col` (ex: `myciel3.col`) para testes locais.

### Compilação
Abra o terminal na raiz do projeto e compile os arquivos utilizando:

```bash
g++ -Wall -Wextra -pedantic-errors *.cpp -o main