# !/bin/bash

if [ $# -gt 0 ] || [ $1 == "--help" ]; then
	echo "Usuwa dane do analizy pobrane z bossa.pl"
        echo "usage: $0"
        exit 0
fi

cd d_akcjegpw/
./czysc.sh
cd ../

cd d_futgpw/
./czysc.sh
cd ../

cd d_fx/
./czysc.sh
cd ../

cd d_grupygpw/
./czysc.sh
cd ../

cd d_indeksy/
./czysc.sh
cd ../

cd d_nc/
./czysc.sh
cd ../

echo "Usuniete!"
