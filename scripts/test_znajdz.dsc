
# print $ticker;
# debug: 1
# codedebug: 0


$smadepth1 = 23;
$smadepth2 = 12;

print $chart.ticker last date $chart.date,0 ;

$dvtdepth1 = dvt,1,0,$smadepth1 ;
$divlast1 = $dvtdepth1 / dvt,2,1,$smadepth1;

$dvtdepth2 = dvt,1,0,$smadepth2 ;
$divlast2 = $dvtdepth2 / dvt,2,1,$smadepth2;

$i = 5 ;
$depth = 1000;
$j = $i + $depth;

$ferr = 0.1;

while $i < $j do
	$pos = $i - 1;
	$divtest = $dvtdepth1 / dvt,$i,$pos,$smadepth1 ;
	if $divlast1.isindt,$divtest,$ferr == 1 then
		$divtest = $dvtdepth2 / dvt,$i,$pos,$smadepth2 ;
		if $divlast2.isindt,$divtest,$ferr == 1 then
			print podobna data: $chart.date,$i;
		fi
	fi
	$i = $i + 1;
done

