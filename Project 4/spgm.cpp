/* Save Routine */

/* Ryan McCoppin 's project 1
 * September 14, 2012
 * Computer Vision, CEG 7550
 * Due: September 25, 2012 @ 3:30pm

 */

// Used for saving pgm images
#include "define.h"

void spgm(FILE *fp, const ByteImage& bimg)
{
	fprintf(fp,"P5\n");
	fprintf(fp,"# Image generated at CSE Dept., WSU.\n");
	printf(" Image being saved is %dx%d.\n", bimg.nr, bimg.nc);
	fprintf(fp,"%d %d\n", bimg.nc, bimg.nr);
	fprintf(fp,"255\n");
	fwrite(bimg.image, bimg.nr * bimg.nc, 1, fp);
}
