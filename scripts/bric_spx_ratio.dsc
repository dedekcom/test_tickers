
%include scripts/libs/mkindeks.dsc
%include scripts/libs/ratio.dsc

$dir = ["snx_w","shc_w","bvp_w","rts_w"];

# argumenty: $1-tablica z plikami $2-path $3-depth $4-startdate 
@build_index $dir,"input_invest2/",1500,0,"bric";

print zapisuje input_invest2/bric_index.csv;
$ret.savestooqc,input_invest2/bric_index.csv

$sec.loadstooq,input_invest2/spx_w.csv,spx
$sec.zerovol

@calc_ratio $ret,$sec;

print zapisuje input_invest2/bric_spx_ratio.csv;
$ret.savestooqc,input_invest2/bric_spx_ratio.csv