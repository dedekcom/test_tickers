# !/bin/bash

if [ $# -gt 0 ]; then
	echo "Skrypt bada sile sektorow gpw. Nalezy pobrac dane ze stooq i przekopiowac do d_stooq/sektory/"
        echo "usage: $0"
        exit 0
fi

cd ..
./badacz scripts/sektory_sila.dsc | sort -n

