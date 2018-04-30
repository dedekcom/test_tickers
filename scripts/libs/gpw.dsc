
/*
	biblioteka z funkcjami dla gpw
*/

# sprawdza czy ticker nie jest certyfikatem
# $1 - nazwa tickera
func @is_akcja
	if $1.isstrsub,RC == 0 & $1.isstrsub,DB == 0 & $1.isstrsub,WIG == 0 & $1.isstrsub,KBC == 0 & $1.isstrsub,GNF == 0 
		& $1.isstrsub,UC == 0 & $1.isstrsub,BPH == 0 & $1.isstrsub,ARKA == 0 & $1.isstrsub,NCINDEX == 0
		& $1.isstrsub,ETF == 0 & $1.isstrsub,INV == 0
		& $1.isstrsub,INTL == 0 & $1.isstrsub,INTS == 0
		& $1.strcontains,-PDA == 0 & $1.strcontains,-PP == 0
		then
		$ret = 1;
	else
		$ret = 0;
	fi
end

func @is_wiggpr
	if $1.isstrsub,"WIG" == 1 then
		$ret = 1;
	else
		$ret = 0;
	fi
end

func @is_sektor
	if $1.isstrsub,"f_" == 1 | $1.isstrsub,"p_" == 1 | $1.isstrsub,"u_" == 1 then
		$ret = 1;
	else
		$ret = 0;
	fi
end

func @get_wig
	$wig = 0;
	$wig.load,d_akcjegpw/data/WIG.mst
end

# jak zmienila sie wartosc tickera
# $1 ticker
# $2 id sesji (0 - ostatania)
func @ticker_zmiana
	local c1
	local c2
	local c3
	$c1 = $1.getc,$2;
	$2 ++
	$c2 = $1.getc,$2;
	$c3 = $c1 - $c2;
	$ret = $c3 / $c1 * 100.0;
end

# jesli dir pusty to laduje wszystkie akcje
func @checkdir
	local i
	$ileakcji = $dir.sizearr;
	if $ileakcji == 0 then
		$dir.loaddir,d_akcjegpw/data/
		$ileakcji = $dir.sizearr;
	else
		$i = 0;
		while $i < $ileakcji do
			$pom @ $dir.getarr,$i + ".mst";
			$dir.setarr,$i,$pom
			$i ++
		done
	fi
end
