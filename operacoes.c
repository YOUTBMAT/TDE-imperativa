#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "operacoes.h"
#include "arvore.h"

/* Cria as 15 categorias, lê cada registro do arquivo, coloca na categoria certa e, no fim, monta as árvores */
NoCategoria* carregarDadosBinarios(const char *nomeArquivoBin) {
    FILE *arquivoBinario = fopen(nomeArquivoBin, "rb");
    NoCategoria *listaCategorias = NULL;
    AlimentoRegistro registro;
    int codigoCategoria;
    NoCategoria *noCategoria = NULL;

    if (arquivoBinario == NULL) {
        printf("Erro fatal: Nao foi possivel abrir o arquivo binario %s.\n", nomeArquivoBin);
        printf("Certifique-se de executar o programa P1 primeiro!\n");
        exit(1);
    }

    for (codigoCategoria = 1; codigoCategoria <= TOTAL_CATEGORIAS; codigoCategoria++) {
        listaCategorias = inserirCategoriaOrdenada(listaCategorias, (CategoriaAlimento)codigoCategoria);
    }

    while (fread(&registro, sizeof(AlimentoRegistro), 1, arquivoBinario) == 1) {
        noCategoria = buscarCategoriaPorCodigo(listaCategorias, registro.categoria);
        if (noCategoria != NULL) {
            noCategoria->inicioAlimentos = inserirAlimentoOrdenado(noCategoria->inicioAlimentos, registro);
        }
    }
    fclose(arquivoBinario);

    noCategoria = listaCategorias;
    while (noCategoria != NULL) {
        reconstruirArvoresCategoria(noCategoria);
        noCategoria = noCategoria->proximo;
    }

    return listaCategorias;
}

/* Escreve todos os alimentos que ainda existem, categoria por categoria */
void salvarDadosBinarios(const char *nomeArquivoBin, NoCategoria *inicioCategorias) {
    FILE *arquivoBinario = fopen(nomeArquivoBin, "wb");

    if (arquivoBinario == NULL) {
        printf("Erro ao regravar arquivo binario %s\n", nomeArquivoBin);
    } else {
        NoCategoria *categoriaAtual = inicioCategorias;
        int total = 0;

        while (categoriaAtual != NULL) {
            NoAlimento *alimentoAtual = categoriaAtual->inicioAlimentos;
            while (alimentoAtual != NULL) {
                fwrite(&(alimentoAtual->dados), sizeof(AlimentoRegistro), 1, arquivoBinario);
                total++;
                alimentoAtual = alimentoAtual->proximo;
            }
            categoriaAtual = categoriaAtual->proximo;
        }

        fclose(arquivoBinario);
        printf("Arquivo binario %s atualizado com sucesso (%d alimentos gravados).\n", nomeArquivoBin, total);
    }
}

/* Lê e joga fora tudo até o fim da linha */
void limparEntrada(void) {
    int caractere = getchar();
    while (caractere != '\n' && caractere != EOF) {
        caractere = getchar();
    }
}

/* Opção 1: mostra as categorias na ordem da lista */
void opListarCategorias(NoCategoria *inicio) {
    NoCategoria *atual = inicio;
    int posicao = 1;

    printf("\n--- LISTA DE CATEGORIAS (Ordem Alfabetica) ---\n");
    if (inicio == NULL) {
        printf("Nenhuma categoria cadastrada.\n");
    } else {
        while (atual != NULL) {
            printf("  %2d. %s\n", posicao, atual->nomeCategoria);
            posicao++;
            atual = atual->proximo;
        }
    }
}

/* Mostra as categorias e pede o número; devolve NULL se a escolha for inválida */
static NoCategoria* selecionarCategoria(NoCategoria *inicio) {
    NoCategoria *selecionada = NULL;
    int numeroEscolhido = 0;

    opListarCategorias(inicio);
    if (inicio != NULL) {
        printf("\nDigite o numero da categoria desejada: ");
        if (scanf("%d", &numeroEscolhido) == 1) {
            selecionada = buscarCategoriaPorIndice(inicio, numeroEscolhido);
            if (selecionada == NULL) {
                printf("Categoria invalida!\n");
            }
        } else {
            printf("Entrada invalida!\n");
            limparEntrada();
        }
    }

    return selecionada;
}

/* Pede um número (mínimo ou máximo) e devolve 1 se a leitura deu certo */
static int lerValorReal(const char *limite, const char *grandeza, float *valor) {
    int leituraOk = 0;

    printf("Informe o valor %s de %s: ", limite, grandeza);
    leituraOk = scanf("%f", valor);
    if (leituraOk != 1) {
        printf("Entrada invalida!\n");
        limparEntrada();
    }

    return leituraOk == 1;
}

/* Opções 3 e 4: mesma lógica, só muda qual árvore usar (energia ou proteína) */
static void listarDecrescentePorChave(NoCategoria *inicio, int ehEnergia) {
    NoCategoria *categoria = selecionarCategoria(inicio);

    if (categoria != NULL) {
        NoArvore *arvore = categoria->arvoreProteina;
        const char *grandeza = "PROTEINA";

        if (ehEnergia == 1) {
            arvore = categoria->arvoreEnergia;
            grandeza = "ENERGIA";
        }

        printf("\n--- ALIMENTOS DE '%s' EM ORDEM DECRESCENTE DE %s ---\n", categoria->nomeCategoria, grandeza);
        if (arvore == NULL) {
            printf("Nenhum alimento indexado na arvore.\n");
        } else {
            listarArvoreDecrescente(arvore, ehEnergia);
        }
    }
}

