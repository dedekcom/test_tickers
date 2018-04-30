
%include scripts/libs/sma.dsc

$s = [23,45,90,135,180]
$err = 0.005;
$proc = $err * 100;
$i = 0;
$calc = [];
$close = [];
while $i < $s.sizearr do
	$sm = $s.getarr,$i;
	@insma $chart,$sm;
	if $ret == 1 then
		$calc.addarr,$sm
	fi
	@closetosma $chart,$sm;
	if $ret <= $err then
		$close.addarr,$sm
	fi
	$i = $i + 1;
done

if $calc.sizearr > 0 | $close.sizearr > 0 then
	print $chart in smas: $calc and in $proc % of smas: $close;
	print ;
fi

