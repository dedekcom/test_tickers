# !/bin/bash

# wget http://bossa.pl/pub/ciagle/mstock/mstcgl.zip
python download.py
plik="mstncn.zip"
if [ -e "$plik" ]; then
	rm data/*.mst
	rm "download/$plik"
	unzip "$plik"
	mv *.mst data/
	mv "$plik" download/
fi
