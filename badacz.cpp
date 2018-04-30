
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#define FreeVar(__x)    if(__x!=NULL){delete __x;__x=NULL;}
#define FreeVarTab(__x) if(__x!=NULL){delete[] __x;__x=NULL;}

//const int iiledata = 2048;
const double f1dive = 0.36787944117;

double getabs(double f)	{	return f > 0 ? f : -f;	}

void mperror(const char *ser, const char *msg);
void mperror(const char *ser, int imsg);
void mperror(const char *ser);
void readstring();

void setstddata(char *sd)	{
	sd[4]=sd[5]; sd[5]=sd[6]; sd[6]=sd[8]; sd[7]=sd[9]; sd[8]='\0';
}

int fileexist(const char *sfile)	{
	FILE *fin;
	fin = fopen(sfile, "r");
	if( fin==NULL ) {
		return 0;
	}
	fclose(fin);
	return 1;
}

// <TICKER>,<DTYYYYMMDD>,<OPEN>,<HIGH>,<LOW>,<CLOSE>,<VOL>
class Chart {
 public:
	char sticker[32];
	char sdata[16];
	double fkurso;
	double fkursh;
	double fkursl;
	double fkursc;
	double fvol;
	int ilekursow;
	int idata;
	Chart() {
		this->set("", 0.0, 0.0, 0.0, 0.0, 0.0);
		ilekursow = 0;
	}
	void set(const char *sd, double fo, double fh, double fl, double fc, double fv)	{
		strcpy(sdata, sd);
		idata = atoi(sdata);
		fkurso = fo;
		fkursh = fh;
		fkursl = fl;
		fkursc = fc;
		fvol = fv;
	}
	void print() {
		printf("%s %f %f %f %f %f\n", sdata, fkurso, fkursh, fkursl, fkursc, fvol);
	}
	void load(char *linia)	{		// load bossa
		float f1, f2, f3, f4, f5;
		sscanf(linia,"%s %s %f %f %f %f %f",sticker, sdata, &f1, &f2, &f3, &f4, &f5);
		fkurso = f1;
		fkursh = f2;
		fkursl = f3;
		fkursc = f4;
		fvol = f5;
		idata = atoi(sdata);
	}
	void load(char *linia, char *stick)	{	// load stooq
		float f1, f2, f3, f4, f5;
		sscanf(linia,"%s %f %f %f %f %f",sdata, &f1, &f2, &f3, &f4, &f5);
		strcpy(sticker,stick);
		setstddata(sdata);
		fkurso = f1;
		fkursh = f2;
		fkursl = f3;
		fkursc = f4;
		fvol = f5;
		idata = atoi(sdata);
	}
	void set(Chart *ch)	{
		this->set(ch->sdata,ch->fkurso,ch->fkursh,ch->fkursl,ch->fkursc,ch->fvol);
		strcpy(this->sticker,ch->sticker);
		ilekursow = ch->ilekursow;
	}
};

int igmdebug = 0;
int igmcodedebug = 0;

double getsma(Chart *ch, int istart, int depth)	{
	double fsma = 0.0;
	if( depth>0 && istart>=0 && ch!=NULL )	{
		if( ch[0].ilekursow <= depth )	{
			depth = ch[0].ilekursow-1;
		}
		double f = 0.0;
		int ile=istart+depth;
		while( istart<ile )	{
			f+=ch[istart].fkursc;
			istart++;
		}
		double fd = depth;
		fsma = f/fd;
	}
	if( igmdebug )	{
		printf("getsma(%d,%d): %f\n",istart,depth,fsma);
	}
	return fsma;
}

// start1 - wczesniejszy (liczony od tylu)
double getdvt(Chart *ch, int start1, int start2, int depth)	{
	if( start1 <= start2 || start1<0 )	{
		printf("start1 musi byc wiekszy niz start2 i wiekszy od 0\n");
		return 0.0;
	} else if( depth<1 )	{
		printf("sma musi byc > 0\n");
		return 0.0;
	}
	double f1 = getsma(ch,start1,depth);
	double f2 = getsma(ch,start2,depth);
	double fdvt = 0.0;
	if( f1>0 && f2>0 )	{
		fdvt = (f2-f1)/(double)(start1-start2);
	}
	if( igmdebug )	{
		printf("getdvt(%d,%d,%d): %e\n",start1,start2,depth,fdvt);
	}
	return fdvt;
}

double getreldvt(Chart *ch, int start1, int start2, int depth)	{
	if( start1 <= start2 || start1<0 )	{
		printf("start1 musi byc wiekszy niz start2 i wiekszy od 0\n");
		return 0.0;
	} else if( depth<1 )	{
		printf("sma musi byc > 0\n");
		return 0.0;
	}
	double f1 = getsma(ch,start1,depth);
	double f2 = getsma(ch,start2,depth);
	double fdvt = 0.0;
	if( f1>0 && f2>0 )	{
		fdvt = ((100.0*(f2-f1))/(double)(start1-start2))/ch[start2].fkursc;
	}
	if( igmdebug )	{
		printf("getreldvt(%d,%d,%d): %e\n",start1,start2,depth,fdvt);
	}
	return fdvt;
}

double getsmatr(Chart *ch, int istart, int depth, int atrdepth)	{
		double fsma = 0.0;
	if( depth>0 && istart>=0 && atrdepth<=depth)	{
		double f = 0.0;
		int ile=istart+depth;
		while( istart<ile )	{
			f+=ch[istart].fkursc;
			istart++;
		}
		fsma = f/(double)depth;
	}
	if( igmdebug )	{
		printf("getsmatr(%d,%d): %f\n",istart,depth,fsma);
	}
	return fsma;
}

double getmax(Chart *ch, int istart, int depth)	{
	if( ch!=NULL && istart>=0 && (istart+depth <= ch[0].ilekursow) )	{
		double f = 0.0;
		int ile = istart+depth;
		while( istart<ile )	{
			if( ch[istart].fkursh > f )	{
				f = ch[istart].fkursh;
			}
			istart++;
		}
		return f;
	}
	return 0.0;
}
double getmin(Chart *ch, int istart, int depth)	{
	if( ch!=NULL && istart>=0 && (istart+depth <= ch[0].ilekursow) )	{
		int ile = istart+depth;
		double f = ch[istart].fkursl;
		istart++;
		while( istart<ile )	{
			if( ch[istart].fkursl < f )	{
				f = ch[istart].fkursl;
			}
			istart++;
		}
		return f;
	}
	return 0.0;
}

double getmaxc(Chart *ch, int istart, int depth)	{
	if( ch!=NULL && istart>=0 && (istart+depth <= ch[0].ilekursow) )	{
		double f = 0.0;
		int ile = istart+depth;
		while( istart<ile )	{
			if( ch[istart].fkursc > f )	{
				f = ch[istart].fkursc;
			}
			istart++;
		}
		return f;
	}
	return 0.0;
}
double getminc(Chart *ch, int istart, int depth)	{
	if( ch!=NULL && istart>=0 && (istart+depth <= ch[0].ilekursow) )	{
		double f = ch[istart].fkursc;
		istart++;
		int ile = istart+depth;
		while( istart<ile )	{
			if( ch[istart].fkursc < f )	{
				f = ch[istart].fkursc;
			}
			istart++;
		}
		return f;
	}
	return 0.0;
}

