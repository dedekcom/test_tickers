# !/bin/bash

wget http://bossa.pl/pub/metastock/mstock/mstall.zip
plik="mstall.zip"
if [ -e "$plik" ]; then
	rm data/*
	rm "download/$plik"
	unzip "$plik"
	mv *.mst data/
	mv "$plik" download/
fi
