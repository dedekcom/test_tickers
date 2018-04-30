
import glob
import sys


def issupported(filename):
	if ( filename.startswith('RC') or filename.startswith('WIG') or filename.startswith('KBC') or filename.startswith('DB') or
		filename.startswith('GNF') or filename.startswith('UC') or filename.startswith('BPH') or
		filename.startswith('ARKA') or filename.startswith('NCINDEX') or filename.startswith('ETF') or
		filename.startswith('INV') or filename.startswith('INTL') or filename.startswith('INTS') or
		filename.find('-PDA') != -1 or filename.find('-PP') != -1 ):
		return False
	else:
		return True


allfiles = glob.glob('../d_akcjegpw/data/*.mst')
output = open('../input_invest2/adline.csv','w')
ticker = {}

for f in allfiles:
	tmp = f.split('/')
	if issupported(tmp[-1]):
		tckfile = open(f)
		i = 0
		for line in tckfile.readlines():
			tmp = line.split(',')
			if i == 1:
				lastclose = float(tmp[5])
			elif i > 1:
				x = float(tmp[5])
				if lastclose > x:
					ticker[tmp[1]] = ticker.get(tmp[1], 0) - 1
				elif lastclose < x:
					ticker[tmp[1]] = ticker.get(tmp[1], 0) + 1
				else:
					ticker[tmp[1]] = ticker.get(tmp[1], 0)
				lastclose = x
			i += 1
		tckfile.close()

tcklist = [ [k,v] for k, v in ticker.items() ]
tcklist.sort()
tckvalue = 100000
for tck in tcklist:
	d = tck[0]
	tckvalue += tck[1]
	c = str(tckvalue)
	output.write(d[0:4]+'-'+d[4:6]+'-'+d[6:8]+','+c+','+c+','+c+','+c+'\n')
output.close()