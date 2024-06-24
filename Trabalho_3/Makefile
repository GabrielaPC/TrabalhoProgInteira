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

all : ppi f0 f0_nocuts f1 f1_nocuts f2 f2_nocuts

ppi: ppi.o maxclique.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) ppi.o maxclique.o -o ppi  $(CCLNFLAGS)

ppi.o: ppi.cpp
	$(CCC) -c $(CCFLAGS) ppi.cpp -o ppi.o

maxclique.o: maxclique.cpp
	$(CCC) -c $(CCFLAGS) maxclique.cpp -o maxclique.o

f0:  
	./ppi TPI_BC_COL_0.txt > TPI_BC_COL_0_saida.txt 1
f1:
	./ppi TPI_BC_COL_1.txt > TPI_BC_COL_1_saida.txt 1
f2:
	./ppi TPI_BC_COL_2.txt > TPI_BC_COL_2_saida.txt 1

f0_nocuts:  
	./ppi TPI_BC_COL_0.txt > TPI_BC_COL_0_nocuts_saida.txt 0
f1_nocuts:
	./ppi TPI_BC_COL_1.txt > TPI_BC_COL_1_nocuts_saida.txt 0
f2_nocuts:
	./ppi TPI_BC_COL_2.txt > TPI_BC_COL_2_nocuts_saida.txt 0

clean:
	rm --force ppi ppi.o maxclique maxclique.o recourse.lp