double getatr(Chart *ch, int istart, int depth)	{
	double fatr = 0.0;
	if( depth>0 && istart>=0 && ch!=NULL )	{
		if( ch[0].ilekursow <= depth-1 )	{
			depth = ch[0].ilekursow-2;
		}
		if( depth<=0 ) return 0.0;
		double f = 0.0;
		int ile=istart+depth;
		while( istart<ile )	{
			double fh = ch[istart].fkursh - ch[istart].fkursl;
			double fh2 = getabs(ch[istart].fkursh - ch[istart-1].fkursc);
			if( fh2 > fh ) fh = fh2;
			fh2 = getabs( ch[istart].fkursl - ch[istart-1].fkursc );
			if( fh2 > fh ) fh = fh2;
			f+=fh;
			istart++;
		}
		double fd = depth;
		fatr = f/fd;
	}
	if( igmdebug )	{
		printf("getatr(%d,%d): %f\n",istart,depth,fatr);
	}
	return fatr;
}

void normalize(Chart *ch, int istart, int depth)	{
	if( istart>=0 && ch!=NULL )	{
		if( depth<=0 ) depth = ch[0].ilekursow - istart;
		if( ch[0].ilekursow <= depth-1 )	{
			depth = ch[0].ilekursow - istart;
		}
		if( depth<=0 ) return;
		/*double fmax = ch[istart].fkursc;
		int pos = istart;
		int ile=istart+depth;
		istart++;
		while( istart<ile )	{
			if( ch[istart].fkursc > fmax )
				fmax = ch[istart].fkursc;
			istart++;
		}*/
		int pos = istart;
		int ile = istart+depth;
		double fmax = getmaxc(ch,istart,depth);
		double fmin = getminc(ch,istart,depth);
 		//printf("found max=%f min=%f\n",fmax,fmin);
		if( fmax > fmin )	{
			double fdiv = 1.0/(fmax-fmin);
			while( pos<ile )	{
				ch[pos].fkursc = (ch[pos].fkursc-fmin)*fdiv + 1.0;
				pos++;
			}
		}
	}
	if( igmdebug )	{
		printf("normalize(%d,%d)\n",istart,depth);
	}
}

int finddate(Chart *ch, int idat)	{
	if( ch!=NULL )	{
		for( int i=0; i<ch[0].ilekursow; i++ )	{
			if( ch[i].idata==idat )
				return i;
		}
	}
	return -1;
}
int finddate(Chart *ch, const char *sd)	{	finddate(ch,atoi(sd));	}

//-------------------------- czytanie skryptu ------------------

class VarString {
public:
	char svar[128];
	void set(const char *c)	{	strcpy(svar,c);	}
	const char *get()	{	return svar;	}
};

class ScriptVar {
 public:
	char sname[64];
	char value[256];
	int ityp;	// 0 string, 1 int, 2 double, 3 chart, 4 double array, 5 string array
	double fvalue;
	Chart *chart;
	int ichartsize;
	double *farr;
	VarString *sarr;
	int istrarrsize;
	int iarrsize;
	void setname(const char *s)	{	strcpy(sname,s);	}
	void set(const char *s, const char *v)	{
		strcpy(sname,s);
		strcpy(value,v);
		ityp = 0;
		fvalue = this->getdouble();
		update();
	}
	void freechart()	{	FreeVarTab(chart); ichartsize=0;	}
	void freearr()		{	FreeVarTab(farr); iarrsize=0;		}
	void freestrarr()	{	FreeVarTab(sarr); istrarrsize=0;	}
	void addarr(double f)	{
		if( farr==NULL || ityp!=4 )	{
			setasarr();
			iarrsize=1;
			farr = new double[1];
			farr[0]=f;
		} else {
			double *cf = new double[iarrsize+1];
			for( int i=0; i<iarrsize; i++ )
				cf[i]=farr[i];
			cf[iarrsize] = f;
			iarrsize++;
			delete [] farr;
			farr = cf;
		}
	}
	void addstrarr(const char *c)	{
		if( sarr==NULL || ityp!=5 )	{
			setasstrarr();
			istrarrsize=1;
			sarr = new VarString[1];
			sarr[0].set(c);
		} else {
			VarString *cf = new VarString[istrarrsize+1];
			for( int i=0; i<istrarrsize; i++ )
				cf[i].set(sarr[i].get());
			cf[istrarrsize].set(c);
			istrarrsize++;
			delete [] sarr;
			sarr = cf;
		}
	}
	double getarr(int id)	{
		if( id<0 || id>= iarrsize )	{
			printf("arr size = %d, id = %d ",iarrsize,id);
			mperror("id array < 0 or >= arrsize",id);
		}
		return farr[id];
	}
	const char *getstrarr(int id)	{
		if( id<0 || id>= istrarrsize )	{
			printf("array:%s strarr size = %d, id = %d ",sname,istrarrsize,id);
			mperror("id strarray < 0 or >= strarrsize",id);
		}
		return sarr[id].get();
	}
	void setarr(int id, double f)	{
		if( id<0 || id>= iarrsize )	{
			printf("array:%s ",sname);
			mperror("id array < 0 or >= arrsize",id);
		}
		farr[id] = f;
	}
	void setstrarr(int id, const char *c)	{
		if( id<0 || id>= istrarrsize )	{
			printf("array:%s ",sname);
			mperror("id array < 0 or >= strarrsize",id);
		}
		sarr[id].set(c);
	}
	void setasarr()	{
		ityp = 4;
		freearr();
		update();
	}
	void setasstrarr()	{
		ityp = 5;
		freestrarr();
		update();
	}
	void update()	{
		if( ityp==2 )	{
			sprintf(value,"%f",fvalue);
		}
		if( ityp!=3 )
			freechart();
		if( ityp!=4 )
			freearr();
		if( ityp!=5 )
			freestrarr();
	}
	void set(double f)	{
		fvalue=f; ityp=2; update();
	}
	void set(const char *s)	{
		strcpy(value,s);
		ityp=0;	
		update();
	}
	void copychart(Chart *ch)	{
		if( ch!=NULL ) {
			freechart();
			int ile = ch[0].ilekursow;
			this->chart = new Chart[ile];
			for( int i=0; i<ile; i++ )	{
				chart[i].set(&ch[i]);
			}
			ichartsize = ile;
		}
	}
	void setaschart(int size, char *st)	{
		freechart();
		if( size>0 )	{
			ichartsize=size;
			this->chart = new Chart[size];
			for( int i=0; i<size; i++ )	{
				chart[i].ilekursow = size;
				strcpy(chart[i].sticker,st);
			}
		}
		ityp=3;
		update();
	}
	void copyarr(double *cf, int ile)	{
		if( cf!=NULL && ile>0 )	{
			freearr();
			iarrsize = ile;
			farr = new double[ile];
			for( int i=0; i<ile; i++ )	{
				farr[i] = cf[i];
			}
		}
	}
	void copystrarr(VarString *cf, int ile)	{
		if( cf!=NULL && ile>0 )	{
			freestrarr();
			istrarrsize = ile;
			sarr = new VarString[ile];
			for( int i=0; i<ile; i++ )	{
				sarr[i].set(cf[i].get());
			}
		}
	}
	void set(ScriptVar *c)	{
		this->ityp = c->ityp;
		if( ityp==0 )	{
			strcpy(value,c->value);
		} else if( ityp == 2 )	{
			this->fvalue = c->fvalue;
		} else if( ityp==3 )	{
			this->copychart(c->chart);
		} else if( ityp==4 )	{
			copyarr(c->farr,c->iarrsize);
		} else if( ityp==5 )	{
			copystrarr(c->sarr,c->istrarrsize);
		}
		
		update();
	}
	ScriptVar() {
		chart=NULL; ichartsize=0;
		farr=NULL; iarrsize=0;
		sarr=NULL; istrarrsize=0;
		this->set("",""); 
	}
	~ScriptVar() { freechart(); freearr(); freestrarr(); }
	int is(const char *s)	{	return strcmp(sname,s)==0;	}
	char *getstring()	{
		update();
		return (char*)value;		
	}
	int getint()		{	return atoi(value);		}
	double getdouble()	{	return atof(value);		}
	void getstooqdata(char *sd, const char *c)	{
		sd[0] = c[0]; sd[1] = c[1]; sd[2]=c[2]; sd[3] = c[3];
		sd[4] = '-';
		sd[5] = c[4]; sd[6] = c[5];
		sd[7] = '-';
		sd[8] = c[6]; sd[9] = c[7];
		sd[10] = '\0';
	}
	void getstooqdata(char *sd, int pos)	{
		if( chart!=NULL && pos<chart[0].ilekursow && pos>=0 )
			getstooqdata(sd,chart[pos].sdata);	
	}
	//Data,Otwarcie,Najwyzszy,Najnizszy,Zamkniecie,Wolumen,LOP
	//2005-07-11,25.2729,25.2729,24.8126,24.8126,36,31
	int savestooq(const char *sfile, bool bonlyc=false)	{
		if( chart==NULL ) {
			printf("no chart to save ");
			return 0;
		}
		FILE *fout = fopen(sfile, "w");
		if( fout==NULL ) {
			printf("can't open file %s to save ",sfile);
			return 0;
		}
		fprintf(fout,"Data,Otwarcie,Najwyzszy,Najnizszy,Zamkniecie,Wolumen,LOP\n");
		
		char sdat[16];
		for( int i=chart[0].ilekursow-1; i>=0; --i )	{
			getstooqdata(sdat,i);
			Chart *ch = &chart[i];
			if( bonlyc )
				fprintf(fout,"%s,%f,%f,%f,%f,%d,0\n",sdat, ch->fkursc,ch->fkursc,ch->fkursc,ch->fkursc,ch->fvol);
			else
				fprintf(fout,"%s,%f,%f,%f,%f,%d,0\n",sdat, ch->fkurso,ch->fkursh,ch->fkursl,ch->fkursc,ch->fvol);
		}
		
		fclose(fout);
		return 1;
	}
	void limitchart(int size)	{
		if( ityp==3 && size>0 && size < ichartsize )	{
			/*for( int i=0; i<size; ++i )	{
				
			}*/
			ichartsize = size;
		}
	}
	void printvar(int typprint=0)	{
		if( ityp==0 )	{
			printf("%s ",value);
		} else if( ityp==2 ) {
			int pom = fvalue;
			if( fvalue==pom )
				printf("%d ",pom);
			else
				printf("%f ",fvalue);
		} else if( ityp==3 )	{
			if( chart!=NULL ) printf("chart(%s) ",chart[0].sticker);
		} else if( ityp==4 )	{
			if( iarrsize>0 )	{
				printf("array:[");
				for( int i=0; i<iarrsize; i++ ) {
					if( typprint==0 )	{
						int pom = farr[i];
						if( i==iarrsize-1 )	{
							if( farr[i]==pom ) printf("%d] ",pom);
							else printf("%f] ",farr[i]);
						} else {
							if( farr[i]==pom ) printf("%d,",pom);
							else printf("%f,",farr[i]);
						}
					} else if (typprint==1 )	{	// int
						int pom = farr[i];
						if( i==iarrsize-1 )	{
							printf("%d] ",pom);
						} else {
							printf("%d,",pom);
						}
					} else if (typprint==2 )	{	// proc
						int pom = farr[i];
						if( i==iarrsize-1 )	{
							printf("%d\%] ",pom);
						} else {
							printf("%d\%,",pom);
						}
					}
				}
			} else printf("[] ");
		} else if( ityp==5 )	{
			if( istrarrsize>0 )	{
				printf("strarray:[");
				for( int i=0; i<istrarrsize; i++ ) {
					if( i==istrarrsize-1 )	{
						printf("%s] ",sarr[i].svar);
					} else {
						printf("%s, ",sarr[i].svar);
					}
				}
			} else printf("[] ");
		}
	}
	void loaddir(const char *sdir)	{
		setasstrarr();
		DIR *pdir;
		dirent *entry;
		if( pdir=opendir(sdir) )	{
			while(entry = readdir(pdir) ) {
				if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )	{
					addstrarr(entry->d_name);
				}
			}
			closedir(pdir);
		}
	}
};

