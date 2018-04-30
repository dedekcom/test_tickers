# !/bin/bash

cd ..
./badacz scripts/akcje_mkloosers.dsc | tac | sed 's/\ /\,/g' > input_invest2/loosers.csv

