
%include scripts/libs/ratio.dsc

print skrypt liczy chart1/chart2;

$first.loadstooq,input_invest2/spx_m.csv,chart1
$sec.loadstooq,input_invest2/wig20_m.csv,chart2
$first.zerovol
$sec.zerovol

@calc_ratio $first,$sec;
# @calc_mul $first,$sec;

$ret.savestooqc,input_invest2/charts_ratio.csv