int isizevars=256, iilevars=0, iidvar=0;
ScriptVar *cvars = NULL;
ScriptVar *defchart = NULL;
ScriptVar varstack[256];	// stos zmiennych lokalnych
int iidstack = 0;

int bcmptest = 0, blastcmp, logicstate;
int state = 0, laststate = 0, cmpstate;
int idifstack = 0;
int idwhilestack = 0;
long size;	// rozmiar pliku z kodem
int idcode = 0;	// pozycja w kodzie
int loops[128];	// identyfikatory petli
char skey[256];
int idvar = 0;
char *scode = NULL;

double val1 = 0, val2=0;
char sarg1[256], sarg2[256];

class ScriptFunc;

ScriptFunc *lastfunc = NULL;

class ScriptFunc {
 public:
	char sname[128];
	int idcodestart;
	int iretcode;
	ScriptFunc *fcaller;
	int iilelocals;
	int ilaststack;
	ScriptFunc()	{
		fcaller = NULL;
		idcodestart = -1;
		iretcode = -1;
		sname[0] = '\0';
		iilelocals = 0;
		ilaststack = 0;
	}
	void set(const char *s, int ids)	{
		strcpy(sname,s);
		idcodestart = ids;
	}
	void call(int istack, int ilearg)	{
		iretcode = idcode;
		idcode = idcodestart;
		fcaller = lastfunc;
		lastfunc = this;
		iilelocals = ilearg;
		ilaststack = istack;
	}
	ScriptFunc *retcaller()	{
		idcode = iretcode;
		iidstack = ilaststack;
		return fcaller;
	}
};

ScriptFunc *scfuncs = NULL;
int isizefuncs = 128;
int iilefuncs = 0;

void freevars()	{
	FreeVarTab(cvars);
	FreeVarTab(scode);
	FreeVarTab(scfuncs);
}

void mperror(const char *ser, const char *msg)	{
	printf("%s %s, state: %d\n",ser,msg,state);
	freevars();
	exit(1);
}
void mperror(const char *ser, int msg)	{
	printf("%s %d, state: %d\n",ser,msg,state);
	freevars();
	exit(1);
}
void mperror(const char *ser)	{
	mperror(ser,"");
}

void pushvar(ScriptVar *sv)	{
	varstack[iidstack].set(sv);
	iidstack++;
}
void pushvar(double f)	{
	varstack[iidstack].set(f);
	iidstack++;
}
void pushvar(char *s)	{
	varstack[iidstack].set(s);
	iidstack++;
}
ScriptVar *getlocal()	{	return &varstack[iidstack-1];	}
ScriptVar *popvar()	{
	iidstack--;
	if( iidstack>=0 ) return &varstack[iidstack];
	mperror("stack < 0!");
	return NULL;
}

int getidfunc(char *sf)	{
	for( int i=0; i<iilefuncs; i++ )
		if( strcmp(sf,scfuncs[i].sname)==0 ) return i;
	return -1;
}

void addfunc(char *sf)	{
	if( getidfunc(sf)==-1 )	{
		scfuncs[iilefuncs].set(sf,idcode);
		iilefuncs++;
	} else mperror("func already exists",sf);
}

