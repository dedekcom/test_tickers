# !/bin/bash

wget http://bossa.pl/pub/newconnect/mstock/mstncn.zip
plik="mstncn.zip"
if [ -e "$plik" ]; then
	rm data/*
	rm "download/$plik"
	unzip "$plik"
	mv *.mst data/
	mv "$plik" download/
fi
