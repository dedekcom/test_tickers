# !/bin/bash

if [ $# -gt 0 ]; then
	echo "Skrypt bada sile instrumentow gpw"
        echo "usage: $0"
        exit 0
fi

cd ../
./badacz scripts/fx_sila.dsc | sort -n

