/* Ryan McCoppin 's project 1
 * September 14, 2012
 * Computer Vision, CEG 7550
 * Due: September 25, 2012 @ 3:30pm

 */


// Used for loading pgm images.
#include "define.h"
 
int read_char(FILE *fp) {
	int c;

	if((c=fgetc(fp))==EOF)
		return(c);
	if(c=='#') {
		do {
			if((c=fgetc(fp))==EOF) return(c);
		} while(c!='\n'&&c!='\r');
	}

	return(c);
}

char *read_field(FILE *fp) {
	static char buf[71];
	int c,i;

	do {
		if((c=read_char(fp))==EOF) return(NULL);
	} while(isspace(c));
	for(i=0;i<70&&!isspace(c);i++) {
		buf[i]=c;
		if((c=read_char(fp))==EOF) break;
	}
	buf[i]='\0';
	return(buf);
}

double get_scale(FILE *fp) {
	char *p;
	double scale=0.0;

	if((p=read_field(fp))==NULL) return(scale);
	if((scale=(double) atol(p))<=0.0) return(0.0);
	return(255.0/scale);
}

ByteImage lpgm(FILE *fp) {
	ByteImage bimg;
	char *p, c;
	double scale;
	int i,j,k;

	if((p=read_field(fp))==NULL) {
		printf(" File is empty.\n");
	}
	else if(strcmp(p,"P5")!=0) {
		printf(" Image format is not pgm.\n");
	}
	else {
		p=read_field(fp);
		bimg.nc=atoi(p);
		p=read_field(fp);
		bimg.nr=atoi(p);
		printf(" Image is %dx%d.\n",bimg.nc,bimg.nr);
		
		bimg.image = new BYTE [bimg.nr * bimg.nc];
		if (!bimg.image) {
			printf(" Can't allocate memory for image.\n");
		}
		else {
			scale=get_scale(fp);
			if(scale!=1.0) {
				for(i=0;i<bimg.nr;i++)
					for(j=0;j<bimg.nc;j++) {
						c=fgetc(fp);
						k=(int) (((double) c)*scale);
						if(k>255)k=255;
							bimg.image[i*bimg.nc+j]=(BYTE)k;
					}
			} 
			else fread(bimg.image, bimg.nr * bimg.nc, 1, fp);
		}
	}

	return bimg;
}