/* Opções 5 e 6: pede mínimo e máximo e consulta a árvore escolhida */
static void listarFaixaPorChave(NoCategoria *inicio, int ehEnergia) {
    NoCategoria *categoria = selecionarCategoria(inicio);

    if (categoria != NULL) {
        NoArvore *arvore = categoria->arvoreProteina;
        const char *grandeza = "proteina (g)";
        const char *titulo = "PROTEINA";
        const char *unidade = "g";
        int casasDecimais = 2;
        float minimo = 0.0f;
        float maximo = 0.0f;
        int encontrou = 0;

        if (ehEnergia == 1) {
            arvore = categoria->arvoreEnergia;
            grandeza = "energia (Kcal)";
            titulo = "ENERGIA";
            unidade = "kcal";
            casasDecimais = 1;
        }

        if (lerValorReal("MINIMO", grandeza, &minimo) == 1 && lerValorReal("MAXIMO", grandeza, &maximo) == 1) {
            printf("\n--- ALIMENTOS DE '%s' COM %s ENTRE %.*f E %.*f %s ---\n",
                   categoria->nomeCategoria, titulo, casasDecimais, minimo, casasDecimais, maximo, unidade);
            listarArvoreIntervalo(arvore, minimo, maximo, ehEnergia, &encontrou);

            if (encontrou == 0) {
                printf("Nenhum alimento encontrado nesta faixa.\n");
            }
        }
    }
}

/* Opção 2: alimentos da categoria na ordem da lista (alfabética) */
void opListarAlimentosCategoria(NoCategoria *inicio) {
    NoCategoria *categoria = selecionarCategoria(inicio);

    if (categoria != NULL) {
        NoAlimento *alimento = categoria->inicioAlimentos;
        int totalAlimentos = 0;

        printf("\n--- ALIMENTOS DA CATEGORIA: %s ---\n", categoria->nomeCategoria);
        if (alimento == NULL) {
            printf("Nenhum alimento nesta categoria.\n");
        } else {
            while (alimento != NULL) {
                printf("  [%3d] %-45s | ", alimento->dados.numero, alimento->dados.descricao);
                if (alimento->dados.energia >= 0.0f) {
                    printf("Energia: %6.1f kcal | ", alimento->dados.energia);
                } else {
                    printf("Energia: [Nao disp] | ");
                }
                if (alimento->dados.proteina >= 0.0f) {
                    printf("Proteina: %6.2f g\n", alimento->dados.proteina);
                } else {
                    printf("Proteina: [Nao disp]\n");
                }
                totalAlimentos++;
                alimento = alimento->proximo;
            }
            printf("Total: %d alimentos encontrados.\n", totalAlimentos);
        }
    }
}

/* Opção 3 */
void opListarDecrescenteEnergia(NoCategoria *inicio) {
    listarDecrescentePorChave(inicio, 1);
}

/* Opção 4 */
void opListarDecrescenteProteina(NoCategoria *inicio) {
    listarDecrescentePorChave(inicio, 0);
}

/* Opção 5 */
void opListarFaixaEnergia(NoCategoria *inicio) {
    listarFaixaPorChave(inicio, 1);
}

/* Opção 6 */
void opListarFaixaProteina(NoCategoria *inicio) {
    listarFaixaPorChave(inicio, 0);
}

/* Opção 7: remove a categoria escolhida e devolve o início da lista (que pode mudar) */
NoCategoria* opRemoverCategoria(NoCategoria *inicio, int *modificado) {
    NoCategoria *resultado = inicio;
    NoCategoria *categoria = selecionarCategoria(inicio);

    if (categoria != NULL) {
        int removido = 0;
        CategoriaAlimento codigo = categoria->categoria;
        /* Guarda o nome antes, porque o nó vai ser liberado */
        char nome[60];
        strncpy(nome, categoria->nomeCategoria, sizeof(nome) - 1);
        nome[sizeof(nome) - 1] = '\0';

        resultado = removerCategoria(inicio, codigo, &removido);
        if (removido == 1) {
            printf("\nCategoria '%s' removida com sucesso!\n", nome);
            *modificado = 1;
        } else {
            printf("\nFalha ao remover a categoria.\n");
        }
    }

    return resultado;
}

/* Opção 8: remove um alimento e refaz as árvores da categoria */
void opRemoverAlimento(NoCategoria *inicio, int *modificado) {
    NoCategoria *categoria = selecionarCategoria(inicio);

    if (categoria != NULL) {
        int numeroAlimento = 0;
        int removido = 0;
        NoAlimento *alimento = categoria->inicioAlimentos;

        printf("\n--- ALIMENTOS EM '%s' ---\n", categoria->nomeCategoria);
        while (alimento != NULL) {
            printf("  [%3d] %s\n", alimento->dados.numero, alimento->dados.descricao);
            alimento = alimento->proximo;
        }

        printf("\nDigite o NUMERO do alimento a remover: ");
        if (scanf("%d", &numeroAlimento) == 1) {
            categoria->inicioAlimentos = removerAlimentoDaLista(categoria->inicioAlimentos, numeroAlimento, &removido);

            if (removido == 1) {
                /* Como a lista mudou, as árvores são recriadas */
                reconstruirArvoresCategoria(categoria);
                printf("Alimento numero %d removido com sucesso e arvores atualizadas!\n", numeroAlimento);
                *modificado = 1;
            } else {
                printf("Alimento numero %d nao encontrado nesta categoria.\n", numeroAlimento);
            }
        } else {
            printf("Entrada invalida!\n");
            limparEntrada();
        }
    }
}
