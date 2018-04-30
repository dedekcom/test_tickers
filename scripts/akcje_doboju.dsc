
/*
	skrypt szuka najsilniejsze akcje
	jesli $dir==[] to sprawdza wszystkie, jesli wypelniony nazwami akcji, to tylko wpisane
	option: 1 - historia=200, 2-400, 3-600
*/

%include scripts/libs/gpw.dsc

# config start
# $dir = ["ELZAB","WOJAS","PEP"];
$dir = []   # wersja do sprawdzenia wszystkich akcji
$smas = [90,135,180,225]
$option = 1;	# 

$historia = 200 * $option;

# config end

$path @ "d_akcjegpw/data/";

$ile = $dir.sizearr;
if $ile == 0 then
	$dir.loaddir,$path
	$ile = $dir.sizearr;
else
	$i = 0;
	while $i < $ile do
		$pom @ $dir.getarr,$i + ".mst";
		$dir.setarr,$i,$pom
		$i ++
	done
fi

$indeks = 0;
$s = 0;

func @calc_dvt
	$ret = $indeks.dvt,1,0,$1;
end

$i = 0;
while $i < $ile do
	$s.getstrarr,$i,$dir
	@is_akcja,$s;
	if $ret == 1 then
		$indeks.load2,$path,$s
		#$indeks.normalize,0,$indeks.size
		$pom = $historia + $smas.lastarr;
		if $pom <= $indeks.size then
			$j = 0;
			$pom = 1;
			while $j < $smas.sizearr do
				$isma = $smas.getarr,$j;
				$is1 = $indeks.reldvt,$historia,0,$isma;
				$is2 = $indeks.reldvt,1,0,$isma;
				if $is1 < -0.1 & $is2 < 0.25 & $is2 > -0.25 then
				else
					$pom = 0;
				fi
				$j ++
			done
			if $pom == 1 then
				print $s;
			fi
		fi
	fi
	$i ++
done

