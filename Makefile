LIBS = -lrt -lstdc++
# FLAGS = -O2 -pg -g
FLAGS = -O3 -s -flto
BUILDDIR = build

all: ace tpt kev pse snz phk spk omp bhs ham naive

tables: tpt_tables.dat snz_tables.dat ham_table1.dat

omp:
	g++ $(LIBS) $(FLAGS) ompeval/OMP_benchmark.cpp ompeval/source/omp/HandEvaluator.cpp -o$(BUILDDIR)/omp

ace:
	g++ $(LIBS) $(FLAGS) ace_eval/ACE_benchmark.cpp -o$(BUILDDIR)/ace

bhs:
	g++ $(LIBS) $(FLAGS) showdown/BHS_benchmark.cpp showdown/source/HandEval.c -o$(BUILDDIR)/bhs

kev:
	g++ $(LIBS) $(FLAGS) cactuskev/KEV_benchmark.cpp -o$(BUILDDIR)/kev

phk:
	gcc -O3 -c perfecthash/source/fast_eval.c -operfecthash/source/fast_eval.o
	g++ $(LIBS) $(FLAGS) perfecthash/PHK_benchmark.cpp perfecthash/source/fast_eval.o -o$(BUILDDIR)/phk

spk:
	g++ -O3 -c specialk/source/src/FiveEval.cpp -o specialk/source/src/FiveEval.o
	g++ $(LIBS) $(FLAGS) specialk/SPK_benchmark.cpp specialk/source/src/FiveEval.o -o$(BUILDDIR)/spk

pse:
	g++ $(LIBS) $(FLAGS) pokersource/PSE_benchmark.cpp -static -Lpokersource/source/lib/.libs -Ipokersource/source/include -lpoker-eval -o$(BUILDDIR)/pse

ham_table1.dat:
	cp hammer/source/handeval/eqcllist ./$(BUILDDIR)/ham_table1.dat && cp hammer/source/handeval/carddag ./$(BUILDDIR)/ham_table2.dat

ham: $(BUILDDIR)/ham_table1.dat
	gcc -O3 -c hammer/source/handeval/handevaluator.c -ohammer/source/handeval/handevaluator.o
	g++ $(LIBS) $(FLAGS) hammer/HAM_benchmark.cpp hammer/source/handeval/handevaluator.o -o$(BUILDDIR)/ham

tpt_tables.dat:
	gcc -O3 twoplustwo/generate_table.c --std=c99 perfecthash/source/fast_eval.c perfecthash/source/pokerlib.c -o$(BUILDDIR)/generate_table && $(BUILDDIR)/generate_table && mv tpt_tables.dat $(BUILDDIR)

tpt: $(BUILDDIR)/tpt_tables.dat
	g++ $(LIBS) $(FLAGS) twoplustwo/TPT_benchmark.cpp -o$(BUILDDIR)/tpt

snz_tables.dat:
	gcc -O3 senzee/build_table.c cactuskev/source/pokerlib.c -o$(BUILDDIR)/build_table && $(BUILDDIR)/build_table && mv snz_tables.dat $(BUILDDIR)

snz: $(BUILDDIR)/snz_tables.dat
	g++ $(LIBS) $(FLAGS) senzee/SNZ_benchmark.cpp -o$(BUILDDIR)/snz

naive:
	g++ $(LIBS) $(FLAGS) naive_benchmark.cpp -o$(BUILDDIR)/naive

clean:
	rm -f $(BUILDDIR)/*