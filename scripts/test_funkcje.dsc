
%include scripts/test_lib.dsc

$first.load,d_fx/data/AUDUSD.mst 
$sec.load,d_fx/data/AUDJPY.mst 

# $first.loadstooq,stooq/chart1.csv,chart1
# $sec.loadstooq,stooq/chart2.csv,chart2

func @testfun2b
	print poped fun2b_arg1 = $1.ticker;
	print call testfun1;
	@testfun2 $1;
	local tfun
	$tfun = $ret + 2.25;
	print poped fun2b_arg1 = $tfun and exit testfun2b;
end

# push: $first
print pushed arg call testfun2b;
@testfun2b $first;

