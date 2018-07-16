
import sys
import re
import matplotlib.pyplot as plt


if len(sys.argv) < 2:
	print ('not enough input argument')
	print ('usage: python ' + sys.argv[0] + ' <input.csv>|<input.mst>')
	sys.exit(0)

inputfile = sys.argv[1]
filetype = inputfile[-3:]

try:
	datareg = re.compile("[0-9]+-[0-9]+[0-9]+")
	tickermst = re.compile("[A-Z]+")
	ticker = open(inputfile)
	yvalues = []
	xvalues = []
	xlabels = []
	xval = 0
	if filetype == "csv":
		for line in ticker.readlines():
			x = line.rstrip().split(',')
			if len(x) > 0 and datareg.match(x[0]):
				# Data,Otwarcie,Najwyzszy,Najnizszy,Zamkniecie,Wolumen
				yvalues.append(float(x[4]))
				xvalues.append(xval)
				xlabels.append(x[0])
				xval = xval + 1
	elif filetype == "mst":
		for line in ticker.readlines():
			x = line.rstrip().split(',')
			if len(x) > 0 and tickermst.match(x[0]):
				#<TICKER>,<DTYYYYMMDD>,<OPEN>,<HIGH>,<LOW>,<CLOSE>,<VOL>
				yvalues.append(float(x[5]))
				xvalues.append(xval)
				xlabels.append(x[1])
				xval = xval + 1
	ticker.close()

	plt.plot(xvalues, yvalues, label='linear')
	plt.suptitle(inputfile)
	xview = []
	xlabelview = []
	n = 12
	step = len(xvalues) / n
	xval = 0
	for i in range(0, n):
		xview.append(xval)
		xlabelview.append(xlabels[int(xval)])
		xval = xval + step
	plt.xticks(xview, xlabelview, rotation='vertical', fontsize=5)
	plt.grid(alpha=0.5)
	plt.show()
except Exception as e:
	print ('program failed: ' + e.message)
	






