#ifndef LISTA_H
#define LISTA_H

#include "alimento.h"

/* Declaração antecipada, só para a categoria poder apontar para as árvores */
struct NoArvore;

/* Nó da lista ligada de alimentos (fica em ordem alfabética da descrição) */
typedef struct NoAlimento {
    AlimentoRegistro dados;
    struct NoAlimento *proximo;
} NoAlimento;

/* Nó da lista ligada de categorias (ordem alfabética), com sua lista de alimentos e as duas árvores */
typedef struct NoCategoria {
    CategoriaAlimento categoria;
    char nomeCategoria[60];
    NoAlimento *inicioAlimentos;
    struct NoArvore *arvoreEnergia;
    struct NoArvore *arvoreProteina;
    struct NoCategoria *proximo;
} NoCategoria;

/* Funções da lista de alimentos */
NoAlimento* criarNoAlimento(AlimentoRegistro reg);
NoAlimento* inserirAlimentoOrdenado(NoAlimento *inicio, AlimentoRegistro reg);
NoAlimento* removerAlimentoDaLista(NoAlimento *inicio, int numero, int *removido);
void liberarListaAlimentos(NoAlimento *inicio);

/* Funções da lista de categorias */
NoCategoria* criarNoCategoria(CategoriaAlimento cat);
NoCategoria* inserirCategoriaOrdenada(NoCategoria *inicio, CategoriaAlimento cat);
NoCategoria* buscarCategoriaPorCodigo(NoCategoria *inicio, CategoriaAlimento cat);
NoCategoria* buscarCategoriaPorIndice(NoCategoria *inicio, int indice);
NoCategoria* removerCategoria(NoCategoria *inicio, CategoriaAlimento cat, int *removido);
void liberarListaCategorias(NoCategoria *inicio);

/* Refaz as duas árvores de uma categoria a partir da lista de alimentos */
void reconstruirArvoresCategoria(NoCategoria *noCat);

#endif