int ischar(char c)	{ return c=='_' || (c>='a'&&c<='z') || (c>='A'&&c<='Z'); }
int misdigit(char c)	{ return (c>='0') && (c<='9'); }
int misnumber()	{
	if( misdigit(scode[idcode]) ) return 1;
	else if( scode[idcode]=='-' )	{
		int id = idcode;
		id++;
		while(id<size && scode[id]==' ') id++;
		return misdigit(scode[id]);
	} else return 0;
}
void getkeyword()	{
	int i=0;
	while(idcode<size && (ischar(scode[idcode]) || misdigit(scode[idcode])) ) {
		skey[i]=scode[idcode];
		i++;
		idcode++;
	}
	skey[i] = '\0';
}
void skip()	{
	while(scode[idcode]==' ') idcode++;
}
void skip2()	{
	while(scode[idcode]==' ' || scode[idcode]==';') idcode++;
}
ScriptVar *actlocal = NULL;
ScriptVar *getact()	{
	if( actlocal!=NULL )	return actlocal;
	else return &cvars[iidvar];
}
ScriptVar *findlocalvar(const char *svar)	{
	if( lastfunc!=NULL )	{
		for( int i=lastfunc->ilaststack; i<iidstack; i++ )	{
			if( varstack[i].is(svar) )	{
				actlocal = &varstack[i];
				return actlocal;
			}
		}
	}
	return NULL;
}
ScriptVar *findvar(const char *svar)	{
	// najpierw szuka lokalne
	ScriptVar *sc = findlocalvar(svar);
	if( sc!=NULL )	return sc;
	for( iidvar=0; iidvar<iilevars; iidvar++ )	{
		if( cvars[iidvar].is(svar) )	{
			actlocal = NULL;
			return &cvars[iidvar];
		}
	}
	return NULL;
}
void addvar(const char *svar,const char *sval)	{
	ScriptVar *v = findvar(svar);
	if( v==NULL )	{
		actlocal = NULL;
		cvars[iidvar].set(svar,sval);
		iilevars++;
	}
}
void setvar(const char *svar)	{
	if( findvar(svar)==NULL ) mperror("no such var",svar);
}
void setvar(int idlocal)	{
	if( lastfunc==NULL ) mperror("local variable outside func");
	int id = lastfunc->ilaststack + idlocal - 1;
	if( id<0 || id>=iidstack ) mperror("local outside stack");
	actlocal = &varstack[id];
}
void setlocalvar(const char *sn)	{
	if( lastfunc==NULL ) mperror("local variable outside func");
	varstack[iidstack].set(sn,"");	// defaultowo tylko nazwa
	iidstack++;
}
void getvar(int bcreate=0)	{
	if( scode[idcode]=='$' )	{
		idcode++;
		if( misdigit(scode[idcode]) )	{
			// pobierz localsa
			if( lastfunc!=NULL )	{
				int i = 0;
				while(idcode<size && misdigit(scode[idcode]))	{
					skey[i]=scode[idcode];
					i++;
					idcode++;
				}
				skey[i]='\0';
				if( scode[idcode]==' ' || scode[idcode]==';' || scode[idcode]=='.' )	{
					setvar(atoi(skey));
					skip();
				} else mperror("wrong local variable",skey);
			} else mperror("local variable outside function");
		} else {
			int i=0;
			while(idcode<size && (scode[idcode]!=' ' && scode[idcode]!=';' && scode[idcode]!='=' && scode[idcode]!='.'
				&& scode[idcode]!='[' && scode[idcode]!=']') ) {
				skey[i]=scode[idcode];
				i++;
				idcode++;
			}
			if( scode[idcode]=='[' )	{
				idcode++;
				if( scode[idcode]=='$' )	{
					char spom[128];
					skey[i] = '\0';
					strcpy(spom,skey);
					getvar();
					if( getact()->ityp==0 )	{
						strcat(spom,getact()->value);
						strcpy(skey,spom);
						i = strlen(skey);
					} else if( getact()->ityp==2 )	{
						int pom = getact()->fvalue;
						sprintf(skey,"%s%d",spom,pom);
						i = strlen(skey);
					}
				} else mperror("must be var as index");
			} else if( scode[idcode]==']' )	{
				idcode++;
			}
			if( i==0 )	mperror("var without name!");
			skey[i] = '\0';
			skip();
			if( bcreate ) addvar(skey,"");
			else setvar(skey);
		}
	} else mperror("expected var in getvar()");
}

int readfunc()	{
	if( scode[idcode]=='@' )	{
		idcode++;
		int i=0;
		while(idcode<size && (scode[idcode]!=' ' && scode[idcode]!=';') ) {
			skey[i]=scode[idcode];
			i++;
			idcode++;
		}
		if( i==0 )	mperror("function without name!");
		skey[i] = '\0';
		skip();
		return getidfunc(skey);
	} else {
		mperror("expected func in readfunc()");
		return -1;
	}
}

// typ: 0 - bossa, 1 - stooq
void loadchart(const char *sname, const char *sfile, int ityp=0, char *sticker=NULL)	{
	FILE *fin;
	addvar(sname,"");
	fin = fopen(sfile, "r");
	if( fin==NULL ) {
		mperror("can't load data file %s", sfile);
	}
	int ilelinia = 1024;
	char *linia = new char[ilelinia];
	int iiledata = 0;
	char sdata[32];
	if( ityp==0 )	{
		while( fgets(linia, ilelinia, fin)!=NULL )   {
			if( linia[0] >= 'A' && linia[0] <= 'Z' )	{
				iiledata++;
			}
		}
	} else if( ityp==1 )	{
		while( fgets(linia, ilelinia, fin)!=NULL )   {
			if( linia[0] >= '0' && linia[0] <= '9' )	{
				iiledata++;
			}
		}
	}
	if( iiledata<=0 )
		return;
	fseek(fin, 0, SEEK_SET);
	
	defchart = getact();
	defchart->freechart();
	defchart->chart = new Chart[iiledata];
	Chart *dchart = getact()->chart;
	int idchart = 0;
	
	Chart *pom = new Chart[iiledata];
	
	if( ityp==0 )	{
		while( fgets(linia, ilelinia, fin)!=NULL )   {
			if( (linia[0] >= 'A' && linia[0] <= 'Z') && idchart<iiledata )	{
				int ilen = strlen(linia);
				if( ilen>0 )	{
					for( int i=0; i<ilen; i++ )	{
						if( linia[i]==',' )
							linia[i] = ' ';
					}
					//dchart[idchart].load(linia);
					pom[idchart].load(linia);
					idchart++;
				}
			}
		}
	} else if( ityp==1 )	{
		while( fgets(linia, ilelinia, fin)!=NULL )   {
			if( (linia[0] >= '0' && linia[0] <= '9') && idchart<iiledata )	{
				int ilen = strlen(linia);
				if( ilen>0 )	{
					for( int i=0; i<ilen; i++ )	{
						if( linia[i]==',' )
							linia[i] = ' ';
					}
					pom[idchart].load(linia, sticker);
					idchart++;
				}
			}
		}
	}
	FreeVarTab(linia);
	fclose(fin);
	
	for( int i=0; i<idchart; i++ )	{
		dchart[i].set(&pom[idchart-1-i]);
		dchart[i].ilekursow = iiledata;
	}
	defchart->ichartsize = idchart;
	defchart->ityp = 3;
	FreeVarTab(pom);
}

int getnext()	{
	int i=0;
	if( scode[idcode]=='$' )	{
		getvar();
		skip();
		return 1;
	} else if( scode[idcode]=='\"' )	{
		readstring();
		skip();
		return 0;
	} else {
		while(idcode<size && scode[idcode]!=' ' ) {
			skey[i]=scode[idcode];
			i++;
			idcode++;
		}
		skey[i] = '\0';
		skip();
		return 0;
	}
}

void readstr()	{
	if( getnext() )	{
		if( getact()->ityp==0 )
			strcpy(skey,getact()->value);
		else if( getact()->ityp==2 )
			strcpy(skey,getact()->getstring());
		else mperror("cannot readstr ityp!=0 && ityp!=2",getact()->ityp);
	}
}

