
print oblicz eem/dm ratio;

$first.loadstooq,stooq/eem_us_w.csv,eem
$sec.loadstooq,stooq/mxwo_w.csv,msciw
$first.zerovol
$sec.zerovol

$ratio = $first;
$ratio.setdefault

$ifrom = $ratio.chartsize ;

while $ifrom >= 0 do
	$id = $ratio.getdateid,$ifrom,$sec ;
	if $id != -1 then
		$val = $ratio.getc,$ifrom / $sec.getc,$id ;
		$ratio.setc,$ifrom,$val
	fi
	$ifrom = $ifrom - 1;
done

$ratio.savestooqc,input_invest2/eem_msci_ratio.csv