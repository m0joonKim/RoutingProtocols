CC=g++
CFLAGS = -Wall -Werror
STUDENTNUMBER = 20201558

all: distvec_$(STUDENTNUMBER) linkstate_$(STUDENTNUMBER)

distvec_$(STUDENTNUMBER): distvec_$(STUDENTNUMBER).cc
	$(CC) $(CFLAGS) -o distvec_$(STUDENTNUMBER) distvec_$(STUDENTNUMBER).cc

linkstate_20201558: linkstate_20201558.cc
	$(CC) $(CFLAGS) -o linkstate_$(STUDENTNUMBER) linkstate_$(STUDENTNUMBER).cc

clean:
	rm -f distvec_$(STUDENTNUMBER) linkstate_$(STUDENTNUMBER)