void readnext()	{
	int i=0;
	while(idcode<size && scode[idcode]!=' ' ) {
		skey[i]=scode[idcode];
		i++;
		idcode++;
	}
	skey[i] = '\0';
	skip();
}

void readnumber()	{
	int i=0;
	if( scode[idcode]=='-' )	{
		skey[0]='-';
		i++;
		idcode++;
		skip();
	}
	while(idcode<size && (misdigit(scode[idcode]) || scode[idcode]=='.') ) {
		skey[i]=scode[idcode];
		i++;
		idcode++;
	}
	skey[i] = '\0';
	skip();
}
void getnext2()	{
	int i=0;
	if( scode[idcode]=='$' )	{
		getvar();
	} else {
		while(idcode<size && (scode[idcode]!=' ' && scode[idcode]!=';' && scode[idcode]!='.') ) {
			skey[i]=scode[idcode];
			i++;
			idcode++;
		}
		skey[i] = '\0';
	}
	skip();
}
int mis(const char *sw)	{	return strcmp(skey,sw)==0;	}
void getcompare()	{
	int i=0;
	while( idcode<size && (scode[idcode]=='=' || scode[idcode]=='<' || scode[idcode]=='>') ) {
		skey[i]=scode[idcode];
		i++;
		idcode++;
	}
	skey[i] = '\0';
}

double getnumber()	{
	/*double number = 0;
	if( scode[idcode]=='$' )	{
		getvar();
		number = getact()->getdouble();
	} else if( misdigit(scode[idcode]) )	{
		getnext();
		number = atof(skey);
	} else mperror("expected digit in getnumber()");
	return number;*/
	readnumber();
	return atof(skey);
}

void readstring()	{
	if( scode[idcode]=='\"' )	{
		idcode++;
		int i=0;
		while(idcode<size && scode[idcode]!='\"' )	{
			skey[i] = scode[idcode];
			i++;
			idcode++;
		}
		skey[i] = '\0';
		idcode++;
	} else mperror("string expected");
}

double getarg();
void read2args(double &f1, double &f2);
void read3args(double &f1, double &f2, double &f3);

int mcalc(double f1, double f2)	{
	int odp=0;
	if( cmpstate==1 ) {if(f1<f2) odp=1;}
	else if( cmpstate==2 ) {if(f1<=f2) odp=1;}
	else if( cmpstate==3 ) {if(f1==f2) odp=1;}
	else if( cmpstate==4 ) {if(f1>f2) odp=1;}
	else if( cmpstate==5 ) {if(f1>=f2) odp=1;}
	else if( cmpstate==6 ) {if(f1!=f2) odp=1;}
	else {
		mperror("mcalc cmpstate!=1,2,3,4,5,6");
	}
	if( igmcodedebug )
		printf("cmp: %f, %f, cmpstate: %d, result=%d\n",f1,f2,cmpstate,odp);
	return odp;
}

