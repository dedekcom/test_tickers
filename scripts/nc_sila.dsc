
/*
	skrypt szuka najsilniejsze akcje z nc
	
*/

%include scripts/libs/gpw.dsc

# config start
$dir = [];
$dir.loaddir,d_nc/data/
$smas = [23,45,90,135,180]
# config end

$ile = $dir.sizearr;

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
	@is_akcja,$s;
	if $ret == 1 then
		$indeks.load2,d_nc/data/,$s
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
	@is_akcja,$s;
	if $ret == 1 then
		$indeks.load2,d_nc/data/,$s
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
			print $sila ------ $s ------ smas $smas ----- $ardiff.printproc;
		fi
	fi
	
	$i ++
done
