
# debug: 1
# codedebug: 0

/*
 skrypt pobiera akcje i bada czy srednie ida w tym samym kierunku
*/

%include scripts/libs/gpw.dsc

$s @ $ticker;

@is_akcja,$s;

if $ret == 1 then

	$d1 = 200;
	$d2 = 100;
	$d3 = 50;
	$d4 = 25;
	$d5 = 10;

	# print maxc is $chart.maxc,0,1 a sma is $chart.sma,0,1 get is $chart.get,2;

	if sma,0,$d1 >= sma,1,$d1 then
		if sma,0,$d2 >= sma,1,$d2 then
			if sma,0,$d3 >= sma,1,$d3 then
				if sma,0,$d4 >= sma,1,$d4 then
					if sma,0,$d5 > sma,1,$d5 then
						print $ticker wzrostowy;
					else
						print $ticker korekta we wzrostowym;
					fi
				else
	# 				print $ticker sredni wzrostowy;
				fi
			else
	# 			print $ticker sma50 spada, sma100 rosnie;
			fi
		else
	# 		print $ticker sma100 spada, sma200 rosnie;
		fi
	else
		if sma,0,$d2 < sma,1,$d2 then
			if sma,0,$d3 < sma,1,$d3 then
				if sma,0,$d4 < sma,1,$d4 then
					if sma,0,$d5 < sma,1,$d5 then
						#print $ticker spadkowy;
					else
						#print $ticker korekta w spadkowym;
					fi
				else
	# 				print $ticker sredni spadkowy;
				fi
			else
	# 			print $ticker sma50 rosnie, sma100 spada;
			fi
		else
	# 		print $ticker sma100 rosnie, sma200 spada;
		fi
	fi

fi