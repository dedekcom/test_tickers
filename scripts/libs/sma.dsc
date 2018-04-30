
/*
	biblioteka z funkcjami badajacymi nachylenia srednich i odleglosci od srednich
*/

# sprawdza czy kurs przykleil sie do sma (l <= sma <= h)
# $1 - chart, $2 - sma
func @insma
	local s

	$s = $1.sma,0,$2;
	if $1.getl,0 <= $s & $1.geth,0 >= $s then
		$ret = 1;
		#print $1.ticker $1.date,0 in sma,$2 - avg= $s - low = $1.getl,0 - high = $1.geth,0;
	else
		$ret = 0;
	fi
end

func @insmaprint
	local s

	$s = $1.sma,0,$2;
	if $1.getl,0 <= $s & $1.geth,0 >= $s then
		print $1.ticker $1.date,0 in sma,$2;
	fi
end

# zwraca (c-sma)/c
# $1 chart, $2 sma
func @closetosma
	local s
	$s = $1.getc,0 - $1.sma,0,$2;
	$s = $s / $1.getc,0;
	$ret = $s.abs;
end