enum GotValue {
	VAL_EMPTY,
	VAL_VALUE,
	VAL_STRING,
	VAL_VARIABLE,
	VAL_SMA,
	VAL_DVT,
	VAL_SMATR,
	VAL_FUNC,
	VAL_ARR
};
int igotvalue;
int getvalue()	{
	igotvalue = VAL_EMPTY;
	char c = scode[idcode];
	if( misnumber() )	{
		readnumber();
		igotvalue=VAL_VALUE;
		return 1;
	} else if( c=='.' )	{
		findvar("default");
		igotvalue = VAL_FUNC;
		idcode++;
		return 1;
	} else if( c=='[' )	{
		igotvalue = VAL_ARR;
		idcode++;
		return 1;
	} else if( c=='\"') {
		igotvalue = VAL_STRING;
		return 1;
	} else {
		getnext2();
		if( c=='$' )	{
			if( scode[idcode]=='.' )	{
				igotvalue = VAL_FUNC;
			} else igotvalue = VAL_VARIABLE;
			return 1;
		} else if( mis("sma") )	{
			igotvalue = VAL_SMA;
			return 1;
		} else if( mis("dvt") )	{
			igotvalue = VAL_DVT;
			return 1;
		} else if( mis("smatr") )	{
			igotvalue = VAL_SMATR;
			return 1;
		}
	}
	return 0;
}
double calcvalue(int ityp);
double getfuncresult(int ityp=0)	{
	idcode++;	// po kropce
	getnext2();	// pobieramy nazwe funkcji
	ScriptVar *objcaller = getact();
	double f1,f2,f3;
	if( strcmp(objcaller->sname,"default")==0 )
		objcaller = defchart;
	if( mis("setdefault") )	{
		defchart = objcaller;
		if( defchart->chart!=NULL )	{
			findvar("ticker")->set(defchart->chart[0].sticker);
		}
		return 0.0;
	} else if( mis("ticker") )	{
		if( ityp==1 ) {
			strcpy(skey,objcaller->chart[0].sticker);
		} else
			printf(" %s ",objcaller->chart[0].sticker);
		return 0.0;
	} else if( mis("sma") )	{
		read2args(f1,f2);
		return getsma(objcaller->chart,f1,f2);
	} else if( mis("atr") )	{
		read2args(f1,f2);
		return getatr(objcaller->chart,f1,f2);
	} else if( mis("dvt") )	{
		read3args(f1,f2,f3);
		return getdvt(objcaller->chart,f1,f2,f3);
	} else if( mis("reldvt") )	{
		read3args(f1,f2,f3);
		return getreldvt(objcaller->chart,f1,f2,f3);
	} else if( mis("max") )	{
		read2args(f1,f2);
		return getmax(objcaller->chart,f1,f2);
	} else if( mis("min") )	{
		read2args(f1,f2);
		return getmin(objcaller->chart,f1,f2);
	} else if( mis("maxc") )	{
		read2args(f1,f2);
		return getmaxc(objcaller->chart,f1,f2);
	} else if( mis("getc") )	{
		int pos = getarg();
		if( objcaller->chart!=NULL && objcaller->chart[0].ilekursow>pos )
			return objcaller->chart[pos].fkursc;
		else return 0.0;
	} else if( mis("geto") )	{
		int pos = getarg();
		if( objcaller->chart!=NULL && objcaller->chart[0].ilekursow>pos )
			return objcaller->chart[pos].fkurso;
		else return 0.0;
	} else if( mis("geth") )	{
		int pos = getarg();
		if( objcaller->chart!=NULL && objcaller->chart[0].ilekursow>pos )
			return objcaller->chart[pos].fkursh;
		else return 0.0;
	} else if( mis("getl") )	{
		int pos = getarg();
		if( objcaller->chart!=NULL && objcaller->chart[0].ilekursow>pos )
			return objcaller->chart[pos].fkursl;
		else return 0.0;
	} else if( mis("getdate") )	{	// zwraca w trybie yyyymmdd
		int pos = getarg();
		if( objcaller->chart!=NULL && objcaller->chart[0].ilekursow>pos )	{
			strcpy(skey, objcaller->chart[pos].sdata);
			return 1.0;
		} else return 0.0;
	} else if( mis("getarr") )	{
		if( ityp==1 )	{
			strcpy(skey,objcaller->getstrarr( getarg() ));
			return 1.0;
		} else
			return objcaller->getarr( getarg() );
	} else if( mis("addarr") )	{
		objcaller->addarr( getarg() );
		return 0.0;
	} else if( mis("setarr") )	{
		if( objcaller->ityp==5 )	{
			int pos = getarg();
			skip();
			if( getvalue() )	{
				calcvalue(1);
				objcaller->setstrarr(pos,skey);
			} else mperror("setarr,<id>,<string> wrong $2");
		} else if( objcaller->ityp==4 ) {
			read2args(f1,f2);
			objcaller->setarr( f1, f2 );
		} else mperror("objcaller is not array ityp=",objcaller->ityp);
		return 0.0;
	} else if( mis("sizearr") )	{
		if( objcaller->ityp==4 )
			return (double)objcaller->iarrsize;
		else if( objcaller->ityp==5 )
			return (double)objcaller->istrarrsize;
		else return 0.0;
	} else if( mis("lastarr") )	{
		if( objcaller->ityp==4 )	{
			if(objcaller->iarrsize > 0 ) return objcaller->farr[objcaller->iarrsize-1];
			else return -1.0;
		} else return 0.0;
	} else if( mis("getdateid") )	{	// szuka w tickerze id z ta sama data  ticker.getdateide(id_ticker_data,ticker2)
		int pos = getarg();	// id kursu
		skip();
		getvar();	// nazwa obiektu z chartem
		if( objcaller->chart!=NULL && objcaller->chart[0].ilekursow>pos && getact()->chart!=NULL )
			return (double)finddate(getact()->chart, objcaller->chart[pos].idata);
		else return -1.0;
	} else if( mis("finddate") )	{	// np. chart.finddate(2014-10-08)
		skip();
		readstr();
		setstddata(skey);
		return finddate(objcaller->chart, skey);
	} else if( mis("setasstooqdata") )	{	// zamienia stringa 20141008 na 2014-10-08
		if( ityp==0 )	{
			strcpy(skey,objcaller->value);
			objcaller->getstooqdata(objcaller->value,skey);
		} else if( ityp==5 )	{
			// dopisac?
		}
		return 0.0;
	} else if( mis("getstrarr") )	{	// pobiera this = $2[$1], uzycie: string.getstrarr(pos,str_array)
		int pos = getarg();	// id kursu
		skip();
		getvar();	// nazwa obiektu z string array
		if( getact()->ityp==5 && getact()->istrarrsize>0 ) {
			objcaller->set(getact()->getstrarr(pos));
			return 1.0;
		} else return 0.0;
	} else if( mis("addstrarr") )	{
		skip();
		getnext();
		objcaller->addstrarr( skey );
		return 0.0;
	} else if( mis("chartsize") )	{
		if( objcaller->chart!=NULL )
			return (double)objcaller->chart[0].ilekursow;
		else return 0.0;
	} else if( mis("setshortchartrow") )	{
		skip();
		readstr();
		char sd[16];
		strcpy(sd,skey);
		skip();
		double v = getarg();
		skip();
		int pos = getarg();
		if( objcaller->ityp==3 )	{
			objcaller->chart[pos].set(sd, v, v, v, v, 1.0);
		}
		return 0.0;
	} else if( mis("setaschart") )	{	// obj.setaschart ticker,size
		skip();
		readstr();
		char st[16];
		strcpy(st,skey);
		skip();
		int ile = getarg();
		objcaller->setaschart(ile,st);
	} else if( mis("setc") )	{
		read2args(f1,f2);
		int pos = f1;
		if( objcaller->chart!=NULL && objcaller->chart[0].ilekursow>pos )	{
			objcaller->chart[pos].fkursc = f2;
			return 1.0;
		} else return 0.0;
	} else if( mis("abs") )	{
		return objcaller->fvalue >= 0.0 ? objcaller->fvalue : -objcaller->fvalue;
	} else if( mis("date") )	{	// zwraca w trybie "yyyy-mm-dd"
		int pos = getarg();
		if( objcaller->chart!=NULL && objcaller->chart[0].ilekursow>pos )	{
			char sd[16];
			objcaller->getstooqdata(sd,pos);
			if( ityp==1 ) {
				strcpy(skey,sd);
			} else
				printf("%s ", sd);
		} else {
			if( ityp==1 )
				strcpy(skey,"");
		}
		return 0.0;
	} else if( mis("isindt") )	{
		read2args(f1,f2);
		if( ((objcaller->fvalue > 0 && f1>0) || (objcaller->fvalue<0&&f1<0) || (objcaller->fvalue==0&&f1==0)) &&
			getabs(objcaller->fvalue-f1) < f2 ) {
			//printf("%f - %f < %f\n",objcaller->fvalue,f1,f2);
			return 1.0;
		} else return 0.0;
	} else if( mis("size") )	{
		if( objcaller->chart!=NULL ) {
			if( ityp==1 ) {
				sprintf(skey,"%d",objcaller->chart[0].ilekursow);
			}
			return (double)objcaller->chart[0].ilekursow;
		} else {
			if( ityp==1 ) {
				strcpy(skey,"0");
			}
			return 0.0;
		}
	} else if( mis("print") )	{
		printf("%s->%f ",objcaller->sname, objcaller->fvalue);
		return 0.0;
	} else if( mis("name") )	{
		if( ityp==1 ) {
			strcpy(skey,objcaller->sname);
		} else
			printf("%s ",objcaller->sname);
		return 0.0;
	} else if( mis("isstr") )	{	//string.isstr(string)
		skip();
		readstr();
		if( objcaller->ityp==0 && strcmp(objcaller->value,skey)==0 ) return 1.0;
		else return 0.0;
	} else if( mis("strcontains") )	{	//string.contains(substring)
		skip();
		readstr();
		if( objcaller->ityp==0 && strstr(objcaller->value,skey)!=NULL ) return 1.0;
		else return 0.0;
	} else if( mis("isstrsub") )	{	// string.isstrb(substring)
		skip();
		readstr();
		if( objcaller->ityp==0 ) {
			int len = strlen(objcaller->value);
			int len2 = strlen(skey);
			if( len2 > len ) return 0.0;
			for( int i=0; i<len2; i++ )	{
				if( skey[i]!=objcaller->value[i] ) return 0.0;
			}
			return 1.0;	// jest substring
		} else return 0.0;
	} else if( mis("load") )	{	// chart.load(filename)
		skip();
		readstr();
		loadchart(objcaller->sname,skey);
		return 0.0;
	} else if( mis("load2") )	{	// chart.load(path, filename)
		skip();
		readstr();
		strcpy(sarg1,skey);
		skip();
		readstr();
		strcat(sarg1,skey);
		loadchart(objcaller->sname,sarg1);
		return 0.0;
	} else if( mis("loadstooq") )	{	// loadstooq,<filename>,<sticker>
		skip();
		readstr();
		strcpy(sarg1,skey);
		skip();
		readstr();
		loadchart(objcaller->sname,sarg1,1,skey);
		return 0.0;
	} else if( mis("limitchart") )	{
		skip();
		int ile = getarg();
		if( objcaller->ityp==3 )
			objcaller->limitchart(ile);
		return 0.0;
	} else if( mis("savestooq") )	{
		skip();
		readstr();
		if( objcaller->chart!=NULL )	{
			if( !objcaller->savestooq(skey) )
				mperror("unabled to save file",skey);
			return 1.0;
		} else return 0.0;
	} else if( mis("savestooqc") )	{	// zapisz l,h,o,c = c
		skip();
		readstr();
		if( objcaller->chart!=NULL )	{
			if( !objcaller->savestooq(skey,1) )
				mperror("unabled to save file",skey);
			return 1.0;
		} else return 0.0;
	} else if( mis("zerovol") )	{
		for( int i=0; i<objcaller->chart[0].ilekursow; i++ ) objcaller->chart[i].fvol=0;
		return 0.0;
	} else if( mis("normalize") )	{
		read2args(f1,f2);
		normalize(objcaller->chart,f1,f2);
		return 0.0;
	} else if( mis("printproc") )	{
		objcaller->printvar(2);
		return 0.0;
	} else if( mis("loaddir") )	{
		skip();
		readstr();
		objcaller->loaddir(skey);
	} else if( mis("fileexist") )	{
		skip();
		return (double)fileexist(objcaller->value);
	} else return 0.0;
}
double calcvalue(int ityp=0)	{
	double f1, f2, f3;
	if( igotvalue==VAL_SMA )	{
		read2args(f1,f2);
		return getsma(defchart->chart,f1,f2);
	} else if( igotvalue==VAL_DVT )	{
		read3args(f1,f2,f3);
		return getdvt(defchart->chart,f1,f2,f3);
	} else if( igotvalue==VAL_SMATR )	{
		return 0.0;
	} else if( igotvalue==VAL_VALUE )	{
		return atof(skey);
	} else if( igotvalue==VAL_VARIABLE )	{
		if( getact()->ityp==0 )	{
			strcpy(skey,getact()->value);
		}
		return getact()->fvalue;
	} else if( igotvalue == VAL_FUNC )	{
		return getfuncresult(ityp);
	} else if( igotvalue == VAL_STRING )	{
		readstring();
		return 0.0;
	} else 
		return 0.0;
}

