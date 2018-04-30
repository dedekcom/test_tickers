
/*
	skrypt tworzy indeks z zadanych lub wszystkich akcji
*/

%include scripts/libs/gpw.dsc

# config start

$dir = ["PROTEKTOR", "UNIMA", "PROJPRZEM", "NOWAGALA", "DECORA"]

# $dir = []; # all

@checkdir;

# $depth = 9000;
$depth = 5000;
$startdate = 0;
# config end 

$pom = $startdate + 1 - $depth;
if $depth < 1 then
	print glebokosc badan musi byc minimum 1;
	exit
fi

$i = 0;
$indeks = [];
$ileticker = [];
while $i < $depth do
	$indeks.addarr,0
	$ileticker.addarr,0
	$i ++
done

$s = 0;
$i = 0;
$sizeakcja = $startdate + $depth + 5;
$ileakcji2 = 0;
while $i < $ileakcji do
	$s.getstrarr,$i,$dir
	@is_akcja,$s;
	if $ret == 1 then
		$akcja.load2,d_akcjegpw/data/,$s
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
			fi
			$j ++
		done
		$ileakcji2 ++
	fi
	$i ++
done

@get_wig;
$i = 0;
while $i < $depth do
	$fdiv = $ileticker.getarr,$i;
	#$fdiv = $ileakcji2;
	if $fdiv > 0 then
		$pom = $indeks.getarr,$i / $fdiv;
		$j = $startdate + $i;
		print $wig.date,$j $pom $pom $pom $pom 0;
	else
		# print $wig.date,$j 1.0 1.0 1.0 1.0 0;
	fi
	$i ++
done

