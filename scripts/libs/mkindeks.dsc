
/*
	skrypt tworzy indeks z przekazanych plikow
*/

# argumenty: $1-tablica z plikami $2-path $3-depth $4-startdate  $5-chartname
func @build_index
	local depth
	local startdate
	local pom
	local i
	local indeks
	local ileticker
	local s
	local sizeakcja
	local ileakcji
	local ileakcji2
	local akcja
	local j
	local norm
	local id
	local fdiv
	local spath
	local splik
	local daty
	
	$spath = $2;
	$depth = $3;
	$startdate = $4;
	$ileakcji = $1.sizearr;
	
	$pom = $startdate + 1 - $depth;
	if $depth < 1 then
		print glebokosc badan musi byc minimum 1;
		exit
	fi

	$i = 0;
	$indeks = [];
	$ileticker = [];
	$daty = [];
	
	while $i < $depth do
		$indeks.addarr,0
		$ileticker.addarr,0
		$daty.addstrarr,""
		$i ++
	done

	$s = 0;
	$i = 0;
	$sizeakcja = $startdate + $depth + 5;
	$ileakcji2 = 0;
	
	while $i < $ileakcji do
		$s.getstrarr,$i,$1
		$splik @ $spath + $s + ".csv";
		$akcja.loadstooq,$splik,$s
		$j = 0;
		$norm = 1;
		while $j < $depth do
			$id = $startdate + $j;
			if $id < $akcja.size then
				if $norm == 1 then
					$akcja.normalize,$startdate,$sizeakcja
					$norm = 0;
				fi
				$pom = $indeks.getarr,$j + $akcja.getc,$id;
				$indeks.setarr,$j,$pom
				$pom = $ileticker.getarr,$j + 1;
				$ileticker.setarr,$j,$pom
				
				$daty.setarr,$j,$akcja.getdate,$id
			fi
			$j ++
		done
		$ileakcji2 ++
		$i ++
	done
	
	$ret.setaschart,$5,$depth
	
	$i = 0;
	while $i < $depth do
		$fdiv = $ileticker.getarr,$i;
		#$fdiv = $ileakcji2;
		if $fdiv > 0 then
			$pom = $indeks.getarr,$i / $fdiv;
			
			$s.getstrarr,$i,$daty
			$ret.setshortchartrow,$s,$pom,$i
			
			/*$j = $startdate + $i;
			#print $j $pom $pom $pom $pom 0;
			$s.getstrarr,$i,$daty
			print $s $pom $pom $pom $pom 0;*/
		else
			#print $indeks.date,$j 1.0 1.0 1.0 1.0 0;
		fi
		$i ++
	done
	
	#$ret = $indeks;
	#$ret2 = $daty;
end
