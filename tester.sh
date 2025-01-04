STUDENTNUMBER=20201558

make clean

make

./distvec_$STUDENTNUMBER topology.txt messages.txt changes.txt
./linkstate_$STUDENTNUMBER topology.txt messages.txt changes.txt