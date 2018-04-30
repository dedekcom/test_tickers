
%include scripts/libs/sma.dsc
%include scripts/libs/gpw.dsc

@is_akcja,$ticker;
if $ret == 1 then

	$smas = [90,135,180]
	$err = 0.005;
	$proc = $err * 100;
	$i = 0;
	$calc = [];
	$close = [];
	while $i < $smas.sizearr do
		$sm = $smas.getarr,$i;
		if $sm <= $chart.size then
			@insma $chart,$sm;
			if $ret == 1 then
				$calc.addarr,$sm
			fi
			@closetosma $chart,$sm;
			if $ret <= $err then
				$close.addarr,$sm
			fi
		fi
		$i = $i + 1;
	done

	if $calc.sizearr > 0 | $close.sizearr > 0 then
		print $chart in smas: $calc and in $proc % of smas: $close;
		print ;
	fi

fi
