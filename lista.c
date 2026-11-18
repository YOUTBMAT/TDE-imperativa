#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "arvore.h"

/* Aloca (malloc) um nó novo para o alimento; se faltar memória, encerra o programa */
NoAlimento* criarNoAlimento(AlimentoRegistro reg) {
    NoAlimento *novo = (NoAlimento*) malloc(sizeof(NoAlimento));
    if (novo == NULL) {
        printf("Erro fatal: Falha de alocacao dinamica para alimento.\n");
        exit(1);
    }
    novo->dados = reg;
    novo->proximo = NULL;
    return novo;
}

/* Insere o alimento já na posição certa, assim a lista nunca precisa ser ordenada depois */
NoAlimento* inserirAlimentoOrdenado(NoAlimento *inicio, AlimentoRegistro reg) {
    NoAlimento *novo = criarNoAlimento(reg);
    NoAlimento *resultado = inicio;

    /* Lista vazia ou o novo vai ficar no começo */
    if (inicio == NULL || compararTextoAlfabetico(reg.descricao, inicio->dados.descricao) < 0) {
        novo->proximo = inicio;
        resultado = novo;
    } else {
        NoAlimento *anterior = inicio;
        NoAlimento *atual = inicio->proximo;
        int inserido = 0;

        while (atual != NULL && inserido == 0) {
            if (compararTextoAlfabetico(reg.descricao, atual->dados.descricao) < 0) {
                anterior->proximo = novo;
                novo->proximo = atual;
                inserido = 1;
            } else {
                anterior = atual;
                atual = atual->proximo;
            }
        }

        /* Se não achou ninguém maior, vai para o final */
        if (inserido == 0) {
            anterior->proximo = novo;
        }
    }

    return resultado;
}

/* Remove o alimento pelo número e dá free no nó; 'removido' avisa se ele foi encontrado */
NoAlimento* removerAlimentoDaLista(NoAlimento *inicio, int numero, int *removido) {
    NoAlimento *resultado = inicio;
    *removido = 0;

    if (inicio != NULL) {
        /* Era o primeiro da lista */
        if (inicio->dados.numero == numero) {
            NoAlimento *remover = inicio;
            resultado = inicio->proximo;
            free(remover);
            *removido = 1;
        } else {
            NoAlimento *anterior = inicio;
            NoAlimento *atual = inicio->proximo;

            /* Procura o nó, guardando o anterior para religar a lista */
            while (atual != NULL && *removido == 0) {
                if (atual->dados.numero == numero) {
                    anterior->proximo = atual->proximo;
                    free(atual);
                    *removido = 1;
                } else {
                    anterior = atual;
                    atual = atual->proximo;
                }
            }
        }
    }

    return resultado;
}

