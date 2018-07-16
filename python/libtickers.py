
import re
import sys

def issupported(filename):
	if ( filename.startswith('RC') or filename.startswith('WIG') or filename.startswith('KBC') or filename.startswith('DB') or
		filename.startswith('GNF') or filename.startswith('UC') or filename.startswith('BPH') or
		filename.startswith('ARKA') or filename.startswith('NCINDEX') or filename.startswith('ETF') or
		filename.startswith('INV') or filename.startswith('INTL') or filename.startswith('INTS') or filename.startswith('TRIGON') or
		filename.startswith('SWIG') or filename.startswith('MWIG') or filename.startswith('LM') or
		filename.startswith('PKOBL') or filename.startswith('PKOG') or filename.startswith('PKOS') or
		filename.startswith('QMUL') or filename.startswith('UNIS') or
		filename.endswith('FIZ.mst') or
		filename.find('-PDA') != -1 or filename.find('-PP') != -1 ):
		return False
	else:
		return True


def getmonth(line):
	return line[0].split('-')[1]

def tomonth(inputfile, outputfile):
	def formatline():
		return x_date+','+x_open+','+str(x_max)+','+str(x_min)+','+x_close+'\n'
	lastmonth = None
	savedmonth = None
	x_month = None
	x_open = None
	x_max = None
	x_min = None
	x_close = None
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
		print ('program failed: ' + e.message)

def createIndexAndAdline(allfiles, filename_index, filename_adline = None):
	if filename_adline != None:
		fileout_adline = open(filename_adline,'w')
	fileout_tickers = open(filename_index,'w')
	ticker = {}

	for f in allfiles:
		tmp = f.split('/')
		if issupported(tmp[-1]):
			tckfile = open(f)
			rows = tckfile.readlines()[1:]
			pricemax = float(max(rows, key=lambda p: float(p.split(',')[5])).split(',')[5])
			i = 0
			for line in rows:
				tmp = line.split(',')
				tkid = tmp[1]
				if i == 0:
					lastclose = float(tmp[5])
					ticker[tkid] = ticker.get(tkid, [1, 0, 0.0, lastclose/pricemax])
				elif i > 0:
					x = float(tmp[5])
					normalized = x / pricemax
					div = x / lastclose
					r = ticker.get(tkid, [0, 0, 0.0, 0.0])
					n = r[0] + 1
					d = r[2] + div
					norm = r[3] + normalized
					if div < 1.0:
						ticker[tkid] = [n, r[1] - 1, d, norm]
					elif div > 1.0:
						ticker[tkid] = [n, r[1] + 1, d, norm]
					else:
						ticker[tkid] = [n, r[1], d, norm]
					lastclose = x
				i += 1
			tckfile.close()

	tcklist = [ [k,v] for k, v in ticker.items() ]
	tcklist.sort()
	adlinestart = 100000
	gpwall = []
	gpwmin = 0.01
	i = 0
	for tck in tcklist:
		d = tck[0]
		r = tck[1]
		# number of tries
		n = float(r[0])
		# save adline
		if filename_adline != None:
			adlinestart += r[1]
			c = str(adlinestart)
			fileout_adline.write(d[0:4]+'-'+d[4:6]+'-'+d[6:8]+','+c+','+c+','+c+','+c+'\n')
		gpwall.append((d, r[3]/n))
		i += 1

	for tck in gpwall:
		d = tck[0]
		c = str(tck[1] + gpwmin)
		fileout_tickers.write(d[0:4]+'-'+d[4:6]+'-'+d[6:8]+','+c+','+c+','+c+','+c+'\n')
	if filename_adline != None:
		fileout_adline.close()
	fileout_tickers.close()
