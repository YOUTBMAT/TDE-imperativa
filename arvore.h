#ifndef ARVORE_H
#define ARVORE_H

#include "lista.h"

/* Nó da árvore: a chave (energia ou proteína) e um ponteiro para o alimento na lista */
typedef struct NoArvore {
    float chave;
    struct NoAlimento *alimento;
    struct NoArvore *esquerda;
    struct NoArvore *direita;
} NoArvore;

/* Criação e inserção */
NoArvore* criarNoArvore(float chave, struct NoAlimento *alimento);
NoArvore* inserirNaArvore(NoArvore *raiz, float chave, struct NoAlimento *alimento);

/* Listagens que usam a árvore */
void listarArvoreDecrescente(NoArvore *raiz, int ehEnergia);
void listarArvoreIntervalo(NoArvore *raiz, float min, float max, int ehEnergia, int *encontrou);

/* Libera a árvore inteira */
void liberarArvore(NoArvore *raiz);

#endif
