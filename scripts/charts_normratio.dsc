
%include scripts/libs/ratio.dsc

print skrypt liczy chart1/chart2;

$first.loadstooq,stooq/chart1.csv,chart1
$sec.loadstooq,stooq/chart2.csv,chart2
$first.zerovol
$sec.zerovol
$ile = $first.size;
$first.normalize,0,$ile
$ile = $sec.size;
$sec.normalize,0,$ile

@calc_ratio $first,$sec;

$ret.savestooqc,stooq/charts_ratio.csv

