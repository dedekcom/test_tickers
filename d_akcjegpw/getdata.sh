# !/bin/bash

# wget http://bossa.pl/pub/ciagle/mstock/mstcgl.zip
python download.py
plik="mstcgl.zip"
if [ -e "$plik" ]; then
	rm data/*
	rm "download/$plik"
	unzip "$plik"
	mv *.mst data/
	mv "$plik" download/
fi
