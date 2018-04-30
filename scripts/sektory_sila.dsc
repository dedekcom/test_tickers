
/*
	skrypt szuka najsilniejsze sektory
	jesli $dir==[] to sprawdza wszystkie, jesli wypelniony nazwami sektorow, to tylko wpisane
	uwaga - skrypt dziala na danych pobranych ze stooq
*/

%include scripts/libs/gpw.dsc

# config start
# $dir = ["ELZAB","WOJAS","PEP"];
$dir = []   # wersja do sprawdzenia wszystkich akcji
#$smas = [23,45,90,135,180]
$smas = [90,135,180,270,360]
$option = 2;	# 1 - krotki raport, 2 - dlugi raport
# config end

$path @ "d_stooq/sektory/";

$ile = $dir.sizearr;
if $ile == 0 then
	$dir.loaddir,$path
	$ile = $dir.sizearr;
else
	$i = 0;
	while $i < $ile do
		$pom @ $dir.getarr,$i + ".txt";
		$dir.setarr,$i,$pom
		$i ++
	done
fi

$indeks = 0;
$s = 0;

$i = 0;
while $i < $smas.sizearr do
	$sm[$i] = 0;
	$i ++
done

func @calc_dvt
	$ret = $indeks.dvt,1,0,$1;
end

$i = 0;
while $i < $ile do
	$s.getstrarr,$i,$dir
	#@is_akcja,$s;
	@is_sektor,$s;
	if $ret == 1 then
		$s2 @ $path + $s;
		$indeks.loadstooq,$s2,$s
		$indeks.normalize,0,$indeks.size
		
		if $smas.lastarr <= $indeks.size then
			$pom = 0;
			while $pom < $smas.sizearr do
				@calc_dvt,$smas.getarr,$pom;
				$sm[$pom] = $sm[$pom] + $ret;
				$pom ++
			done
		fi
	fi
	
	$i ++
done

$i = 0;
while $i < $smas.sizearr do
	$sm[$i] = $sm[$i] / $ile;
	$i ++
done

$i = 0;
while $i < $ile do
	$s.getstrarr,$i,$dir
	@is_sektor,$s;
	if $ret == 1 then
		#$indeks.load2,$path,$s
		$s2 @ $path + $s;
		$indeks.loadstooq,$s2,$s
		$indeks.normalize,0,$indeks.size
		
		if $smas.lastarr <= $indeks.size then
			$ardiff = [];
			$pom = 0;
			$sila = 0;
			while $pom < $smas.sizearr do
				@calc_dvt,$smas.getarr,$pom;
				if $sm[$pom] != 0 then
					$smb[$pom] = $ret - $sm[$pom] / $sm[$pom] * 100;
				else
					$smb[$pom] = 0;
				fi
				$ardiff.addarr,$smb[$pom]
				if $sm[$pom] > 0 then
					$sila = $sila + $smb[$pom];
				else
					$sila = $sila - $smb[$pom];
				fi
				$pom ++
			done
			if $option == 1 then
				print $sila ------ $s;
			else
				print $sila ------ $s ------ smas $smas ----- $ardiff.printproc;
			fi
		fi
	fi
	
	$i ++
done
