
/*
	skrypt liczy spolki rosnace i malejace
	opcje:
	1 - print raport
	2 - calc adline - skrypt wysyla na stdout format stooqowy - nalezy dokleic do pliku stooq/adline.csv
		start_adline: 0 - ostatni, 1 - dzien wczesniej itd.
*/

%include scripts/libs/gpw.dsc

# config start
$dir = [];
$dir.loaddir,d_akcjegpw/data
$option = 2;

# bada daty od start_adline do depth+start_adline
$depth = 213;
$start_adline = 212;	# 0 - ostatnie kwotowanie (depth musi byc 1, zeby je pobrac)
$start = 56715;		# poczatkowa wartosc adline
# config end 

if $option == 2 & $depth <= 0 then
	print glebokosc badan musi byc minimum 1;
	exit
fi

$ileakcji = $dir.sizearr;

func @status_rynku
	local akcja
	local prev
	local i
	local s
	
	$rosnie = 0;
	$spada = 0;
	$bezzmian = 0;
	
	$akcja = 0;
	$prev = $id_sesji + 1;
	
	$i = 0;
	$s = 0;
	
	while $i < $ileakcji do
		$s.getstrarr,$i,$dir
		@is_akcja,$s;
		if $ret == 1 then
			$akcja.load2,d_akcjegpw/data/,$s
			if $prev < $akcja.size then
				$c1 = $akcja.getc,$id_sesji;
				$c2 = $akcja.getc,$prev;
				if $c1 > $c2 then
					$rosnie ++
				else
					if $c1 < $c2 then
						$spada ++
					else
						$bezzmian ++
					fi
				fi
			fi
		fi
		$i ++
	done
end

@get_wig;

if $option == 1 then
	$id_sesji = 0;
	@status_rynku;

	@ticker_zmiana,$wig,0;
	print WIG $wig.date,0 : $ret ;
	print rosnace: $rosnie;
	print spadaja: $spada;
	print bez zmian: $bezzmian;
else
	while $depth > 0 do
		# $id_sesji = $depth;
		if $start_adline >= 0 then
			$id_sesji = $start_adline;
			@status_rynku;
			$start = $start + $rosnie - $spada;
			print $wig.date,$id_sesji $start $start $start $start 0;
			$depth --
			$start_adline --
		fi
	done
fi

