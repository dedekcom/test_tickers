# !/bin/bash

if [ $# -gt 0 ]; then
	echo "Skrypt bada sile indeksow swiatowych"
        echo "usage: $0"
        exit 0
fi

cd ..
./badacz scripts/eem_dm_ratio.dsc

