/* Ryan McCoppin 's project 1
 * September 14, 2012
 * Computer Vision, CEG 7550
 * Due: September 25, 2012 @ 3:30pm

 */


#include <cstdio>
#include <cstdlib>
#include "windows.h"
#include "byteimage.h"
 
ByteImage lpgm(FILE*);
void spgm(FILE*, const ByteImage&);

ByteImage openImageDialog() {
	OPENFILENAME ofn;

	//Set size of structure
	ofn.lStructSize = sizeof(OPENFILENAME);

	//Owner and instance of the window
	ofn.hwndOwner = NULL;
	ofn.hInstance = NULL;

	//Set file-type filters
	char filters[200];
	sprintf(filters, "PGM Images|*.pgm;*.PGM|");
	for (int i = 0; filters[i]; i++) if (filters[i] == '|') filters[i] = 0;//Nulls separate files
	ofn.lpstrFilter = filters;

	//Not holding the custom filter
	ofn.lpstrCustomFilter = NULL;
	ofn.nFilterIndex = 0;

	//Set the file name
	char fn[1024];
	sprintf(fn, "");
	ofn.lpstrFile = fn;
	ofn.nMaxFile = 1024;//Size of file name buffer

	//Normally, this contains file+ext of selected file without path
	ofn.lpstrFileTitle = NULL;

	//Initial directory is set to default
	ofn.lpstrInitialDir = NULL;

	//Use default title, too
	ofn.lpstrTitle = NULL;

	//Default flags
	ofn.Flags = 0;

	//Default extension: pgm
	char defFilter[4];
	sprintf(defFilter, "pgm");
	ofn.lpstrDefExt = defFilter;

	ByteImage bimg;
	if (GetOpenFileName(&ofn)) {
		FILE* fp;
		if (fp = fopen(fn, "rb")) {
			bimg = lpgm(fp);
			fclose(fp);
		}
	}
	return bimg;
}

void saveImageDialog(const ByteImage& bimg) {
	OPENFILENAME ofn;

	//Set size of structure
	ofn.lStructSize = sizeof(OPENFILENAME);

	//Owner and instance of the window
	ofn.hwndOwner = NULL;
	ofn.hInstance = NULL;

	//Set file-type filters
	char filters[200];
	sprintf(filters, "PGM Images|*.pgm;*.PGM|");
	for (int i = 0; filters[i]; i++) if (filters[i] == '|') filters[i] = 0;//Nulls separate files
	ofn.lpstrFilter = filters;

	//Not holding the custom filter
	ofn.lpstrCustomFilter = NULL;
	ofn.nFilterIndex = 0;

	//Set the file name
	char fn[1024];
	sprintf(fn, "");
	ofn.lpstrFile = fn;
	ofn.nMaxFile = 1024;//Size of file name buffer

	//Normally, this contains file+ext of selected file without path
	ofn.lpstrFileTitle = NULL;

	//Initial directory is set to default
	ofn.lpstrInitialDir = NULL;

	//Use default title, too
	ofn.lpstrTitle = NULL;

	//Default flags
	ofn.Flags = 0;

	//Default extension: pgm
	ofn.lpstrDefExt = filters+13;

	if (GetOpenFileName(&ofn)) {
		FILE* fp;
		if (fp = fopen(fn, "wb")) {
			spgm(fp, bimg);
			fclose(fp);
		}
		else {
			printf("Couldn't save image!\n");
		}
	}
}