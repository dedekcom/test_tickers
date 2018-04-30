# !/bin/bash

cd ..
./badacz scripts/akcje_balony.dsc | tac | sed 's/\ /\,/g' > input_invest2/balony.csv

