# !/bin/bash

if [ $# -gt 0 ]; then
	echo "Program pobiera dane do analizy z bossa.pl"
        echo "usage: $0"
        exit 0
fi

cd d_akcjegpw/
./getdata.sh
cd ../

cd d_futgpw/
./getdata.sh
cd ../

cd d_fx/
./getdata.sh
cd ../

cd d_grupygpw/
./getdata.sh
cd ../

cd d_indeksy/
./getdata.sh
cd ../

cd d_nc/
./getdata.sh
cd ../

echo "Pobrane!"
