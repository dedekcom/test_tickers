# !/bin/bash

if [ $# == 0 ] || [ $1 == "--help" ]; then
	echo "Bada wszystko"
        echo "usage: $0 <script file>"
        exit 0
fi

./checkallakcje.sh $1

echo "akcje ok"

./checkallfut.sh $1

echo "fut ok"

./checkallfx.sh $1

echo "fx ok"

./checkallgrupygpw.sh $1

echo "gpw ok"

./checkallindeksy.sh $1

echo "indeksy ok"

./checkallnc.sh $1

echo "nc ok"
