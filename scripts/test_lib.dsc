

func @testfun1
	print testfun1: $1.ticker a 2: $2 $2.name;
	$ret = 10;
end

func @testfun2
	print przed callem testfun1 fun2_arg1 = $1.ticker;
	@testfun1 $1,50.55;
	$1 = $ret;
	local dupa
	$dupa = $1;
	print po callu testfun1 fun2_arg1 = $1.name i drugi: $dupa.name;
	$ret = $1;
	print pushed fun2_arg1 and exit testfun2;
end