double getarg()	{
	if( getvalue() )	{	return calcvalue();	}
	else {
		printf("igotvalue = %d ",igotvalue);
		mperror("can't read arg: ",skey);
		return 0.0;
	}
}

void read2args(double &f1, double &f2)	{
	skip();
	f1 = getarg();
	skip();
	f2 = getarg();
}
void read3args(double &f1, double &f2, double &f3)	{
	skip();
	f1 = getarg();
	skip();
	f2 = getarg();
	skip();
	f3 = getarg();
}

void fincmptest()	{
	if( logicstate==-1 ) blastcmp = bcmptest;
	else if( logicstate==0 ) blastcmp = blastcmp && bcmptest;
	else if( logicstate==1 ) blastcmp = blastcmp || bcmptest;
	if( igmcodedebug )
		printf("lastcmp: %d, logicstate: %d, bcmptest: %d, cmpstate: %d\n",blastcmp, logicstate, bcmptest,cmpstate);
	cmpstate = 0;
	state = 201;
}

void printkey(const char *msg="")	{
	printf("%s skey now is %s\n",msg,skey);
}

void clearcode(char *sc, int ilen)	{
	for( int i=0; i<ilen; i++ )	{
		if( sc[i]=='\\' )	{
			sc[i] = ' ';
			i++;
		} else if( sc[i]==',' || sc[i]=='\t' || sc[i]=='\r' || sc[i]=='\n' )	{
			sc[i] = ' ';
		} else if ( sc[i]=='#' )	{
			while(sc[i]!='\n' && i<ilen)	{
				sc[i]=' ';
				i++;
			}
			i--;
		} else if( sc[i]=='/' )	{
			i++;
			if( i<ilen && sc[i]=='*' )	{
				sc[i-1]=' ';
				sc[i] = ' ';
				bool b=1;
				while(b) {
					i++;
					if( i<ilen )	{
						if( sc[i]=='*' )	{
							sc[i]=' ';
							i++;
							if( i<ilen )	{
								if( sc[i]=='/' )	{ 
									b=0;
								}
								sc[i]=' ';
							} else b=0;
						} else {
							sc[i]=' ';
						}
					} else {
						b=0;
					}
				}
			} else i--;
		}
	}
}

void loadscript(const char *filename)	{
	FILE *fsc;
	fsc = fopen(filename, "r");
	if( fsc==NULL )	{
		mperror("no script file %s", filename);
	}
	fseek(fsc, 0, SEEK_END);
        int size2 = ftell(fsc);
        rewind(fsc);
	if( size2<=0 )   {
		fclose(fsc);
		mperror("empty code\n", filename);
	}
	char *scode2 = new char[size2+1];
        fread((void*)scode2, 1, size2, fsc);
        fclose(fsc);
	scode2[size2] = ' ';
	size2++;
	
	clearcode(scode2, size2);
	if( scode==NULL )	{
		scode = scode2;
		size = size2;
	} else {
		int size3 = size + size2;
		char *scode3 = new char[size3];
		int pos = 0;
		for( int i=0; i<size2; i++ )	{
			scode3[pos] = scode2[i];
			pos++;
		}
		for( int i=0; i<size; i++ )	{
			scode3[pos] = scode[i];
			pos++;
		}
		FreeVarTab(scode);
		FreeVarTab(scode2);
		scode = scode3;
		size = size3;
	}
}

