
%include scripts/libs/sma.dsc
%include scripts/libs/gpw.dsc

$sma_trend = 200;

@is_akcja,$ticker;
if $ret == 1 & $chart.size > $sma_trend & $chart.dvt,1,0,$sma_trend > 0 then

	#$smas = [45,90,135,180]
	$smas = [45]
	
	$err = 0.005;
	$proc = $err * 100;
	$i = 0;
	$calc = [];
	$close = [];
	while $i < $smas.sizearr do
		$sm = $smas.getarr,$i;

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
		print $ticker in smas: $calc and in $proc % of smas: $close;
		print ;
	fi

fi
