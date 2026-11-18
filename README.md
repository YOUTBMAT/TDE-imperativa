# Trabalho de Programacao Imperativa

Implementacao em C de um sistema para consultar alimentos por categoria, energia
e proteina, usando listas encadeadas e arvores binarias de busca.

## Requisitos

- GCC com suporte a C99

## Compilar e executar

No PowerShell, na pasta do projeto:

```powershell
gcc -Wall -Wextra -std=c99 -o P1.exe P1.c alimento.c
gcc -Wall -Wextra -std=c99 -o P2.exe P2.c operacoes.c lista.c arvore.c alimento.c
.\P1.exe
.\P2.exe
```

Execute o `P1` primeiro para gerar `dados.bin` a partir de `alimentos.csv`. O
`P2` carrega esse arquivo, apresenta o menu interativo e salva as alteracoes ao
encerrar.

## Funcionalidades

- Listagem alfabetica de categorias e alimentos.
- Consultas por categoria em ordem decrescente de energia ou proteina.
- Consultas por faixa de energia ou proteina.
- Remocao de categorias ou alimentos, com atualizacao das arvores.

O arquivo `dados.bin` e os executaveis sao gerados localmente e nao fazem parte
do repositorio; o arquivo CSV e mantido para que os dados possam ser gerados
novamente.

Para mais detalhes sobre os modulos e as opcoes do menu, consulte
[`LEIAME.txt`](LEIAME.txt).
