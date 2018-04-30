
/*
	skrypt analizuje sile walut
	nzd - brakuje do cad, wiec sie nie liczy
*/

# aud
$audcad.load,d_fx/data/AUDCAD.mst $audchf.load,d_fx/data/AUDCHF.mst $euraud.load,d_fx/data/EURAUD.mst
$gbpaud.load,d_fx/data/GBPAUD.mst $audjpy.load,d_fx/data/AUDJPY.mst $audusd.load,d_fx/data/AUDUSD.mst
# cad
$cadchf.load,d_fx/data/CADCHF.mst $eurcad.load,d_fx/data/EURCAD.mst $gbpcad.load,d_fx/data/GBPCAD.mst
$cadjpy.load,d_fx/data/CADJPY.mst $usdcad.load,d_fx/data/USDCAD.mst
# chf
$eurchf.load,d_fx/data/EURCHF.mst $gbpchf.load,d_fx/data/GBPCHF.mst $chfjpy.load,d_fx/data/CHFJPY.mst
$usdchf.load,d_fx/data/USDCHF.mst
# eur
$eurgbp.load,d_fx/data/EURGBP.mst $eurjpy.load,d_fx/data/EURJPY.mst $eurusd.load,d_fx/data/EURUSD.mst
# gbp
$gbpjpy.load,d_fx/data/GBPJPY.mst $gbpusd.load,d_fx/data/GBPUSD.mst
# jpy
$usdjpy.load,d_fx/data/USDJPY.mst

func @zerujfx
	$vaud = 0; $vcad=0; $vchf=0; $veur=0; $vgbp=0; $vjpy=0; $vusd=0;
	$ilesil = 0;
end

func @oblicz_sile
	$smad = $1;
	$aud = 0; $cad = 0; $chf = 0; $eur = 0; $gbp = 0; $jpy = 0; $usd = 0;

	if $audcad.dvt,1,0,$smad > 0 then $aud ++ else $cad ++ fi
	if $audchf.dvt,1,0,$smad > 0 then $aud ++ else $chf ++ fi
	if $euraud.dvt,1,0,$smad > 0 then $eur ++ else $aud ++ fi
	if $gbpaud.dvt,1,0,$smad > 0 then $gbp ++ else $aud ++ fi
	if $audjpy.dvt,1,0,$smad > 0 then $aud ++ else $jpy ++ fi
	if $audusd.dvt,1,0,$smad > 0 then $aud ++ else $usd ++ fi

	if $cadchf.dvt,1,0,$smad > 0 then $cad ++ else $chf ++ fi
	if $eurcad.dvt,1,0,$smad > 0 then $eur ++ else $cad ++ fi
	if $gbpcad.dvt,1,0,$smad > 0 then $gbp ++ else $cad ++ fi
	if $cadjpy.dvt,1,0,$smad > 0 then $cad ++ else $jpy ++ fi
	if $usdcad.dvt,1,0,$smad > 0 then $usd ++ else $cad ++ fi

	if $eurchf.dvt,1,0,$smad > 0 then $eur ++ else $chf ++ fi
	if $gbpchf.dvt,1,0,$smad > 0 then $gbp ++ else $chf ++ fi
	if $chfjpy.dvt,1,0,$smad > 0 then $chf ++ else $jpy ++ fi
	if $usdchf.dvt,1,0,$smad > 0 then $usd ++ else $chf ++ fi

	if $eurgbp.dvt,1,0,$smad > 0 then $eur ++ else $gbp ++ fi
	if $eurjpy.dvt,1,0,$smad > 0 then $eur ++ else $jpy ++ fi
	if $eurusd.dvt,1,0,$smad > 0 then $eur ++ else $usd ++ fi

	if $gbpjpy.dvt,1,0,$smad > 0 then $gbp ++ else $jpy ++ fi
	if $gbpusd.dvt,1,0,$smad > 0 then $gbp ++ else $usd ++ fi
	
	if $usdjpy.dvt,1,0,$smad > 0 then $usd ++ else $jpy ++ fi
	
	$vaud = $vaud + $aud; $vcad = $vcad + $cad; $vchf = $vchf + $chf; $veur = $veur + $eur;
	$vgbp = $vgbp + $gbp; $vjpy = $vjpy + $jpy; $vusd = $vusd + $usd;
	$ilesil ++
	
	if $2 == 1 then
		print sila dla $smad aud: $aud - cad: $cad - chf: $chf - eur: $eur - gbp: $gbp - jpy: $jpy - usd: $usd;
	fi
end

func @printfx
	$vpom = $vaud / $ilesil; print $vpom aud;
	$vpom = $vcad / $ilesil; print $vpom cad;
	$vpom = $vchf / $ilesil; print $vpom chf;
	$vpom = $veur / $ilesil; print $vpom eur;
	$vpom = $vgbp / $ilesil; print $vpom gbp;
	$vpom = $vjpy / $ilesil; print $vpom jpy;
	$vpom = $vusd / $ilesil; print $vpom usd;
	print ;
end

@zerujfx;
@oblicz_sile,3,1;
@oblicz_sile,5,1;
@oblicz_sile,8,1;
print avg short-term sila:;
@printfx;

@oblicz_sile,25,0;
@oblicz_sile,50,0;
@oblicz_sile,100,0;
print sila razem:;
@printfx;

@zerujfx;
@oblicz_sile,25,1;
@oblicz_sile,50,1;
@oblicz_sile,100,1;
print avg long-term sila:;
@printfx;