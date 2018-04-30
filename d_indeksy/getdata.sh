# !/bin/bash

wget http://bossa.pl/pub/indzagr/mstock/mstzgr.zip
plik="mstzgr.zip"
if [ -e "$plik" ]; then
	rm data/*
	rm "download/$plik"
	unzip "$plik"
	mv *.mst data/
	mv "$plik" download/
fi
