#include <stdio.h>
#include "lista.h"
#include "operacoes.h"

/* Programa P2: carrega os dados, mostra o menu e trata a opção escolhida */
int main(int argc, char *argv[]) {
    const char *nomeArquivoBin = "dados.bin";
    NoCategoria *listaCategorias = NULL;
    int executando = 1;
    int opcao = 0;
    int leitura = 0;
    int modificado = 0;

    if (argc > 1) {
        nomeArquivoBin = argv[1];
    }

    /* Lê o dados.bin e monta as estruturas em memória */
    listaCategorias = carregarDadosBinarios(nomeArquivoBin);

    printf("\n>>> Dados carregados com sucesso em memoria! <<<\n");

    /* Repete o menu até o usuário escolher 9 */
    while (executando == 1) {
        printf("\n=======================================================\n");
        printf("       SISTEMA DE CONSULTA TACO - TRABALHO RA2         \n");
        printf("=======================================================\n");
        printf(" 1. Listar todas as categorias de alimentos\n");
        printf(" 2. Listar todos os alimentos de uma categoria (alfabetica)\n");
        printf(" 3. Listar alimentos por ordem DECRESCENTE de ENERGIA (arvore)\n");
        printf(" 4. Listar alimentos por ordem DECRESCENTE de PROTEINA (arvore)\n");
        printf(" 5. Listar alimentos por intervalo de ENERGIA (arvore)\n");
        printf(" 6. Listar alimentos por intervalo de PROTEINA (arvore)\n");
        printf(" 7. Remover uma categoria de alimentos\n");
        printf(" 8. Remover um alimento especifico\n");
        printf(" 9. Encerrar o programa (salvando alteracoes se houver)\n");
        printf("=======================================================\n");
        printf("Escolha uma opcao (1 a 9): ");

        /* Se a entrada acabar (EOF) encerra; se não for número, avisa e tenta de novo */
        leitura = scanf("%d", &opcao);
        if (leitura == EOF) {
            executando = 0;
        } else if (leitura != 1) {
            printf("\nOpcao invalida! Digite um numero de 1 a 9.\n");
            limparEntrada();
        } else {
            if (opcao == 1) {
                opListarCategorias(listaCategorias);
            } else if (opcao == 2) {
                opListarAlimentosCategoria(listaCategorias);
            } else if (opcao == 3) {
                opListarDecrescenteEnergia(listaCategorias);
            } else if (opcao == 4) {
                opListarDecrescenteProteina(listaCategorias);
            } else if (opcao == 5) {
                opListarFaixaEnergia(listaCategorias);
            } else if (opcao == 6) {
                opListarFaixaProteina(listaCategorias);
            } else if (opcao == 7) {
                listaCategorias = opRemoverCategoria(listaCategorias, &modificado);
            } else if (opcao == 8) {
                opRemoverAlimento(listaCategorias, &modificado);
            } else if (opcao == 9) {
                executando = 0;
            } else {
                printf("\nOpcao inexistente! Escolha entre 1 e 9.\n");
            }
        }
    }

    /* Só regrava o dados.bin se algo foi removido */
    if (modificado == 1) {
        printf("\nAlteracoes detectadas. Atualizando arquivo binario...\n");
        salvarDadosBinarios(nomeArquivoBin, listaCategorias);
    } else {
        printf("\nNenhuma alteracao realizada. O arquivo %s foi mantido inalterado.\n", nomeArquivoBin);
    }

    /* Devolve toda a memória alocada antes de terminar */
    liberarListaCategorias(listaCategorias);
    listaCategorias = NULL;

    printf("Memoria desalocada com sucesso. Programa finalizado!\n");
    return 0;
}
