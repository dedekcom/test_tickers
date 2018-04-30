# !/bin/bash

wget http://bossa.pl/pub/futures/mstock/mstfut.zip
plik="mstfut.zip"
if [ -e "$plik" ]; then
	rm data/*
	rm "download/$plik"
	unzip "$plik"
	mv *.mst data/
	mv "$plik" download/
fi
