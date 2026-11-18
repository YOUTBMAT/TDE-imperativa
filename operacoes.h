#ifndef OPERACOES_H
#define OPERACOES_H

#include "lista.h"

/* Lê o dados.bin e monta listas e árvores na memória */
NoCategoria* carregarDadosBinarios(const char *nomeArquivoBin);

/* Grava de volta o dados.bin com o que sobrou */
void salvarDadosBinarios(const char *nomeArquivoBin, NoCategoria *inicioCategorias);

/* Descarta o resto da linha digitada (usado quando a entrada é inválida) */
void limparEntrada(void);

/* Uma função para cada opção do menu */
void opListarCategorias(NoCategoria *inicio);
void opListarAlimentosCategoria(NoCategoria *inicio);
void opListarDecrescenteEnergia(NoCategoria *inicio);
void opListarDecrescenteProteina(NoCategoria *inicio);
void opListarFaixaEnergia(NoCategoria *inicio);
void opListarFaixaProteina(NoCategoria *inicio);
NoCategoria* opRemoverCategoria(NoCategoria *inicio, int *modificado);
void opRemoverAlimento(NoCategoria *inicio, int *modificado);

#endif
