# !/bin/bash

if [ $# == 0 ] || [ $1 == "--help" ]; then
	echo "Bada wszystkie akcje nc"
        echo "usage: $0 <script file>"
        exit 0
fi

./_checktickers.sh d_nc $1

