#include	"dat.h"
#include	"fns.h"
#include	"error.h"

char file1[256];
char file2[256];

enum
{
	Qdir,
	Qctl,
	Qdata,
};

Dirtab raidtab[] =
{
	".",		{Qdir, 0, QTDIR},	0,	0556,
	"ctl",		{Qctl},		0,	0701,
	"data",		{Qdata},	0,	0702,
};

static Chan*
raidattach(char *spec)
{
	print("In raidattach\n");
	return devattach('R', spec);
}

static Walkqid*
raidwalk(Chan *c, Chan *nc, char **name, int nname)
{
	return devwalk(c, nc, name, nname, raidtab, nelem(raidtab), devgen);
}

static int
raidstat(Chan *c, uchar *db, int n)
{
	return devstat(c, db, n, raidtab, nelem(raidtab), devgen);
}

static Chan*
raidopen(Chan *c, int omode)
{
	c = devopen(c, omode, raidtab, nelem(raidtab), devgen);
	if(waserror()){
		c->flag &= ~COPEN;
		nexterror();
	}
	switch(c->qid.path) {
	case Qdir:
	case Qctl:
		break;
	case Qdata:
		break;
	default:
		error(Egreg);
	}
	poperror();
	return c;
}

static void
raidclose(Chan *c)
{
	if((c->flag & COPEN) == 0)
		return;

	switch(c->qid.path) {
	case Qdir:
	case Qctl:
		USED(c);
		break;
	case Qdata:
		USED(c);
		break;
	default:
		error(Egreg);
	}
}

static long
raidread(Chan *c, void *va, long count, vlong offset)
{
	char *buf;
	int n;
	
	if(c->qid.type & QTDIR)
		return devdirread(c, va, count, raidtab, nelem(raidtab), devgen);
	
	switch(c->qid.path) {
	case Qctl:
		print("Attempting to read ctl file\n");
		return n;
	case Qdata:
		print("Attempting to read data file\n");
		
		// Attempt to read from file 1, if fails, try file 2
		
		return n;
	}
	return 0;
}

static long
raidwrite(Chan *c, void *va, long count, vlong offset)
{
	char str[(int)count];
	char *p;
	int i;


	if (count <= 0) {
		return 0;
	}

	memmove(str,va,(int)count);
	str[count-1] = '\0';

	switch(c->qid.path) {
	case Qctl:
		// Parse command line args
		p = strtok(str," ");

		if (strcmp("bind", p) == 0) {
			// Get first file
			p = strtok(nil, " ");
			if (p != nil) {
				strcpy(file1, p);				
			} else {
				break;
			}

			// Get second file
			p = strtok(nil, " ");
			if (p != nil) {
				strcpy(file2, p);
			} else {
				break;
			}

			//print("file1 = %s\n", file1);
			//print("file2 = %s\n", file2);

		} else {
			print("Error writing to raid control file\n");
			break;
		}

		break;
	case Qdata:
		if (file1 == nil || file2 == nil) {
			print("no files bound to raid yet\n");
			break;
		}
		
		// Attempt to write to file 1
		int f1, f2;

		f1 = kopen(file1,OWRITE);
		print("file1 = %s\n", file1);
		if (f1 == -1) {
			print("Raid write to file 1 failed\n");
		
			// Try to write to file 2
			f2 = kopen(file2, OWRITE);

			if (f2 == -1) {
				print("Raid write to file 2 failed\n");
			} else {
				fprint(f2,"%s\n", str);			
			}
		
		} else {
			fprint(f1,"%s\n", str);
			
			// Try to write to file 2
                        f2 = kopen(file2, OWRITE);

                        if (f2 == -1) {
                                print("Raid write to file 2 failed\n");
                        } else {
				fprint(f2,"%s\n", str);
                        }
		}
		break;
	}
	return 0;
}

Dev raiddevtab = {
        'R',
        "raid",

        devinit,
        raidattach,
        raidwalk,
        raidstat,
        raidopen,
	devcreate,
        raidclose,
        raidread,
        devbread,
        raidwrite,
        devbwrite,
        devremove,
        devwstat
};
