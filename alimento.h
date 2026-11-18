#ifndef ALIMENTO_H
#define ALIMENTO_H

/* Tamanho máximo da descrição de um alimento */
#define TAMANHO_DESCRICAO 100
/* Quantidade de categorias da tabela */
#define TOTAL_CATEGORIAS 15

/* As categorias de alimentos, na mesma ordem do enunciado */
typedef enum {
    CAT_CEREAIS = 1,
    CAT_VERDURAS,
    CAT_FRUTAS,
    CAT_GORDURAS,
    CAT_PESCADOS,
    CAT_CARNES,
    CAT_LEITE,
    CAT_BEBIDAS,
    CAT_OVOS,
    CAT_ACUCARADOS,
    CAT_MISCELANEAS,
    CAT_OUTROS_IND,
    CAT_PREPARADOS,
    CAT_LEGUMINOSAS,
    CAT_NOZES_SEMENTES
} CategoriaAlimento;

/* Um alimento do jeito que é gravado no dados.bin */
typedef struct {
    int numero;
    char descricao[TAMANHO_DESCRICAO];
    /* Energia em kcal e proteína em gramas; -1 significa que o valor não está disponível */
    float energia;
    float proteina;
    CategoriaAlimento categoria;
} AlimentoRegistro;

/* Devolve o nome da categoria a partir do enum */
const char* obterNomeCategoria(CategoriaAlimento cat);

/* Descobre o enum a partir do texto do CSV (devolve 0 se não achar) */
CategoriaAlimento identificarCategoria(const char *nome);

/* Compara dois textos em ordem alfabética, ignorando acento e maiúscula */
int compararTextoAlfabetico(const char *textoA, const char *textoB);

#endif
