#include <string.h>
#include "alimento.h"

/* Traduz o enum para o nome da categoria */
const char* obterNomeCategoria(CategoriaAlimento cat) {
    const char *nome = "Desconhecida";

    if (cat == CAT_CEREAIS) {
        nome = "Cereais e derivados";
    } else if (cat == CAT_VERDURAS) {
        nome = "Verduras, hortaliças e derivados";
    } else if (cat == CAT_FRUTAS) {
        nome = "Frutas e derivados";
    } else if (cat == CAT_GORDURAS) {
        nome = "Gorduras e óleos";
    } else if (cat == CAT_PESCADOS) {
        nome = "Pescados e frutos do mar";
    } else if (cat == CAT_CARNES) {
        nome = "Carnes e derivados";
    } else if (cat == CAT_LEITE) {
        nome = "Leite e derivados";
    } else if (cat == CAT_BEBIDAS) {
        nome = "Bebidas (alcoólicas e não alcoólicas)";
    } else if (cat == CAT_OVOS) {
        nome = "Ovos e derivados";
    } else if (cat == CAT_ACUCARADOS) {
        nome = "Produtos açucarados";
    } else if (cat == CAT_MISCELANEAS) {
        nome = "Miscelâneas";
    } else if (cat == CAT_OUTROS_IND) {
        nome = "Outros alimentos industrializados";
    } else if (cat == CAT_PREPARADOS) {
        nome = "Alimentos preparados";
    } else if (cat == CAT_LEGUMINOSAS) {
        nome = "Leguminosas e derivados";
    } else if (cat == CAT_NOZES_SEMENTES) {
        nome = "Nozes e sementes";
    }

    return nome;
}

/* Faz o contrário: procura uma palavra-chave no texto e devolve o enum */
CategoriaAlimento identificarCategoria(const char *nome) {
    CategoriaAlimento cat = (CategoriaAlimento)0;

    if (strstr(nome, "Cereais") != NULL) {
        cat = CAT_CEREAIS;
    } else if (strstr(nome, "Verduras") != NULL) {
        cat = CAT_VERDURAS;
    } else if (strstr(nome, "Frutas") != NULL) {
        cat = CAT_FRUTAS;
    } else if (strstr(nome, "Gorduras") != NULL) {
        cat = CAT_GORDURAS;
    } else if (strstr(nome, "Pescados") != NULL) {
        cat = CAT_PESCADOS;
    } else if (strstr(nome, "Carnes") != NULL) {
        cat = CAT_CARNES;
    } else if (strstr(nome, "Leite") != NULL) {
        cat = CAT_LEITE;
    } else if (strstr(nome, "Bebidas") != NULL) {
        cat = CAT_BEBIDAS;
    } else if (strstr(nome, "Ovos") != NULL) {
        cat = CAT_OVOS;
    } else if (strstr(nome, "açucarados") != NULL || strstr(nome, "acucarados") != NULL) {
        cat = CAT_ACUCARADOS;
    } else if (strstr(nome, "Miscel") != NULL) {
        cat = CAT_MISCELANEAS;
    } else if (strstr(nome, "Outros") != NULL) {
        cat = CAT_OUTROS_IND;
    } else if (strstr(nome, "preparados") != NULL) {
        cat = CAT_PREPARADOS;
    } else if (strstr(nome, "Leguminosas") != NULL) {
        cat = CAT_LEGUMINOSAS;
    } else if (strstr(nome, "Nozes") != NULL) {
        cat = CAT_NOZES_SEMENTES;
    }

    return cat;
}

/* Devolve o caractere em minúsculo e sem acento (letra acentuada ocupa 2 bytes em UTF-8) */
static char normalizarCaractere(const char *texto, int *posicao) {
    static const char *const BASE_MAIUSCULAS = "aaaaaaaceeeeiiiidnoooooxouuuuyts";
    static const char *const BASE_MINUSCULAS = "aaaaaaaceeeeiiiidnooooo/ouuuuyty";
    unsigned char atual = (unsigned char)texto[*posicao];
    char normalizado = (char)atual;

    if (atual == 0xC3 && (unsigned char)texto[*posicao + 1] >= 0x80 && (unsigned char)texto[*posicao + 1] <= 0xBF) {
        int indice = (unsigned char)texto[*posicao + 1] - 0x80;
        if (indice < 32) {
            normalizado = BASE_MAIUSCULAS[indice];
        } else {
            normalizado = BASE_MINUSCULAS[indice - 32];
        }
        *posicao += 2;
    } else {
        if (atual >= 'A' && atual <= 'Z') {
            normalizado = (char)(atual + ('a' - 'A'));
        }
        if (atual != '\0') {
            *posicao += 1;
        }
    }

    return normalizado;
}

/* Compara letra por letra já normalizada: negativo se A vem antes de B, positivo se vem depois, 0 se igual */
int compararTextoAlfabetico(const char *textoA, const char *textoB) {
    int posicaoA = 0;
    int posicaoB = 0;
    int resultado = 0;
    int terminou = 0;

    while (terminou == 0) {
        char caractereA = normalizarCaractere(textoA, &posicaoA);
        char caractereB = normalizarCaractere(textoB, &posicaoB);

        if (caractereA != caractereB) {
            resultado = (int)(unsigned char)caractereA - (int)(unsigned char)caractereB;
            terminou = 1;
        } else if (caractereA == '\0') {
            terminou = 1;
        }
    }

    /* Se ficaram iguais sem os acentos, desempata pelo texto original */
    if (resultado == 0) {
        resultado = strcmp(textoA, textoB);
    }

    return resultado;
}
