# !/bin/bash

cd ..
./badacz scripts/akcje_mkindeks.dsc | tac | sed 's/\ /\,/g' > input_invest2/caly_indeks.csv
cd input_invest2
./gpwmonthly.sh
