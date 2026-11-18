#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"
#include "lista.h"

/* Aloca (malloc) um nó da árvore; só guarda o ponteiro, não copia o alimento */
NoArvore* criarNoArvore(float chave, NoAlimento *alimento) {
    NoArvore *novo = (NoArvore*) malloc(sizeof(NoArvore));
    if (novo == NULL) {
        printf("Erro fatal: Falha de alocacao dinamica de memoria na arvore.\n");
        exit(1);
    }
    novo->chave = chave;
    novo->alimento = alimento;
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

/* Insere pela chave: menor ou igual vai para a esquerda, maior vai para a direita */
NoArvore* inserirNaArvore(NoArvore *raiz, float chave, NoAlimento *alimento) {
    NoArvore *resultado = raiz;

    if (raiz == NULL) {
        resultado = criarNoArvore(chave, alimento);
    } else {
        if (chave <= raiz->chave) {
            raiz->esquerda = inserirNaArvore(raiz->esquerda, chave, alimento);
        } else {
            raiz->direita = inserirNaArvore(raiz->direita, chave, alimento);
        }
        resultado = raiz;
    }

    return resultado;
}

/* Mostra uma linha do alimento; valor -1 aparece como não disponível */
static void imprimirNoArvore(const NoArvore *no, int ehEnergia) {
    const char *rotulo = "Proteina";
    const char *unidade = "g";
    int casasDecimais = 2;

    if (ehEnergia == 1) {
        rotulo = "Energia";
        unidade = "kcal";
        casasDecimais = 1;
    }

    printf("  [%3d] %-45s | %s: ", no->alimento->dados.numero, no->alimento->dados.descricao, rotulo);
    if (no->chave >= 0.0f) {
        printf("%6.*f %s\n", casasDecimais, no->chave, unidade);
    } else {
        printf("[Nao disp]\n");
    }
}

/* Direita, raiz, esquerda: do maior para o menor */
void listarArvoreDecrescente(NoArvore *raiz, int ehEnergia) {
    if (raiz != NULL) {
        listarArvoreDecrescente(raiz->direita, ehEnergia);
        imprimirNoArvore(raiz, ehEnergia);
        listarArvoreDecrescente(raiz->esquerda, ehEnergia);
    }
}

/* Lista só os nós entre min e max, sem percorrer a árvore toda */
void listarArvoreIntervalo(NoArvore *raiz, float min, float max, int ehEnergia, int *encontrou) {
    if (raiz != NULL) {
        /* Só vale olhar a esquerda se a chave atual ainda é maior ou igual ao mínimo */
        if (raiz->chave >= min) {
            listarArvoreIntervalo(raiz->esquerda, min, max, ehEnergia, encontrou);
        }

        /* Está dentro da faixa (e o valor existe): imprime */
        if (raiz->chave >= 0.0f && raiz->chave >= min && raiz->chave <= max) {
            *encontrou = 1;
            imprimirNoArvore(raiz, ehEnergia);
        }

        /* Só vale olhar a direita se a chave atual é menor que o máximo */
        if (raiz->chave < max) {
            listarArvoreIntervalo(raiz->direita, min, max, ehEnergia, encontrou);
        }
    }
}

/* Dá free nos filhos primeiro e depois na raiz */
void liberarArvore(NoArvore *raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}
