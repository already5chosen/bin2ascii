CC = clang.exe -O2 -Wall
CCO3 = clang.exe -O3 -Wall
# CC = gcc -O2 -Wall
# CCO3 = gcc -O3 -Wall
CPP = g++ -O2 -Wall

all: bin2ascii_tb.exe

bin2ascii_tb.o: bin2ascii_tb.cpp
	${CPP} -c $<

zero.o: zero.c
	${CC} -c $<

bin2ascii_t.o: bin2ascii_t.c
	${CC} -c $<

bin2ascii_j.o: bin2ascii_j.c
	${CC} -c $<

bin2ascii_jm.o: bin2ascii_jm.c
	${CC} -c $<

bin2ascii_t_o3.o: bin2ascii_t.c
	${CCO3} -c -Doptimization_o3 $< -o $@

bin2ascii_1mul.o: bin2ascii_1mul.c
	${CC} -c $<

bin2ascii_1chain_base100.o: bin2ascii_1chain_base100.c
	${CC} -c $<

bin2ascii_2chains_base100.o: bin2ascii_2chains_base100.c
	${CC} -c $<

bin2ascii_3chains_base100.o: bin2ascii_3chains_base100.c
	${CC} -c $<

bin2ascii_halfnaive_base10.o: bin2ascii_halfnaive_base10.c
	${CC} -c $<

bin2ascii_halfnaive_base10_ts.o: bin2ascii_halfnaive_base10_ts.c
	${CC} -c $<

bin2ascii_halfnaive_base100.o: bin2ascii_halfnaive_base100.c
	${CC} -c $<

bin2ascii_halfnaive_base100_ts.o: bin2ascii_halfnaive_base100_ts.c
	${CC} -c $<

bin2ascii_halfnaive_base1000.o: bin2ascii_halfnaive_base1000.c
	${CC} -c $<

bin2ascii_thirdnaive_base100.o: bin2ascii_thirdnaive_base100.c
	${CC} -c $<

bin2ascii_thirdnaive_base100_ts.o: bin2ascii_thirdnaive_base100_ts.c
	${CC} -c $<

bin2ascii_terjealike_base100.o : bin2ascii_terjealike_base100.c
	${CC} -c $<

bin2ascii_terjealike_base100_7_32.o : bin2ascii_terjealike_base100_7_32.c
	${CC} -c $<

bin2ascii_terjealike3_base100.o : bin2ascii_terjealike3_base100.c
	${CC} -c $<

bin2ascii_terjealike_base100_icc.o : bin2ascii_terjealike_base100_icc.c
	${CC} -c $<

OBJECTS :=
OBJECTS += bin2ascii_tb.o
OBJECTS += zero.o
OBJECTS += bin2ascii_t.o
OBJECTS += bin2ascii_t_o3.o
OBJECTS += bin2ascii_j.o
OBJECTS += bin2ascii_jm.o
OBJECTS += bin2ascii_1mul.o
OBJECTS += bin2ascii_1chain_base100.o
OBJECTS += bin2ascii_2chains_base100.o
OBJECTS += bin2ascii_3chains_base100.o
OBJECTS += bin2ascii_halfnaive_base10.o
OBJECTS += bin2ascii_halfnaive_base10_ts.o
OBJECTS += bin2ascii_halfnaive_base100.o
OBJECTS += bin2ascii_halfnaive_base100_ts.o
OBJECTS += bin2ascii_halfnaive_base1000.o
OBJECTS += bin2ascii_thirdnaive_base100.o
OBJECTS += bin2ascii_thirdnaive_base100_ts.o
OBJECTS += bin2ascii_terjealike_base100.o
OBJECTS += bin2ascii_terjealike_base100_icc.o
OBJECTS += bin2ascii_terjealike_base100_7_32.o
OBJECTS += bin2ascii_terjealike3_base100.o

bin2ascii_tb.exe: ${OBJECTS}
	${CPP} $+ -o $@
