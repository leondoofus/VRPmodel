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
LEMONDIR      = $(PATHTUTOMIP)/lemon-1.3.1


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
LEMONLIBDIR	= $(LEMONDIR)/lib

CCLNDIRS  = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR) -L$(LEMONLIBDIR)


CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include
LEMONINCDIR   = $(LEMONDIR)/include

CCLNFLAGS = -lconcert -lilocplex -lcplex -lemon -lm -lpthread

CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) -I$(LEMONINCDIR)




# ---------------------------------------------------------------------
# Your part
# ---------------------------------------------------------------------



all: Test

Graph.o: Graph.cpp Graph.h
	$(CCC) -c -w $(CCFLAGS) $(CCLNDIRS) -c Graph.cpp -o Graph.o

TSP.o: Graph.o TSP.h TSP.cpp
	$(CCC) -c -w $(CCFLAGS) $(CCLNDIRS) -c TSP.cpp Graph.o -o TSP.o

BP.o: Graph.o BP.h BP.cpp
	$(CCC) -c -w  $(CCFLAGS) $(CCLNDIRS) -c BP.cpp Graph.o -o BP.o

EvaluateTour.o: Graph.o EvaluateTour.h EvaluateTour.cpp
	$(CCC) -c -w $(CCFLAGS) $(CCLNDIRS) -c EvaluateTour.cpp Graph.o -o EvaluateTour.o

#SA.o: Graph.o EvaluateTour.o BP.o SA.cpp SA.h
#	$(CCC) -c -w $(CCFLAGS) $(CCLNDIRS) -c SA.cpp Graph.o EvaluateTour.o BP.o -o SA.o

MTZ.o: Graph.o MTZ.cpp
	$(CCC) -c -w $(CCFLAGS) $(CCLNDIRS) MTZ.cpp Graph.o -o MTZ.o

Test: TSP.o BP.o EvaluateTour.o MTZ.o Main.cpp
	$(CCC) $(CCFLAGS) $(CCLNDIRS) Graph.o TSP.o BP.o EvaluateTour.o MTZ.o Main.cpp -o Test $(CCLNFLAGS)

clean:
	rm -f *.o Test

#all:
#	g++ -std=c++11 -w *.cpp -o Test
#clean:
#	rm -f *.o Test