# !/bin/bash

cd ..
./badacz scripts/akcje_fundament.dsc | tac | sed 's/\ /\,/g' > input_invest2/fundament.csv

