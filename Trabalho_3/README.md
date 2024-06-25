# Terceiro Trabalho de Programação Inteira
Repositório referente ao terceiro trabalho da disciplina Programação Inteira no semestre 2024.1

Aluna: Gabriela Pinheiro

## Instruções de execução


### Edição do arquivo Makefile
O código foi feito considerando a utilização de um sistema Unix, caso seja executado em outro sistema é necessário alterar a seguinte linha do [Makefile](./Makefile) para representar o sistema correto:

    SYSTEM     = x86-64_linux

Para a execução correta do código é necessário a alteração das seguintes linhas do arquivo [Makefile](./Makefile) com o endereço da instalação do CPLEX:

    CPLEXDIR   = /opt/ibm/ILOG/CPLEX_Studio2211/cplex
    CONCERTDIR = /opt/ibm/ILOG/CPLEX_Studio2211/concert

### Execução do código
A execução do código pode ser feita através do uso de comandos do [Makefile](./Makefile).

#### Executar todos os exemplos:
Compila o código e executa todos os exemplos com e sem cortes, com cada execução sendo retornada no arquivo correspondente. O modelo da última execução será retornado no arquivo [recourse.lp](./recourse.lp).
    
    make all

### Compila o PPI
Compila o código e gera o executável. 

**É necessário executar este comando ou o `make all` antes de executar os exemplos individuais.**

    make ppi

#### Executar o exemplo 1:
##### Com cortes
O resultado da execução será retornado no arquivo [TPI_BC_COL_0_saida](./TPI_BC_COL_0_saida.txt), e o modelo será retornado no arquivo [recourse.lp](./recourse.lp).

    make f0
##### Sem cortes
O resultado da execução será retornado no arquivo [TPI_BC_COL_0_nocuts_saida](./TPI_BC_COL_0_nocuts_saida.txt), e o modelo será retornado no arquivo [recourse.lp](./recourse.lp).

    make f0_nocuts

#### Executar o exemplo 2:
##### Com cortes
O resultado da execução será retornado no arquivo [TPI_BC_COL_1_saida](./TPI_BC_COL_0_saida.txt), e o modelo será retornado no arquivo [recourse.lp](./recourse.lp).

    make f1
##### Sem cortes
O resultado da execução será retornado no arquivo [TPI_BC_COL_1_nocuts_saida](./TPI_BC_COL_1_nocuts_saida.txt), e o modelo será retornado no arquivo [recourse.lp](./recourse.lp).

    make f1_nocuts

#### Executar o exemplo 3:
##### Com cortes
O resultado da execução será retornado no arquivo [TPI_BC_COL_2_saida](./TPI_BC_COL_2_saida.txt), e o modelo será retornado no arquivo [recourse.lp](./recourse.lp).

    make f2
##### Sem cortes
O resultado da execução será retornado no arquivo [TPI_BC_COL_2_nocuts_saida](./TPI_BC_COL_2_nocuts_saida.txt), e o modelo será retornado no arquivo [recourse.lp](./recourse.lp).

    make f2_nocuts

#### Executar um exemplo genérico:
É possivel executar um exemplo genérico utilizando o executável, o endereço do arquivo diretamente e a variável booleana `$CORTES` recebendo o valor `0` caso deseje executar sem cortes ou 1 caso deseje aplicar os cortes de clique Máxima.

    ./ppi $ENDERECO_ARQUIVO $CORTES

#### Apagar os arquivos
Este comando apaga os arquivo executáveis e modelo gerados. 

    make clean

## Observações sobre os resultados obtidos

Em casos com instâncias maiores (Exemplos 2 e 3), o tempo de execução é maior com o uso dos cortes, porém a quantidade de nós analisados pelo CPLEx tende a ser reduzida.

Não foi possível terminar a execução de ambas as versões para o exemplo 3 devido ao seu tamanho, porém elas chegaram ao valor mínimo.