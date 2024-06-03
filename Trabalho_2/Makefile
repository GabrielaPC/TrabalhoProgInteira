################################

SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic
CPLEXDIR   = /opt/ibm/ILOG/CPLEX_Studio2211/cplex
CONCERTDIR = /opt/ibm/ILOG/CPLEX_Studio2211/concert

################

# Compilador 
CCC = g++ -O0

# Opcoes de compilacao
CCOPT = -m64 -O -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD -Wno-ignored-attributes

# Bibliotecas e includes
CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

CPLEXBINDIR   = $(CPLEXDIR)/bin/$(SYSTEM)
CPLEXLIB      = cplex$(dynamic:yes=2010)

CCLNDIRS  = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR) $(dynamic:yes=-L$(CPLEXBINDIR))
CCLNFLAGS = -lconcert -lilocplex -l$(CPLEXLIB) -lm -lpthread -ldl

CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include

CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) 

all : ppi f0 f1 f2

ppi: ppi.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o ppi ppi.o $(CCLNFLAGS)

ppi.o: ppi.cpp
	$(CCC) -c $(CCFLAGS) ppi.cpp -o ppi.o

f0:  
	./ppi TPI_COL_1.txt > TPI_COL_1_saida.txt
f1:
	./ppi TPI_COL_2.txt > TPI_COL_2_saida.txt
f2:
	./ppi TPI_COL_3.txt > TPI_COL_3_saida.txt
clean:
	rm --force ppi ppi.o recourse.lp
