# !/bin/bash

cd ..
# ./badacz scripts/akcje_mkindeks.dsc | tac
./badacz scripts/akcje_mkindeks.dsc | tac | sed 's/\ /\,/g' > input_invest2/akcjeindeks.csv