int main(int argc, char **argv)	{
	cvars = new ScriptVar[isizevars];
	scfuncs = new ScriptFunc[isizefuncs];
	
	if( argc>1 )	{
		loadscript(argv[1]);
		if( argc>2 )	{
			loadchart("chart", argv[2]);
			addvar("ticker", getact()->chart[0].sticker);
		} else addvar("ticker","");
	} else {
		mperror("usage: %s <script file> [data file]\n");
	}
	
	char sinclude[32][128];
	idcode = 0;
	int idinclude = 0;
	int pos;
	while(idcode < size)	{
		skip();
		pos = idcode;
		readnext();
		if( mis("%include") )	{
			skip();
			getnext();
			strcpy(sinclude[idinclude],skey);
			idinclude++;
			for( int i=pos; i<idcode; i++ )	{
				scode[i] = ' '; // zerujemy includa
			}
		}
	}
	idcode = 0;
	for( int i=0; i<idinclude; i++ )	{
		loadscript(sinclude[i]);
	}
	
	char c;
	double f1,f2,f3,f4;
	ScriptVar *vareq;
	while(idcode < size )	{
		c = scode[idcode];
		if( state==0 )	{
			if( c==' ' || c==';' || c==']' ) {
				idcode++;
			} else if( c=='$' )	{
				getvar(1);
				vareq = getact();
				if( scode[idcode]=='=' )	{
					idcode++;
					skip();
					if( getvalue() )	{
						if( igotvalue==VAL_VARIABLE )	{
							if( vareq != getact() )
								vareq->set(getact());
						} else if( igotvalue==VAL_ARR )	{
							skip();
							if( misnumber() )	{
								vareq->setasarr();
								while(idcode<size && scode[idcode]!=']')	{
									if( getvalue() )	{
										vareq->addarr(calcvalue());
									} else mperror("variable expected in array; received:",skey);
									skip();
								}
							} else {
								vareq->setasstrarr();
								char s[128];
								while(idcode<size && scode[idcode]!=']')	{
									if( getvalue() )	{
										calcvalue(1);
										vareq->addstrarr(skey);
									} else mperror("variable expected in array; received:",skey);
									skip();
								}
							}
							/*if( idcode<size && scode[idcode]==']' ) idcode++;
							else mperror("missing ] in array");*/
						} else {
							vareq->set(calcvalue());
						}
					} else vareq->set(skey);
					if( scode[idcode]!=']' ) state = 3;
				} else if( scode[idcode]=='@' )	{
					idcode++;
					skip();
					if( getvalue() )	{
						if( igotvalue==VAL_VARIABLE )	{
							if( vareq != getact() ) {
								vareq->set(getact()->value);
							}
						} else {
							calcvalue(1);
							vareq->set(skey);
						}
					} else vareq->set(skey);
					state = 3;
				} else if( scode[idcode]=='.' )	{
					getfuncresult();
				} else {
					getnext();
					if( mis("++") )	{
						vareq->set( vareq->fvalue + 1.0 );
					} else if( mis("--") )	{
						vareq->set( vareq->fvalue - 1.0 );
					} else mperror("expected = or inc/dec after var",skey);
				}
			} else if( c=='@' )	{
				int id = readfunc();
				if( id>=0 )	{
					int istack = iidstack;
					int ilearg = 0;
					char sid[32];
					while(idcode<size && scode[idcode]!=';')	{
						if( getvalue() )	{
							if( igotvalue==VAL_VARIABLE )	{
								pushvar(getact());
							} else if( igotvalue==VAL_STRING )	{
								calcvalue();
								pushvar(skey);
							} else {
								pushvar(calcvalue());
							}
						} else pushvar(skey);
						ilearg++;
						sprintf(sid,"%d",ilearg);
						getlocal()->setname(sid);
						skip();
					}
					scfuncs[id].call(istack, ilearg);
				} else mperror("func not found",skey);
			} else {
				getnext();
				if( mis("if") )	{
					cmpstate = 0;
					logicstate = -1;	// and = 0; or=1
					state = 1;
					idifstack++;
				} else if ( mis("while") )	{
					cmpstate = 0;
					logicstate = -1;	// and = 0; or=1
					state = 4;
					loops[idwhilestack] = idcode;
					idwhilestack++;
				} else if ( mis("print") )	{
					state = 2;
				} else if ( mis("else") )	{
					if( idifstack>0 )	{
						int tmpif = 0;
						int b=1;
						while(idcode<size && b)	{
							readnext();
							if( mis("if") ) tmpif++;
							else if( mis("fi") )	{
								tmpif--;
								if( tmpif<0 )	{
									b=0;
								}
							}
						}
						if( idcode>=size && b ) mperror("key word 'fi' not found");
						idifstack--;
						if( idifstack < 0 )	{
							mperror("ifstack < 0");
						}
					} else mperror("else without if");
				} else if ( mis("fi") )	{
					idifstack--;
					if( idifstack < 0 )	{
						mperror("found fi: ifstack < 0");
					}
				} else if ( mis("done") )	{
					if( idwhilestack < 1 )	{
						mperror("found done: idwhilestack < 0");
					}
					idcode = loops[idwhilestack-1];	// goto
					cmpstate = 0;
					logicstate = -1;	// and = 0; or=1
					state = 4;
				} else if ( mis("end") )	{	// end of function
					if( lastfunc!=NULL )	{
						lastfunc = lastfunc->retcaller();
					} else mperror("found kw end beyond func");
				} else if ( mis("func") )	{
					skip();
					readfunc();
					addfunc(skey);
					bool b = 1;
					while(idcode<size && b)	{
						readnext();
						if( mis("end") ) b=0;
						else if( mis("func") )	{
							mperror("function not closed!");
						}
					}
				} else if( mis("local") )	{
					skip();
					getnext();
					setlocalvar(skey);
				} else if( mis("exit") )	{
					exit(0);
				} else if( mis("push") )	{
					skip();
					if( getvalue() )	{
						if( igotvalue==VAL_VARIABLE )	{
							pushvar(getact());
						} else {
							pushvar(calcvalue());
						}
					} else pushvar(skey);
				} else if( mis("pop") )	{
					skip();
					if( getvalue() && igotvalue==VAL_VARIABLE )	{
						getact()->set(popvar());
					} else mperror("pop: expected variable, received %s", skey);
				} else if( mis("debug") )	{
					skip();
					igmdebug = getnumber();
					printf("igmdebug: = %d\n",igmdebug);
				} else if( mis("codedebug") )	{
					skip();
					igmcodedebug = getnumber();
				} else {
					mperror("unexpected key word", skey);
				}
			}
		} else if (state==1 )	{	// comparizon
			if( c==' ' ) idcode++;
			else {
				if( getvalue() )	{
					if( cmpstate==0 )	{
						val1 = calcvalue();
						laststate = 1;
						state = 200;
					} else {
						bcmptest = mcalc(val1,calcvalue());
						fincmptest();
					}
				} else {
					mperror("unexpected key word", skey);
				}
			}
		} else if (state==4 )	{	// comparizon
			if( c==' ' ) idcode++;
			else {
				if( getvalue() )	{
					if( cmpstate==0 )	{
						val1 = calcvalue();
						laststate = 4;
						state = 200;
					} else {
						bcmptest = mcalc(val1,calcvalue());
						fincmptest();
					}
				} else {
					mperror("unexpected key word", skey);
				}
			}
		} else if (state==2 )	{
			if (c==';') {
				printf("\n");
				idcode++;
				state = 0;
			} else if( c=='$' )	{
				getvar();
				if( scode[idcode]=='.' )	{
					float f = getfuncresult();
					if( f!=0.0 )	{
						int pom = f;
						if( f==pom )
							printf("%d ", pom);
						else
							printf("%f ", f);
					}
				} else {
					getact()->printvar();
				}
			} else {
				printf("%c",c);
				idcode++;
			}
		} else if (state==3 )	{
			if( c==' ' ) idcode++;
			else if (c==';') {
				state = 0;
			} else if( c=='+' || c=='-' || c=='*' || c=='/' ) {
				idcode++;
				skip();
				if( getvalue() )	{
					if( vareq->ityp==0 )	{
						calcvalue(1);
						if( c=='+' )	{
							strcat(vareq->value, skey);
						} else {
							char c2[2];
							c2[0]=c; c2[1]='\0';
							mperror("wrong operator in string expression",c2);
						}
					} else {
						double v2 = calcvalue();
						if( c=='+' )	{		vareq->fvalue += v2;
						} else if( c=='-' )	{	vareq->fvalue -= v2;
						} else if( c=='*' )	{	vareq->fvalue *= v2;
						} else if( c=='/' )	{	vareq->fvalue /= v2;
						}
					}
				}
				vareq->update();
				//skip();
			}
		} else if( state==200 )	{
			if( c==' ' ) idcode++;
			else {
				getnext();
				if( mis("<") )	{
					cmpstate = 1;
					state=1;
				} else if( mis("<=") )	{
					cmpstate = 2;
					state=1;
				} else if( mis("==") )	{
					cmpstate = 3;
					state=1;
				} else if( mis(">") )	{
					cmpstate = 4;
					state=1;
				} else if( mis(">=") )	{
					cmpstate = 5;
					state=1;
				} else if( mis("!=") )	{
					cmpstate = 6;
					state=1;
				} else {
					mperror("unexpected key word", skey);
				}
			}
		} else if( state==201 )	{
			if( c==' ' ) idcode++;
			else if( c=='&' )	{
				logicstate = 0;
				idcode++;
				state = 1;
			} else if( c=='|' )	{
				logicstate = 1;
				idcode++;
				state = 1;
			} else {
				getnext();
				if( mis("then") )	{
					if( blastcmp )	{
						state=0;
					} else {
						int tmpif = 0;
						int b=1;
						while(idcode<size && b)	{
							readnext();
							if( mis("if") ) tmpif++;
							else if( mis("fi") )	{
								tmpif--;
								if( tmpif<0 )	{
									b=0;
									state=0;
									idifstack--;
									if( idifstack < 0 )	mperror("ifstack < 0");
								}
							} else if( mis("else") )	{
								if( tmpif==0 )	{
									b=0;
									state=0;
								}
							}
						}
						if( idcode>=size && b ) mperror("key word 'fi' or 'else' not found");
					}
				} else if( mis("do") )	{
					if( blastcmp )	{
						state=0;
					} else {
						int tmpwhile = 0;
						int b=1;
						while(idcode<size && b)	{
							readnext();
							if( mis("while") ) tmpwhile++;
							else if( mis("done") )	{
								tmpwhile--;
								if( tmpwhile<0 )	{
									b=0;
									state=0;
									idwhilestack--;
									if( idwhilestack < 0 )	mperror("idwhilestack < 0");
								}
							}
						}
					}
				} else {
					mperror("unexpected key word\n", skey);
				}
			}
		}
	}
	
	freevars();
	
	return 0;
}
