
import sys
import re


if len(sys.argv) < 3:
	print 'not enough input argument'
	print 'usage: python ' + sys.argv[0] + ' <input.csv> <output.csv>'
	sys.exit(0)

inputfile = sys.argv[1]
outputfile = sys.argv[2]

lastmonth = None
savedmonth = None
x_month = None
x_open = None
x_max = None
x_min = None
x_close = None

def getmonth(line):
	return line[0].split('-')[1]

def formatline():
	return x_date+','+x_open+','+str(x_max)+','+str(x_min)+','+x_close+'\n'

try:
	datareg = re.compile("[0-9]+-[0-9]+[0-9]+")
	ticker = open(inputfile)
	tosave = open(outputfile, 'w')
	for line in ticker.readlines():
		x = line.rstrip().split(',')
		if len(x) > 0 and datareg.match(x[0]):
			if lastmonth is None:
				lastmonth = getmonth(x)
				x_open = x[1]
			# Data,Otwarcie,Najwyzszy,Najnizszy,Zamkniecie,Wolumen
			x_month = getmonth(x)
			if x_month != lastmonth:
				savedmonth = lastmonth
				lastmonth = x_month
				tosave.write(formatline())
				x_open = x[1]
				x_max = None
				x_min = None
			x_date = x[0]
			if x_max is None or x_max < float(x[2]):
				x_max = float(x[2])
			if x_min is None or x_min > float(x[3]):
				x_min = float(x[3])
			x_close = x[4]
	if x_month != savedmonth:
		tosave.write(formatline())
	ticker.close()
	tosave.close()
except Exception as e:
	print 'program failed: ' + e.message
	