/* Dá free em todos os nós (guarda o próximo antes de liberar o atual) */
void liberarListaAlimentos(NoAlimento *inicio) {
    NoAlimento *atual = inicio;
    while (atual != NULL) {
        NoAlimento *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
}

/* Aloca (malloc) uma categoria vazia, já com o nome vindo do enum */
NoCategoria* criarNoCategoria(CategoriaAlimento cat) {
    NoCategoria *novo = (NoCategoria*) malloc(sizeof(NoCategoria));
    if (novo == NULL) {
        printf("Erro fatal: Falha de alocacao dinamica para categoria.\n");
        exit(1);
    }
    novo->categoria = cat;
    strncpy(novo->nomeCategoria, obterNomeCategoria(cat), sizeof(novo->nomeCategoria) - 1);
    novo->nomeCategoria[sizeof(novo->nomeCategoria) - 1] = '\0';
    novo->inicioAlimentos = NULL;
    novo->arvoreEnergia = NULL;
    novo->arvoreProteina = NULL;
    novo->proximo = NULL;
    return novo;
}

/* Mesma ideia da lista de alimentos: insere na posição certa pelo nome */
NoCategoria* inserirCategoriaOrdenada(NoCategoria *inicio, CategoriaAlimento cat) {
    NoCategoria *novo = criarNoCategoria(cat);
    NoCategoria *resultado = inicio;

    /* Lista vazia ou o novo vai ficar no começo */
    if (inicio == NULL || compararTextoAlfabetico(novo->nomeCategoria, inicio->nomeCategoria) < 0) {
        novo->proximo = inicio;
        resultado = novo;
    } else {
        NoCategoria *anterior = inicio;
        NoCategoria *atual = inicio->proximo;
        int inserido = 0;

        while (atual != NULL && inserido == 0) {
            if (compararTextoAlfabetico(novo->nomeCategoria, atual->nomeCategoria) < 0) {
                anterior->proximo = novo;
                novo->proximo = atual;
                inserido = 1;
            } else {
                anterior = atual;
                atual = atual->proximo;
            }
        }

        /* Se não achou ninguém maior, vai para o final */
        if (inserido == 0) {
            anterior->proximo = novo;
        }
    }

    return resultado;
}

/* Procura a categoria pelo enum */
NoCategoria* buscarCategoriaPorCodigo(NoCategoria *inicio, CategoriaAlimento cat) {
    NoCategoria *atual = inicio;
    NoCategoria *encontrado = NULL;

    while (atual != NULL && encontrado == NULL) {
        if (atual->categoria == cat) {
            encontrado = atual;
        } else {
            atual = atual->proximo;
        }
    }

    return encontrado;
}

/* Procura a categoria pela posição no menu (1, 2, 3...) */
NoCategoria* buscarCategoriaPorIndice(NoCategoria *inicio, int indice) {
    NoCategoria *atual = inicio;
    NoCategoria *encontrado = NULL;
    int posicao = 1;

    while (atual != NULL && encontrado == NULL) {
        if (posicao == indice) {
            encontrado = atual;
        } else {
            posicao++;
            atual = atual->proximo;
        }
    }

    return encontrado;
}

/* Libera tudo de uma categoria: alimentos, árvores e o próprio nó */
static void liberarNoCategoria(NoCategoria *noCategoria) {
    liberarListaAlimentos(noCategoria->inicioAlimentos);
    liberarArvore(noCategoria->arvoreEnergia);
    liberarArvore(noCategoria->arvoreProteina);
    free(noCategoria);
}

/* Tira a categoria da lista e libera a memória dela */
NoCategoria* removerCategoria(NoCategoria *inicio, CategoriaAlimento cat, int *removido) {
    NoCategoria *resultado = inicio;
    *removido = 0;

    if (inicio != NULL) {
        if (inicio->categoria == cat) {
            NoCategoria *remover = inicio;
            resultado = inicio->proximo;
            liberarNoCategoria(remover);
            *removido = 1;
        } else {
            NoCategoria *anterior = inicio;
            NoCategoria *atual = inicio->proximo;

            /* Procura o nó, guardando o anterior para religar a lista */
            while (atual != NULL && *removido == 0) {
                if (atual->categoria == cat) {
                    anterior->proximo = atual->proximo;
                    liberarNoCategoria(atual);
                    *removido = 1;
                } else {
                    anterior = atual;
                    atual = atual->proximo;
                }
            }
        }
    }

    return resultado;
}

/* Joga fora as árvores antigas (free) e monta de novo com os alimentos que sobraram */
void reconstruirArvoresCategoria(NoCategoria *noCat) {
    if (noCat != NULL) {
        NoAlimento *atual = NULL;

        liberarArvore(noCat->arvoreEnergia);
        liberarArvore(noCat->arvoreProteina);
        noCat->arvoreEnergia = NULL;
        noCat->arvoreProteina = NULL;

        atual = noCat->inicioAlimentos;
        while (atual != NULL) {
            noCat->arvoreEnergia = inserirNaArvore(noCat->arvoreEnergia, atual->dados.energia, atual);
            noCat->arvoreProteina = inserirNaArvore(noCat->arvoreProteina, atual->dados.proteina, atual);
            atual = atual->proximo;
        }
    }
}

/* Libera todas as categorias; usado no fim do programa */
void liberarListaCategorias(NoCategoria *inicio) {
    NoCategoria *atual = inicio;
    while (atual != NULL) {
        NoCategoria *proximo = atual->proximo;
        liberarNoCategoria(atual);
        atual = proximo;
    }
}
