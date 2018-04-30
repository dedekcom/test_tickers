# !/bin/bash

if [ $# == 0 ] || [ $1 == "--help" ]; then
	echo "Bada wszystkie tickery z katalogu"
        echo "usage: $0 <dir name> <script file>"
        exit 0
fi

echo "Badam: $1 skrypt: $2"
echo ""

cd ..
pliki=`ls ./${1}/data/*`
# echo $pliki
# exit 0
for x in $pliki ; do
	./badacz "$2" "$x"
# 	echo "./badacz.sh $2 $x"
done

echo ""
