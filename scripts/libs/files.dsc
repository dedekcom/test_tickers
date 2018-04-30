
func @getfile_gpwfx
	local file
	$file @ "d_akcjegpw/data/" + $1 + ".mst";
	if $file.fileexist == 0 then
		$file @ "d_fx/data/" + $1 + ".mst";
	fi
	if $file.fileexist == 0 then
		$file @ "d_indeksy/data/" + $1 + ".mst";
	fi
	$ret @ $file;
end