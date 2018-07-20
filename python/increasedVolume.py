
import glob
import libtickers

filetype_output = '.csv'
path_output_dir = '../input_invest2/'

filename_adline = 'adline'
filename_gpwall = 'gpwall'
allfiles = glob.glob('../d_akcjegpw/data/*.mst')

allncfiles = glob.glob('../d_nc/data/*.mst')


def findIncreasedVolume(allfiles, lastPeriod, depthOfSearch, howHigher):
	result = []
	for f in allfiles:
		linuxFile = f.replace('\\','/')
		tmp = linuxFile.split('/')
		ticker = tmp[-1]
		if libtickers.issupported(ticker):
			tckfile = open(f)
			try:	
				rows = tckfile.readlines()[1:]
				l = len(rows)
				
				if l > depthOfSearch + lastPeriod:
					rows = rows[::-1]	# reverse
					lastVolRows = list( map(lambda p: float(p.split(',')[6]) , rows[:lastPeriod]) )
					testVolRows = list( map(lambda p: float(p.split(',')[6]) , rows[lastPeriod:(lastPeriod+depthOfSearch)]) )
					
					lastAvgVol = sum(lastVolRows) / float(lastPeriod)
					testAvgVol = sum(testVolRows) / float(depthOfSearch)
					
					#if len(lastVolRows) != lastPeriod:
					#	print ('last period ' + str(len(lastVolRows)) + ' , ' + str(lastPeriod))
					#if len(testVolRows) != depthOfSearch:
					#	print ('test depthOfSearch ' + str(len(testVolRows)) +  ' , ' + str(depthOfSearch))
					
					if testAvgVol > 0:
						ratio = lastAvgVol / testAvgVol
						if ratio > howHigher:
							r = (ratio, ticker)
							#print(r[1] + ': vol is ' + str(r[0]) + ' higher')
							result.append( r )
				tckfile.close()
			except Exception as e:
				tckfile.close()
	result.sort(key = lambda tup: tup[0], reverse = True)
	for r in result:
		print(r[1] + ': vol is ' + str(r[0]) + ' higher')


lastPeriod = 10
depthOfSearch = 200
print('calc increased volume for last ' + str(lastPeriod) + ' sessions, depth = ' + str(depthOfSearch))
findIncreasedVolume(allfiles, lastPeriod, depthOfSearch, 1.5)

print('newconnect:')
findIncreasedVolume(allncfiles, lastPeriod, depthOfSearch, 1.5)
