# !/bin/bash

wget http://bossa.pl/pub/forex/mstock/mstfx.zip
if [ -e mstfx.zip ]; then
	rm data/*
	rm download/mstfx.zip
	unzip mstfx.zip
	mv *.mst data/
	mv mstfx.zip download/
fi
