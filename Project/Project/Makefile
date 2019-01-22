#------------------------------------------------------------
#
# Please define an environnement variable PATHTUTOMIP
# equal to the XXX directory where Lemon,Graphviz and Cplex have been installed
# In order to these:
# edit ~/.bashrc add "export PATHTUTOMIP=XXX"
# close your xterm and star new ones
#
#------------------------------------------------------------

CPLEXDIR      = $(PATHTUTOMIP)/cplex-12.7.1/cplex
CONCERTDIR    = $(PATHTUTOMIP)/cplex-12.7.1/concert


SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic


# ---------------------------------------------------------------------
# Compiler selection 
# ---------------------------------------------------------------------

CCC = g++ -O0


# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------

CCOPT = -m64 -O -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD

# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

CCLNDIRS  = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR)


CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include

CCLNFLAGS = -lconcert -lilocplex -lcplex -lm -lpthread

CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR)




# ---------------------------------------------------------------------
# Your part
# ---------------------------------------------------------------------



all: Test

Graph.o: Graph.cpp Graph.h
	$(CCC) -c -w $(CCFLAGS) $(CCLNDIRS) -c Graph.cpp -o Graph.o

TSP.o: Graph.o EvaluateTour.o TSP.h TSP.cpp
	$(CCC) -c -w $(CCFLAGS) $(CCLNDIRS) -c TSP.cpp Graph.o EvaluateTour.o -o TSP.o

BP.o: Graph.o BP.h BP.cpp defines.h
	$(CCC) -c -w  $(CCFLAGS) $(CCLNDIRS) -c BP.cpp Graph.o -o BP.o

EvaluateTour.o: Graph.o EvaluateTour.h EvaluateTour.cpp defines.h
	$(CCC) -c -w $(CCFLAGS) $(CCLNDIRS) -c EvaluateTour.cpp Graph.o -o EvaluateTour.o

SA.o: Graph.o EvaluateTour.o BP.o SA.cpp SA.h defines.h
	$(CCC) -c -w $(CCFLAGS) $(CCLNDIRS) -c SA.cpp Graph.o EvaluateTour.o BP.o -o SA.o

MTZ.o: Graph.o MTZ.cpp
	$(CCC) -c -w $(CCFLAGS) $(CCLNDIRS) MTZ.cpp Graph.o -o MTZ.o

Csol.o: Graph.o Csol.cpp
	$(CCC) -c -w $(CCFLAGS) $(CCLNDIRS) Csol.cpp Graph.o -o Csol.o

Test: TSP.o BP.o EvaluateTour.o SA.o MTZ.o Main.cpp
	$(CCC) $(CCFLAGS) $(CCLNDIRS) Graph.o TSP.o BP.o EvaluateTour.o SA.o MTZ.o Main.cpp -o Test $(CCLNFLAGS)

#Test: TSP.o BP.o EvaluateTour.o SA.o Csol.o Main.cpp
#	$(CCC) $(CCFLAGS) $(CCLNDIRS) Graph.o TSP.o BP.o EvaluateTour.o SA.o Csol.o Main.cpp -o Test $(CCLNFLAGS)

clean:
	rm -f *.o Test

#all:
#	g++ -std=c++11 -w *.cpp -o Test
#clean:
#	rm -f *.o Test
