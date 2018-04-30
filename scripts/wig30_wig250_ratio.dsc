
%include scripts/libs/ratio.dsc
%include scripts/libs/files.dsc

func @obliczratio
	print obliczam $1 / $2 ratio;
	
	@getfile_gpwfx,$1;
	$first.load,$ret
	$first.zerovol
	
	@getfile_gpwfx,$2;
	$sec.load,$ret
	$sec.zerovol
	
	@calc_ratio $first,$sec;
	$file @ "input_invest2/" + $1 + "_" + $2 + "_ratio.csv";
	$ret.savestooqc,$file
end


# @obliczratio,"WIG30","WIG250";
@obliczratio,"WIG20","MIS80";
# @obliczratio,"WIG20","FGOLD";
# @obliczratio,"WIG20","SP500";
