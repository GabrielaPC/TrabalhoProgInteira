# Trabalho de Programação Inteira
Repositório referente ao primeiro trabalho da disciplina Programação Inteira no semestre 2024.1

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
Compila o código e executa todos os exemplos, com cada execução sendo retornada no arquivo correspondente. O modelo da última execução será retornado no arquivo [recourse.lp](./recourse.lp).
    
    make all

### Compila o PPI
Compila o código e gera o executável. 

**É necessário executar este comando ou o `make all` antes de executar os exemplos individuais.**

    make ppi

#### Executar o exemplo 0:
O resultado da execução será retornado no arquivo [TPI_F_0_saida](./TPI_F_0_saida.txt), e o modelo será retornado no arquivo [recourse.lp](./recourse.lp).

    make f0

#### Executar o exemplo 1:
O resultado da execução será retornado no arquivo [TPI_F_1_saida](./TPI_F_1_saida.txt), e o modelo será retornado no arquivo [recourse.lp](./recourse.lp).

    make f1

#### Executar o exemplo 2:
O resultado da execução será retornado no arquivo [TPI_F_2_saida](./TPI_F_2_saida.txt), e o modelo será retornado no arquivo [recourse.lp](./recourse.lp).

    make f2

#### Executar um exemplo genérico:
É possivel executar um exemplo genérico utilizando o executável e o endereço do arquivo diretamente .

    ./ppi $ENDERECO_ARQUIVO

#### Apagar os arquivos
Este comando apaga os arquivo executáveis e modelo gerados. 

    make clean