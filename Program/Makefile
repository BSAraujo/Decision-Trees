all : dectree

CCC = g++
CCFLAGS = -O3 
LIBS= -lm
TARGETDIR=.
CPPFLAGS += \
		-std=c++11

OBJS2 = \
        $(TARGETDIR)/main.o \
        $(TARGETDIR)/Params.o \
        $(TARGETDIR)/Greedy.o \
		$(TARGETDIR)/GreedyDecoder.o \
		$(TARGETDIR)/LocalSearch.o \
		$(TARGETDIR)/GeneticAlgorithm.o 

$(TARGETDIR)/dectree: $(OBJS2)
	$(CCC)  $(CCFLAGS) -o $(TARGETDIR)/dectree $(OBJS2)
	
$(TARGETDIR)/Params.o: Params.h Params.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c Params.cpp -o $(TARGETDIR)/Params.o

$(TARGETDIR)/Greedy.o: Greedy.h Greedy.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c Greedy.cpp -o $(TARGETDIR)/Greedy.o

$(TARGETDIR)/GreedyDecoder.o: GreedyDecoder.h GreedyDecoder.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c GreedyDecoder.cpp -o $(TARGETDIR)/GreedyDecoder.o

$(TARGETDIR)/LocalSearch.o: LocalSearch.h LocalSearch.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c LocalSearch.cpp -o $(TARGETDIR)/LocalSearch.o

$(TARGETDIR)/GeneticAlgorithm.o: GeneticAlgorithm.h GeneticAlgorithm.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c GeneticAlgorithm.cpp -o $(TARGETDIR)/GeneticAlgorithm.o

$(TARGETDIR)/main.o: main.cpp
	$(CCC) $(CCFLAGS) $(CPPFLAGS) -c main.cpp -o $(TARGETDIR)/main.o


clean:
	$(RM) \
	$(TARGETDIR)/main.o \
	$(TARGETDIR)/Params.o \
	$(TARGETDIR)/Greedy.o \
	$(TARGETDIR)/GreedyDecoder.o \
	$(TARGETDIR)/LocalSearch.o \
	$(TARGETDIR)/GeneticAlgorithm.o 