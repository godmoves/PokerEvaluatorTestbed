LIBS = -lrt
FLAGS = -O3 -s 
#FLAGS = -g 
BUILDDIR = build


all: ace tpt kev pse snz phk null_eval

tables: tpt_tables.dat snz_tables.dat

null_eval:
	gcc -DTESTNUL=1 $(LIBS) $(FLAGS) speed_test.c  -o$(BUILDDIR)/null_eval
null_sz:
	gcc -DTESTNUL=1 -s $(FLAGS) miniparse.c  -o$(BUILDDIR)/null_sz


ace:
	gcc -DTESTACE=1 $(LIBS) $(FLAGS) speed_test.c ace_eval/source/ace_eval_best.c -o$(BUILDDIR)/ace
ace_sz:
	gcc -DTESTACE=1 -s $(FLAGS) miniparse.c ace_eval/source/ace_eval_best.c -oace_sz

tpt: $(BUILDDIR)/tpt_tables.dat
	gcc -DTESTTPT=1 $(LIBS) $(FLAGS) speed_test.c twoplustwo/TPT_eval.c -o$(BUILDDIR)/tpt

tpt_tables.dat:
	gcc twoplustwo/generate_table.c --std=c99 perfecthash/source/fast_eval.c perfecthash/source/pokerlib.c -o$(BUILDDIR)/generate_table && $(BUILDDIR)/generate_table && mv tpt_tables.dat $(BUILDDIR)

kev:
	gcc -DTESTKEV=1 $(LIBS) $(FLAGS) speed_test.c cactuskev/source/pokerlib.c -o$(BUILDDIR)/kev

phk:
	gcc -DTESTPHK=1 $(LIBS) $(FLAGS) speed_test.c perfecthash/PHK_eval.c perfecthash/source/fast_eval.c cactuskev/source/pokerlib.c -o$(BUILDDIR)/phk

hes:
	gcc -DTESTHES=1  $(LIBS) $(FLAGS) speed_test.c showdown/HandEval.c -o$(BUILDDIR)/hes

# bhs:
# 	gcc -DTESTBHS=1  $(LIBS) $(FLAGS) speed_test.c showdown/source/HandEval.c -o$(BUILDDIR)/bhs
# bhs_sz:
# 	gcc -DTESTBHS=1 -s $(FLAGS) miniparse.c showdown/source/HandEval.c -o$(BUILDDIR)/bhs_sz

pse:
	gcc -DTESTPSE=1  $(LIBS) $(FLAGS) speed_test.c -static -Lpokersource/source/lib/.libs -Ipokersource/source/include -lpoker-eval -o$(BUILDDIR)/pse

snz: $(BUILDDIR)/snz_tables.dat
	gcc -DTESTSNZ=1  $(LIBS) $(FLAGS) speed_test.c senzee/SNZ_eval.c -o$(BUILDDIR)/snz

snz_tables.dat:
	gcc -DTESTSNZ=1  $(LIBS) $(FLAGS) senzee/build_table.c cactuskev/source/pokerlib.c -o$(BUILDDIR)/build_table && $(BUILDDIR)/build_table && mv snz_tables.dat $(BUILDDIR)

# ham: ham_table1.dat
# 	gcc -DTESTHAM=1  $(LIBS) $(FLAGS) speed_test.c hammer/source/handeval/handevaluator.c -o$(BUILDDIR)/ham

# ham_table1.dat:
# 	cp hammer/source/handeval/eqcllist ./ham_table1.dat && cp hammer/source/handeval/carddag ./ham_table2.dat


specialk/libspecialk.a:
	g++ -c specialk/SPK_eval.cpp -o  specialk/SPK_eval.o
	g++ -c specialk/source/src/FiveEval.cpp  -o specialk/FiveEval.o
	ar crf specialk/libspecialk.a specialk/FiveEval.o specialk/SPK_eval.o


spk: specialk/libspecialk.a
	g++ -DTESTSPK=1 -lstdc++  $(LIBS) $(FLAGS) -x c speed_test.c  -Lspecialk -lspecialk  -Ispecialk/source -o$(BUILDDIR)/spk


clean:
	rm $(BUILDDIR)/*
