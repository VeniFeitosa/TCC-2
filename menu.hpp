#ifndef MENU_HPP
#define MENU_HPP
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <iomanip>

#include "grafo.hpp"
#include "coloracao.hpp"

using namespace std;
namespace fs = std::filesystem;
class Menu
{
public:
    void printLine()
    {
        cout << "==============================" << endl;
    }
    void exibirHeader()
    {
        this->printLine();
        cout << "   MENU  " << endl;
        this->printLine();
    }
    void exibirMenu()
    {
        int escolha = 0;
        string nomeArquivo;
        string nomePasta;
        Grafo *g = nullptr;

        vector<string> opcoes = {
            "Ler Grafo de um arquivo",
            "Imprimir Grafo",
            "Comparar todas as coloracoes_i do grafo atual",
            "Comparar as coloracoes_i de TODAS as instancias da pasta",
            "Comparar todas as coloracoes_f do grafo atual",
            "Comparar as coloracoes_f de TODAS as instancias da pasta",
            "Realizar testes e exportar arquivo CSV",
            "Sair"};

        do
        {
            for (size_t i = 0; i < opcoes.size(); i++)
            {
                cout << (i + 1) << ". " << opcoes[i] << endl;
            }
            this->printLine();

            cout << "Escolha uma opcao: ";
            cin >> escolha;
            this->printLine();

            switch (escolha)
            {
            case 1:
            {
                cout << "Digite o nome do arquivo (ex: myciel3.col): ";
                cin >> nomeArquivo;

                string caminhoArquivo = "instancias/" + nomeArquivo;
                cout << "Lendo grafo do arquivo: " << caminhoArquivo << endl;

                if (g != nullptr)
                {
                    delete g;
                }

                g = Grafo::lerGrafoDeArquivo(caminhoArquivo);

                if (g != nullptr)
                {
                    cout << "Grafo lido com sucesso!" << endl;
                }
                else
                {
                    cout << "Falha ao ler o grafo. Verifique se o arquivo existe na pasta 'instancias'." << endl;
                }
                break;
            }
            case 2:
            {
                if (g != nullptr)
                {
                    g->printarGrafo();
                }
                else
                {
                    cout << "Nenhum grafo carregado na memoria!" << endl;
                }
                break;
            }
            case 3:
            {
                if (g != nullptr)
                {
                    Coloracao col;

                    Grafo gCopia1(g), gCopia2(g), gCopia3(g), gCopia4(g);

                    vector<int> ordem;
                    for (auto v : gCopia1.getVertices())
                        ordem.push_back(v->getId());

                    cout << "\nExecutando comparacoes para o grafo: " << g->getNome() << "..." << endl;

                    int cSimples = col.coloracaoSimples(gCopia1, ordem);
                    int cgSimples = col.coloracaoColorGroup(gCopia1);

                    int cLF = col.coloracaoLargestFirst(gCopia2);
                    int cgLF = col.coloracaoColorGroup(gCopia2);

                    int cSL = col.coloracaoSmallestLast(gCopia3);
                    int cgSL = col.coloracaoColorGroup(gCopia3);

                    int cDSATUR = col.coloracaoDSATUR(gCopia4);
                    int cgDSATUR = col.coloracaoColorGroup(gCopia4);

                    cout << "\nRESULTADOS (Base -> Com Color-Group):" << endl;
                    cout << "Coloracao Simples     : " << cSimples << " -> " << cgSimples << " cores" << endl;
                    cout << "Largest First         : " << cLF << " -> " << cgLF << " cores" << endl;
                    cout << "Smallest Last         : " << cSL << " -> " << cgSL << " cores" << endl;
                    cout << "DSATUR                : " << cDSATUR << " -> " << cgDSATUR << " cores" << endl;
                }
                else
                {
                    cout << "Nenhum grafo carregado na memoria! Use a opcao 1 primeiro." << endl;
                }
                break;
            }
            case 4:
            {
                cout << "Digite o nome da pasta (ex: instancias): ";
                cin >> nomePasta;
                string caminhoPasta = nomePasta + "/";

                if (!fs::exists(caminhoPasta) || !fs::is_directory(caminhoPasta))
                {
                    cout << "A pasta '" << caminhoPasta << "' nao foi encontrada!" << endl;
                    break;
                }

                cout << "\nProcessando todas as instancias da pasta '" << caminhoPasta << "'...\n"
                     << endl;

                cout << left << setw(40) << "Instancia"
                     << setw(35) << "Simples(CG)"
                     << setw(35) << "LF(CG)"
                     << setw(35) << "SL(CG)"
                     << setw(35) << "DSATUR(CG)" << endl;
                cout << string(150, '-') << endl;

                for (const auto &entry : fs::directory_iterator(caminhoPasta))
                {
                    if (entry.is_regular_file())
                    {
                        string pathAtual = entry.path().string();
                        string nomeInstancia = entry.path().filename().string();

                        Grafo *grafoAtual = Grafo::lerGrafoDeArquivo(pathAtual);
                        if (grafoAtual != nullptr)
                        {
                            Coloracao col;
                            Grafo g1(grafoAtual), g2(grafoAtual), g3(grafoAtual), g4(grafoAtual);

                            vector<int> ordem;
                            for (auto v : g1.getVertices())
                                ordem.push_back(v->getId());

                            int cSimples = col.coloracaoSimples(g1, ordem);
                            bool validadeSimples = g1.checarValidadedaColoracao();
                            if (!validadeSimples)
                            {
                                cout << "Erro: Coloração inválida detectada na instância " << nomeInstancia << " para a heurística Simples!" << endl;
                                continue;
                            }

                            int cgSimples = col.coloracaoColorGroup(g1);
                            bool validadeSimplesCG = g1.checarValidadedaColoracao();
                            if (!validadeSimplesCG)
                            {
                                cout << "Erro: Coloração inválida detectada na instância " << nomeInstancia << " para a heurística Simples + Color-Group!" << endl;
                                continue;
                            }
                            // LF
                            int cLF = col.coloracaoLargestFirst(g2);
                            bool validadeLF = g2.checarValidadedaColoracao();
                            if (!validadeLF)
                            {
                                cout << "Erro: Coloração inválida detectada na instância " << nomeInstancia << " para a heurística Largest First!" << endl;
                                continue;
                            }

                            int cgLF = col.coloracaoColorGroup(g2);
                            bool validadeLF_CG = g2.checarValidadedaColoracao();
                            if (!validadeLF_CG)
                            {
                                cout << "Erro: Coloração inválida detectada na instância " << nomeInstancia << " para a heurística Largest First + Color-Group!" << endl;
                                continue;
                            }

                            // SL
                            int cSL = col.coloracaoSmallestLast(g3);
                            bool validadeSL = g3.checarValidadedaColoracao();
                            if (!validadeSL)
                            {
                                cout << "Erro: Coloração inválida detectada na instância " << nomeInstancia << " para a heurística Smallest Last!" << endl;
                                continue;
                            }
                            int cgSL = col.coloracaoColorGroup(g3);
                            bool validadeSL_CG = g3.checarValidadedaColoracao();
                            if (!validadeSL_CG)
                            {
                                cout << "Erro: Coloração inválida detectada na instância " << nomeInstancia << " para a heurística Smallest Last + Color-Group!" << endl;
                                continue;
                            }

                            // DSATUR
                            int cDSATUR = col.coloracaoDSATUR(g4);
                            bool validadeDSATUR = g4.checarValidadedaColoracao();
                            if (!validadeDSATUR)
                            {
                                cout << "Erro: Coloração inválida detectada na instância " << nomeInstancia << " para a heurística DSATUR!" << endl;
                                continue;
                            }
                            int cgDSATUR = col.coloracaoColorGroup(g4);
                            bool validadeDSATUR_CG = g4.checarValidadedaColoracao();
                            if (!validadeDSATUR_CG)
                            {
                                cout << "Erro: Coloração inválida detectada na instância " << nomeInstancia << " para a heurística DSATUR + Color-Group!" << endl;
                                continue;
                            }

                            string resSimples = to_string(cSimples) + "->" + to_string(cgSimples) + "cores";
                            string resLF = to_string(cLF) + "->" + to_string(cgLF) + "cores";
                            string resSL = to_string(cSL) + "->" + to_string(cgSL) + "cores";
                            string resDSATUR = to_string(cDSATUR) + "->" + to_string(cgDSATUR) + "cores";

                            cout << left << setw(40) << nomeInstancia
                                 << setw(35) << resSimples
                                 << setw(35) << resLF
                                 << setw(35) << resSL
                                 << setw(35) << resDSATUR << endl;

                            delete grafoAtual;
                        }
                    }
                }
                break;
            }
            case 5:
                if (g != nullptr)
                {
                    Coloracao col;
                    cout << "\n========================================================" << endl;
                    cout << "EXECUTANDO COMPARATIVO: " << g->getNome() << endl;
                    cout << "========================================================" << endl;

                    // Ordem padrao usada pelo algoritmo Simples
                    vector<int> ordem;
                    for (auto v : g->getVertices())
                        ordem.push_back(v->getId());

                    int colWidth = 20;

                    // ---------------------------------------------------------
                    // 1. HEURÍSTICA: SIMPLES
                    // ---------------------------------------------------------
                    cout << "\n[ SIMPLES ]" << endl;

                    Grafo gS1(g);
                    int s_ich = col.coloracaoSimples(gS1, ordem);
                    gS1.checarValidadedaColoracao();
                    cout << "  " << left << setw(colWidth) << "ICH" << ": X(g)   = " << s_ich << endl;

                    Grafo gS2(g);
                    col.coloracaoSimples(gS2, ordem);
                    int s_cg = col.coloracaoColorGroup(gS2);
                    gS2.checarValidadedaColoracao();
                    cout << "  " << left << setw(colWidth) << "ICH + CG" << ": X(g)   = " << s_cg << endl;

                    Grafo gS3(g);
                    double s_frac = col.coloracaoFracionaria(gS3, SIMPLES).limiteFracionario;
                    gS3.checarValidadedaColoracaoFracionaria();
                    cout << "  " << left << setw(colWidth) << "Fracionaria" << ": X_f(g) = " << fixed << setprecision(2) << s_frac << endl;

                    Grafo gS4(g);
                    double s_frac_cg = col.coloracaoFracionaria(gS4, SIMPLES_CG).limiteFracionario;
                    gS4.checarValidadedaColoracaoFracionaria();
                    cout << "  " << left << setw(colWidth) << "Fracionaria + CG" << ": X_f(g) = " << fixed << setprecision(2) << s_frac_cg << endl;

                    // ---------------------------------------------------------
                    // 2. HEURÍSTICA: LARGEST FIRST (LF)
                    // ---------------------------------------------------------
                    cout << "\n[ LARGEST FIRST ]" << endl;

                    Grafo gLF1(g);
                    int lf_ich = col.coloracaoLargestFirst(gLF1);
                    gLF1.checarValidadedaColoracao();
                    cout << "  " << left << setw(colWidth) << "ICH" << ": X(g)   = " << lf_ich << endl;

                    Grafo gLF2(g);
                    col.coloracaoLargestFirst(gLF2);
                    int lf_cg = col.coloracaoColorGroup(gLF2);
                    gLF2.checarValidadedaColoracao();
                    cout << "  " << left << setw(colWidth) << "ICH + CG" << ": X(g)   = " << lf_cg << endl;

                    Grafo gLF3(g);
                    double lf_frac = col.coloracaoFracionaria(gLF3, LARGEST_FIRST).limiteFracionario;
                    gLF3.checarValidadedaColoracaoFracionaria();
                    cout << "  " << left << setw(colWidth) << "Fracionaria" << ": X_f(g) = " << fixed << setprecision(2) << lf_frac << endl;

                    Grafo gLF4(g);
                    double lf_frac_cg = col.coloracaoFracionaria(gLF4, LARGEST_FIRST_CG).limiteFracionario;
                    gLF4.checarValidadedaColoracaoFracionaria();
                    cout << "  " << left << setw(colWidth) << "Fracionaria + CG" << ": X_f(g) = " << fixed << setprecision(2) << lf_frac_cg << endl;

                    // ---------------------------------------------------------
                    // 3. HEURÍSTICA: SMALLEST LAST (SL)
                    // ---------------------------------------------------------
                    cout << "\n[ SMALLEST LAST ]" << endl;

                    Grafo gSL1(g);
                    int sl_ich = col.coloracaoSmallestLast(gSL1);
                    gSL1.checarValidadedaColoracao();
                    cout << "  " << left << setw(colWidth) << "ICH" << ": X(g)   = " << sl_ich << endl;

                    Grafo gSL2(g);
                    col.coloracaoSmallestLast(gSL2);
                    int sl_cg = col.coloracaoColorGroup(gSL2);
                    gSL2.checarValidadedaColoracao();
                    cout << "  " << left << setw(colWidth) << "ICH + CG" << ": X(g)   = " << sl_cg << endl;

                    Grafo gSL3(g);
                    double sl_frac = col.coloracaoFracionaria(gSL3, SMALLEST_LAST).limiteFracionario;
                    gSL3.checarValidadedaColoracaoFracionaria();
                    cout << "  " << left << setw(colWidth) << "Fracionaria" << ": X_f(g) = " << fixed << setprecision(2) << sl_frac << endl;

                    Grafo gSL4(g);
                    double sl_frac_cg = col.coloracaoFracionaria(gSL4, SMALLEST_LAST_CG).limiteFracionario;
                    gSL4.checarValidadedaColoracaoFracionaria();
                    cout << "  " << left << setw(colWidth) << "Fracionaria + CG" << ": X_f(g) = " << fixed << setprecision(2) << sl_frac_cg << endl;

                    // ---------------------------------------------------------
                    // 4. HEURÍSTICA: DSATUR
                    // ---------------------------------------------------------
                    cout << "\n[ DSATUR ]" << endl;

                    Grafo gD1(g);
                    int d_ich = col.coloracaoDSATUR(gD1);
                    gD1.checarValidadedaColoracao();
                    cout << "  " << left << setw(colWidth) << "ICH" << ": X(g)   = " << d_ich << endl;

                    Grafo gD2(g);
                    col.coloracaoDSATUR(gD2);
                    int d_cg = col.coloracaoColorGroup(gD2);
                    gD2.checarValidadedaColoracao();
                    cout << "  " << left << setw(colWidth) << "ICH + CG" << ": X(g)   = " << d_cg << endl;

                    Grafo gD3(g);
                    double d_frac = col.coloracaoFracionaria(gD3, DSATUR).limiteFracionario;
                    gD3.checarValidadedaColoracaoFracionaria();
                    cout << "  " << left << setw(colWidth) << "Fracionaria" << ": X_f(g) = " << fixed << setprecision(2) << d_frac << endl;

                    Grafo gD4(g);
                    double d_frac_cg = col.coloracaoFracionaria(gD4, DSATUR_CG).limiteFracionario;
                    gD4.checarValidadedaColoracaoFracionaria();
                    cout << "  " << left << setw(colWidth) << "Fracionaria + CG" << ": X_f(g) = " << fixed << setprecision(2) << d_frac_cg << endl;

                    cout << "\n========================================================" << endl;
                }
                else
                {
                    cout << "Nenhum grafo carregado na memoria! Use a opcao 1 primeiro." << endl;
                }
                break;

            case 6:
            {
                cout << "Digite o nome da pasta (ex: instancias): ";
                cin >> nomePasta;
                string caminhoPasta = nomePasta + "/";

                if (!fs::exists(caminhoPasta) || !fs::is_directory(caminhoPasta))
                {
                    cout << "A pasta '" << caminhoPasta << "' nao foi encontrada!" << endl;
                    break;
                }

                cout << "\nProcessando FRACIONARIA em todas as instancias da pasta '" << caminhoPasta << "'...\n"
                     << endl;

                cout << left << setw(40) << "Instancia"
                     << setw(35) << "F_Simples(CG)"
                     << setw(35) << "F_LF(CG)"
                     << setw(35) << "F_SL(CG)"
                     << setw(35) << "F_DSATUR(CG)" << endl;
                cout << string(140, '-') << endl;

                auto formatarDouble = [](double valor)
                {
                    stringstream stream;
                    stream << fixed << setprecision(2) << valor;
                    return stream.str();
                };

                for (const auto &entry : fs::directory_iterator(caminhoPasta))
                {
                    if (entry.is_regular_file())
                    {
                        string pathAtual = entry.path().string();
                        string nomeInstancia = entry.path().filename().string();

                        Grafo *grafoAtual = Grafo::lerGrafoDeArquivo(pathAtual);
                        if (grafoAtual != nullptr)
                        {
                            Coloracao col;

                            Grafo gS(grafoAtual), gS_CG(grafoAtual);
                            Grafo gLF(grafoAtual), gLF_CG(grafoAtual);
                            Grafo gSL(grafoAtual), gSL_CG(grafoAtual);
                            Grafo gD(grafoAtual), gD_CG(grafoAtual);

                            // 1. SIMPLES
                            ColoracaoResult fSimples = col.coloracaoFracionaria(gS, SIMPLES);
                            if (!gS.checarValidadedaColoracaoFracionaria())
                            {
                                cout << "Erro de validade (Fracionaria Simples) na instancia " << nomeInstancia << endl;
                                continue;
                            }

                            ColoracaoResult fSimples_CG = col.coloracaoFracionaria(gS_CG, SIMPLES_CG);
                            if (!gS_CG.checarValidadedaColoracaoFracionaria())
                            {
                                cout << "Erro de validade (Fracionaria Simples+CG) na instancia " << nomeInstancia << endl;
                                continue;
                            }

                            // 2. LARGEST FIRST
                            ColoracaoResult fLF = col.coloracaoFracionaria(gLF, LARGEST_FIRST);
                            if (!gLF.checarValidadedaColoracaoFracionaria())
                            {
                                cout << "Erro de validade (Fracionaria LF) na instancia " << nomeInstancia << endl;
                                continue;
                            }

                            ColoracaoResult fLF_CG = col.coloracaoFracionaria(gLF_CG, LARGEST_FIRST_CG);
                            if (!gLF_CG.checarValidadedaColoracaoFracionaria())
                            {
                                cout << "Erro de validade (Fracionaria LF+CG) na instancia " << nomeInstancia << endl;
                                continue;
                            }

                            // 3. SMALLEST LAST
                            ColoracaoResult fSL = col.coloracaoFracionaria(gSL, SMALLEST_LAST);
                            if (!gSL.checarValidadedaColoracaoFracionaria())
                            {
                                cout << "Erro de validade (Fracionaria SL) na instancia " << nomeInstancia << endl;
                                continue;
                            }

                            ColoracaoResult fSL_CG = col.coloracaoFracionaria(gSL_CG, SMALLEST_LAST_CG);
                            if (!gSL_CG.checarValidadedaColoracaoFracionaria())
                            {
                                cout << "Erro de validade (Fracionaria SL+CG) na instancia " << nomeInstancia << endl;
                                continue;
                            }

                            // 4. DSATUR
                            ColoracaoResult fDSATUR = col.coloracaoFracionaria(gD, DSATUR);
                            if (!gD.checarValidadedaColoracaoFracionaria())
                            {
                                cout << "Erro de validade (Fracionaria DSATUR) na instancia " << nomeInstancia << endl;
                                continue;
                            }

                            ColoracaoResult fDSATUR_CG = col.coloracaoFracionaria(gD_CG, DSATUR_CG);
                            if (!gD_CG.checarValidadedaColoracaoFracionaria())
                            {
                                cout << "Erro de validade (Fracionaria DSATUR+CG) na instancia " << nomeInstancia << endl;
                                continue;
                            }

                            // Formatando os resultados para a exibição (Ex: "2.50->2.50")
                            string resSimples = formatarDouble(fSimples.limiteFracionario) + "(" + to_string(fSimples.numCores) + "/" + to_string(fSimples.numIteracoes) + ")" + "->" + formatarDouble(fSimples_CG.limiteFracionario) + "(" + to_string(fSimples_CG.numCores) + "/" + to_string(fSimples_CG.numIteracoes) + ")";

                            string resLF = formatarDouble(fLF.limiteFracionario) + "(" + to_string(fLF.numCores) + "/" + to_string(fLF.numIteracoes) + ")" + "->" + formatarDouble(fLF_CG.limiteFracionario) + "(" + to_string(fLF_CG.numCores) + "/" + to_string(fLF_CG.numIteracoes) + ")";

                            string resSL = formatarDouble(fSL.limiteFracionario) + "(" + to_string(fSL.numCores) + "/" + to_string(fSL.numIteracoes) + ")" + "->" + formatarDouble(fSL_CG.limiteFracionario) + "(" + to_string(fSL_CG.numCores) + "/" + to_string(fSL_CG.numIteracoes) + ")";

                            string resDSATUR = formatarDouble(fDSATUR.limiteFracionario) + "(" + to_string(fDSATUR.numCores) + "/" + to_string(fDSATUR.numIteracoes) + ")" + "->" + formatarDouble(fDSATUR_CG.limiteFracionario) + "(" + to_string(fDSATUR_CG.numCores) + "/" + to_string(fDSATUR_CG.numIteracoes) + ")";

                            cout << left << setw(40) << nomeInstancia
                                 << setw(35) << resSimples
                                 << setw(35) << resLF
                                 << setw(35) << resSL
                                 << setw(35) << resDSATUR << endl;

                            delete grafoAtual;
                        }
                    }
                }
                break;
            }

            case 7:
            {
                cout << "Digite o nome da pasta (ex: instancias): ";
                cin >> nomePasta;
                string caminhoPasta = nomePasta + "/";

                if (!fs::exists(caminhoPasta) || !fs::is_directory(caminhoPasta))
                {
                    cout << "A pasta '" << caminhoPasta << "' nao foi encontrada!" << endl;
                    break;
                }

                ofstream arquivoCSV("result_" + nomePasta + "_.csv");
                if (!arquivoCSV.is_open())
                {
                    cout << "Erro ao criar o arquivo result_" + nomePasta + "_.csv! Verifique as permissoes da pasta." << endl;
                    break;
                }

                cout << "\nIniciando bateria completa de testes. Os resultados serao salvos em 'result_" + nomePasta + "_.csv'..." << endl;
                cout << "Isso pode levar alguns minutos dependendo do tamanho e quantidade dos grafos.\n"
                     << endl;

                arquivoCSV << "Instancia;"
                           << "Simples;Simples_CG;Simples_F;Simples_F_CG;"
                           << "LF;LF_CG;LF_F;LF_F_CG;"
                           << "SL;SL_CG;SL_F;SL_F_CG;"
                           << "DSATUR;DSATUR_CG;DSATUR_F;DSATUR_F_CG\n";

                arquivoCSV << fixed << setprecision(2);

                auto formatarDouble = [](double valor)
                {
                    stringstream stream;
                    stream << fixed << setprecision(2) << valor;
                    return stream.str();
                };

                for (const auto &entry : fs::directory_iterator(caminhoPasta))
                {
                    if (entry.is_regular_file())
                    {
                        string pathAtual = entry.path().string();
                        string nomeInstancia = entry.path().filename().string();

                        Grafo *grafoAtual = Grafo::lerGrafoDeArquivo(pathAtual);
                        if (grafoAtual != nullptr)
                        {
                            cout << "Processando: " << nomeInstancia << "..." << endl;

                            Coloracao col;

                            vector<int> ordem;
                            for (auto v : grafoAtual->getVertices())
                                ordem.push_back(v->getId());

                            // =======================================================
                            // 1. SIMPLES
                            // =======================================================
                            Grafo gS1(grafoAtual);
                            int s_ich = col.coloracaoSimples(gS1, ordem);

                            Grafo gS2(grafoAtual);
                            col.coloracaoSimples(gS2, ordem);
                            int s_cg = col.coloracaoColorGroup(gS2);

                            Grafo gS3(grafoAtual);
                            ColoracaoResult s_frac = col.coloracaoFracionaria(gS3, SIMPLES);
                            string s_frac_str = formatarDouble(s_frac.limiteFracionario) +
                                                "(" + to_string(s_frac.numCores) +
                                                "/" + to_string(s_frac.numIteracoes) + ")";

                            Grafo gS4(grafoAtual);
                            ColoracaoResult s_frac_cg = col.coloracaoFracionaria(gS4, SIMPLES_CG);
                            string s_frac_cg_str = formatarDouble(s_frac_cg.limiteFracionario) +
                                                   "(" + to_string(s_frac_cg.numCores) +
                                                   "/" + to_string(s_frac_cg.numIteracoes) + ")";

                            // =======================================================
                            // 2. LARGEST FIRST (LF)
                            // =======================================================
                            Grafo gLF1(grafoAtual);
                            int lf_ich = col.coloracaoLargestFirst(gLF1);

                            Grafo gLF2(grafoAtual);
                            col.coloracaoLargestFirst(gLF2);
                            int lf_cg = col.coloracaoColorGroup(gLF2);

                            Grafo gLF3(grafoAtual);
                            ColoracaoResult lf_frac = col.coloracaoFracionaria(gLF3, LARGEST_FIRST);
                            string lf_frac_str = formatarDouble(lf_frac.limiteFracionario) +
                                                 "(" + to_string(lf_frac.numCores) +
                                                 "/" + to_string(lf_frac.numIteracoes) + ")";

                            Grafo gLF4(grafoAtual);
                            ColoracaoResult lf_frac_cg = col.coloracaoFracionaria(gLF4, LARGEST_FIRST_CG);
                            string lf_frac_cg_str = formatarDouble(lf_frac_cg.limiteFracionario) +
                                                    "(" + to_string(lf_frac_cg.numCores) +
                                                    "/" + to_string(lf_frac_cg.numIteracoes) + ")";

                            // =======================================================
                            // 3. SMALLEST LAST (SL)
                            // =======================================================
                            Grafo gSL1(grafoAtual);
                            int sl_ich = col.coloracaoSmallestLast(gSL1);

                            Grafo gSL2(grafoAtual);
                            col.coloracaoSmallestLast(gSL2);
                            int sl_cg = col.coloracaoColorGroup(gSL2);

                            Grafo gSL3(grafoAtual);
                            ColoracaoResult sl_frac = col.coloracaoFracionaria(gSL3, SMALLEST_LAST);
                            string sl_frac_str = formatarDouble(sl_frac.limiteFracionario) +
                                                 "(" + to_string(sl_frac.numCores) +
                                                 "/" + to_string(sl_frac.numIteracoes) + ")";

                            Grafo gSL4(grafoAtual);
                            ColoracaoResult sl_frac_cg = col.coloracaoFracionaria(gSL4, SMALLEST_LAST_CG);
                            string sl_frac_cg_str = formatarDouble(sl_frac_cg.limiteFracionario) +
                                                    "(" + to_string(sl_frac_cg.numCores) +
                                                    "/" + to_string(sl_frac_cg.numIteracoes) + ")";

                            // =======================================================
                            // 4. DSATUR
                            // =======================================================
                            Grafo gD1(grafoAtual);
                            int d_ich = col.coloracaoDSATUR(gD1);

                            Grafo gD2(grafoAtual);
                            col.coloracaoDSATUR(gD2);
                            int d_cg = col.coloracaoColorGroup(gD2);

                            Grafo gD3(grafoAtual);
                            ColoracaoResult d_frac = col.coloracaoFracionaria(gD3, DSATUR);
                            string d_frac_str = formatarDouble(d_frac.limiteFracionario) +
                                                "(" + to_string(d_frac.numCores) +
                                                "/" + to_string(d_frac.numIteracoes) + ")";

                            Grafo gD4(grafoAtual);
                            ColoracaoResult d_frac_cg = col.coloracaoFracionaria(gD4, DSATUR_CG);
                            string d_frac_cg_str = formatarDouble(d_frac_cg.limiteFracionario) +
                                                   "(" + to_string(d_frac_cg.numCores) +
                                                   "/" + to_string(d_frac_cg.numIteracoes) + ")";

                            // =======================================================
                            // ESCREVER NO CSV
                            // =======================================================
                            arquivoCSV << nomeInstancia << ";"
                                       << s_ich << ";" << s_cg << ";" << s_frac_str << ";" << s_frac_cg_str << ";"
                                       << lf_ich << ";" << lf_cg << ";" << lf_frac_str << ";" << lf_frac_cg_str << ";"
                                       << sl_ich << ";" << sl_cg << ";" << sl_frac_str << ";" << sl_frac_cg_str << ";"
                                       << d_ich << ";" << d_cg << ";" << d_frac_str << ";" << d_frac_cg_str
                                       << "\n";

                            delete grafoAtual;
                        }
                    }
                }

                arquivoCSV.close();
                cout << "\nSucesso! Todos os resultados foram salvos no arquivo 'result.csv'." << endl;
                break;
            }

            case 8:
                cout << "Encerrando o programa..." << endl;
                break;
            default:
                cout << "Opcao invalida! Tente novamente." << endl;
                break;
            }

            cout << endl;

        } while (escolha != 8);

        if (g != nullptr)
        {
            delete g;
            cout << "Memoria do grafo liberada." << endl;
        }
    }
};

#endif