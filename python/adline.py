
import glob
import libtickers

filetype_output = '.csv'
path_output_dir = '../input_invest2/'

filename_adline = 'adline'
filename_gpwall = 'gpwall'
allfiles = glob.glob('../d_akcjegpw/data/*.mst')

def getpath(filename):
	return path_output_dir + filename + filetype_output

def paths_gpw(ltickers):
	return map(lambda t: '../d_akcjegpw/data/'+t+'.mst', ltickers)

print 'generate ' + filename_gpwall
print 'generate ' + filename_adline
libtickers.createIndexAndAdline(allfiles, getpath(filename_gpwall), getpath(filename_adline))

print 'tomonth adline'
libtickers.tomonth(getpath(filename_adline), getpath(filename_adline + '_m'))

print 'tomonth gpwall'
libtickers.tomonth(getpath(filename_gpwall), getpath(filename_gpwall + '_m'))


filename_ncall = 'ncall'
allfiles = glob.glob('../d_nc/data/*.mst')

print 'generate ' + filename_ncall
libtickers.createIndexAndAdline(allfiles, getpath(filename_ncall))

print 'tomonth ncall'
libtickers.tomonth(getpath(filename_ncall), getpath(filename_ncall + '_m'))

loosers = ["ATLANTAPL", "COLIAN", "ESSYSTEM", "IPOPEMA", "LIBET", "ORANGEPL", "OTMUCHOW", "PEKAO", "PROCAD", "RELPOL", "SOLAR", "TESGAS", "TRITON", "WADEX"]
print 'generate loosers: '
print loosers
libtickers.createIndexAndAdline(paths_gpw(loosers), getpath('loosers'))

print 'tomonth loosers'
libtickers.tomonth(getpath('loosers'), getpath('loosers_m'))


