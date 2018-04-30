

print $chart.ticker last date $chart.date,0 ;

$audusd.load,d_fx/data/AUDUSD.mst 
$cad = $chart;
$cad.setdefault

$ifrom = $cad.chartsize ;

while $ifrom >= 0 do
	$id = $cad.getdateid,$ifrom,$audusd ;
	if $id != -1 then
		$val = $cad.getc,$ifrom / $audusd.getc,$id ;
		#print $cad [ $ifrom ] = $cad.getc,$ifrom , $audusd [ $id ] = $audusd.getc,$id div = $val;
		$cad.setc,$ifrom,$val
	fi
	$ifrom = $ifrom - 1;
done

$cad.savestooqc,cadjpy_audusd.csv