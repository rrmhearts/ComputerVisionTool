/*
 * byteimage.h, written by Brian Jackson, CSE Dept., WSU
 * A simple, single-channel image class
 */

#include <cstdio>
#include <cstring>

typedef unsigned char BYTE;

class ByteImage {
public:
	BYTE* image;
	int nr, nc;

	inline ByteImage() {
		image = NULL;
		nr = nc = 0;
	}
	inline ByteImage(int nr, int nc) {
		image = new BYTE [nr * nc];
		this->nr = nr;
		this->nc = nc;
	}
	inline ByteImage(const ByteImage& bimg) {
		image = NULL;
		*this = bimg;
	}
	inline ~ByteImage() {if (image) delete [] image;}
	
	inline ByteImage& operator=(const ByteImage& bimg) {
		if (image) delete [] image;
		nr = bimg.nr;
		nc = bimg.nc;
		image = new BYTE [nr * nc];
		memcpy(image, bimg.image, nr * nc);
		return *this;
	}

	//Flip all rows (used by some image formats)
	inline void flip() {
		BYTE* flipped = new BYTE [nr * nc];
		for(int r = 0; r < nr; r++)
			memcpy(flipped + r * nc, image + (nr - r - 1) * nc, nc);
		delete [] image;
		image = flipped;
	}
};
