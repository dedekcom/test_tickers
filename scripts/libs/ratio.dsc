
/*
	skrypt liczy stosunek chart1/chart2
*/

# argumenty: $1 - chart1, $2 - chart2
func @calc_ratio
	local id
	local last
	local ifrom
	local val
	
	$ifrom = $1.chartsize;
	$last = 0;
	
	while $ifrom >= 0 do
		$id = $1.getdateid,$ifrom,$2;
		if $id != -1 then
			$val = $1.getc,$ifrom / $2.getc,$id;
			$1.setc,$ifrom,$val
			$last = $val;
		else
			$1.setc,$ifrom,$last
		fi
		$ifrom = $ifrom - 1;
	done
	
	$ret = $1;
end

# argumenty: $1 - chart1, $2 - chart2
func @calc_mul
	local id
	local last
	local ifrom
	local val
	
	$ifrom = $1.chartsize;
	$last = 0;
	
	while $ifrom >= 0 do
		$id = $1.getdateid,$ifrom,$2;
		if $id != -1 then
			$val = $1.getc,$ifrom * $2.getc,$id;
			$1.setc,$ifrom,$val
			$last = $val;
		else
			$1.setc,$ifrom,$last
		fi
		$ifrom = $ifrom - 1;
	done
	
	$ret = $1;
end
