
%include scripts/libs/ratio.dsc

print skrypt liczy chart1/chart2;

$first.loadstooq,input_invest2/wig_w.csv,chart1
$sec.loadstooq,input_invest2/xaupln_w.csv,chart2
$first.zerovol
$sec.zerovol

@calc_ratio $first,$sec;
# @calc_mul $first,$sec;

$ret.savestooqc,input_invest2/wigxau.csv

