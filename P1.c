#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alimento.h"

#define TAM_LINHA 512

/* Converte o texto do CSV em número: vírgula vira ponto, Tr vira 0 e NA ou * viram -1 */
static float converterValor(const char *texto) {
    float valorConvertido = -1.0f;
    char buffer[64];
    int posicaoLeitura = 0;
    int posicaoEscrita = 0;
    int numerico = 1;

    while (texto[posicaoLeitura] == ' ') {
        posicaoLeitura++;
    }

    if (strcmp(&texto[posicaoLeitura], "Tr") == 0) {
        valorConvertido = 0.0f;
        numerico = 0;
    } else if (texto[posicaoLeitura] == '\0' || strcmp(&texto[posicaoLeitura], "NA") == 0 ||
               strcmp(&texto[posicaoLeitura], "*") == 0) {
        numerico = 0;
    }

    if (numerico == 1) {
        while (texto[posicaoLeitura] != '\0' && posicaoEscrita < 63) {
            if (texto[posicaoLeitura] == ',') {
                buffer[posicaoEscrita] = '.';
            } else {
                buffer[posicaoEscrita] = texto[posicaoLeitura];
            }
            posicaoLeitura++;
            posicaoEscrita++;
        }
        buffer[posicaoEscrita] = '\0';
        valorConvertido = (float)atof(buffer);
    }

    return valorConvertido;
}

/* Devolve o próximo campo da linha (até o ';') e avança o cursor */
static char* extrairCampo(char **cursor) {
    char *inicioCampo = *cursor;
    char *separador = NULL;

    if (inicioCampo != NULL) {
        separador = strchr(inicioCampo, ';');
        if (separador != NULL) {
            *separador = '\0';
            *cursor = separador + 1;
        } else {
            *cursor = NULL;
        }
    }

    return inicioCampo;
}

/* Programa P1: lê o alimentos.csv e grava cada alimento no dados.bin */
int main(int argc, char *argv[]) {
    const char *nomeEntrada = "alimentos.csv";
    const char *nomeSaida = "dados.bin";
    FILE *arquivoCSV = NULL;
    FILE *arquivoBin = NULL;
    char linha[TAM_LINHA];
    int totalGravados = 0;
    int primeiraLinha = 1;

    if (argc > 1) {
        nomeEntrada = argv[1];
    }
    if (argc > 2) {
        nomeSaida = argv[2];
    }

    arquivoCSV = fopen(nomeEntrada, "r");
    if (arquivoCSV == NULL) {
        printf("Erro fatal: Nao foi possivel abrir o arquivo de entrada %s\n", nomeEntrada);
        exit(1);
    }

    arquivoBin = fopen(nomeSaida, "wb");
    if (arquivoBin == NULL) {
        printf("Erro fatal: Nao foi possivel criar o arquivo binario %s\n", nomeSaida);
        fclose(arquivoCSV);
        exit(1);
    }

    while (fgets(linha, sizeof(linha), arquivoCSV) != NULL) {
        /* A primeira linha é o cabeçalho, então é ignorada */
        if (primeiraLinha == 1) {
            primeiraLinha = 0;
        } else {
            char *resto = linha;
            char *campoNumero = NULL;
            char *campoDescricao = NULL;
            char *campoEnergia = NULL;
            char *campoProteina = NULL;
            char *campoCategoria = NULL;

            /* Tira o fim de linha (\n ou \r) do final do texto */
            int tamanhoLinha = (int)strlen(linha);
            while (tamanhoLinha > 0 && (linha[tamanhoLinha - 1] == '\n' || linha[tamanhoLinha - 1] == '\r')) {
                linha[tamanhoLinha - 1] = '\0';
                tamanhoLinha--;
            }

            if (tamanhoLinha > 0) {
                /* Separa os 5 campos da linha */
                campoNumero = extrairCampo(&resto);
                campoDescricao = extrairCampo(&resto);
                campoEnergia = extrairCampo(&resto);
                campoProteina = extrairCampo(&resto);
                campoCategoria = extrairCampo(&resto);

                if (campoNumero != NULL && campoDescricao != NULL && campoEnergia != NULL &&
                    campoProteina != NULL && campoCategoria != NULL) {

                    /* Monta o registro (zerado antes) e grava no arquivo */
                    AlimentoRegistro registro;
                    memset(&registro, 0, sizeof(AlimentoRegistro));

                    registro.numero = atoi(campoNumero);
                    strncpy(registro.descricao, campoDescricao, TAMANHO_DESCRICAO - 1);
                    registro.descricao[TAMANHO_DESCRICAO - 1] = '\0';
                    registro.energia = converterValor(campoEnergia);
                    registro.proteina = converterValor(campoProteina);
                    registro.categoria = identificarCategoria(campoCategoria);

                    if (fwrite(&registro, sizeof(AlimentoRegistro), 1, arquivoBin) == 1) {
                        totalGravados++;
                    }
                }
            }
        }
    }

    fclose(arquivoCSV);
    fclose(arquivoBin);

    printf("====================================================\n");
    printf(" Programa P1 executado com sucesso!\n");
    printf(" Arquivo binario gerado: %s\n", nomeSaida);
    printf(" Total de alimentos gravados: %d\n", totalGravados);
    printf("====================================================\n");

    return 0;
}
