/* 
 * Ryan McCoppin 's project 1
 * September 14, 2012
 * Computer Vision, CEG 7550
 * Due: October 11, 2012 @ 3:30pm

 */

#include "define.h"


void gaussian_filter(ByteImage& bimg, const double sigma) {

	int g_size = 6 * sigma + 1;
	int middle = (int) g_size / 2;
	int pixel;

	// Visual Studio is picky about how arrays are made
	// Used store 1-D gaussian filter
	double* gauss = new double[g_size];
	double gauss_sum = 0, gauss_remove = 0;
	double divisor = 0;
	

	double etmp = 2*sigma*sigma;
	double tmp = sqrt((double)PI*etmp);
	// Create 1-D gaussian based on sigma, band length is 6*sigma + 1
	for (int i = 0; i < g_size; i++)
	{
		//gauss[i] = 10.0/tmp*exp(-1.0*(i-middle)*(i-middle)/etmp);
		divisor += (gauss[i] = ceil(5*tmp*exp(-1.0*(i-middle)*(i-middle)/etmp)));
		// gauss[i];
		//std::cout << gauss[i]  << "  " << divisor << std::endl;
	}


	// Apply gaussian to image horizontally and then vertically

	// For each pixel in the image, apply horizontal filter
	for (int i = 0; i < bimg.nr; i++)
	{
		for (int j = 0; j < bimg.nc; j++)
		{
			gauss_sum = 0;
			gauss_remove = 0;
			for (int k = 0; k < g_size; k++)
			{
				// If you leave the border, flip outer half to inner half
				if (j-middle+1+k < 0 || j-middle+1+k >= bimg.nc)
					gauss_remove -= gauss[k]; //gauss_sum += gauss[k] * bimg.image[i*bimg.nc+(j-middle+1-k)];
				else
					gauss_sum += gauss[k] * bimg.image[i*bimg.nc+(j-middle+1+k)];
			}
			bimg.image[i*bimg.nc+j] = ceil(gauss_sum/(divisor+gauss_remove));
			//std::cout << "value after: " << (int)bimg.image[i*bimg.nc+j] << std::endl;
		}
	}

	// For each pixel in the image, apply vertical filter
	for (int i = 0; i < bimg.nr; i++)
	{
		for (int j = 0; j < bimg.nc; j++)
		{
			gauss_sum = 0;
			gauss_remove =0;
			for (int k = 0; k < g_size; k++)//-(middle-1); k < middle; k++)
			{
				// if you leave the border, flip outer half to inner half
				if (i-middle+1+k < 0 || i-middle+1+k >= bimg.nr)
					gauss_remove -= gauss[k];//gauss_sum += gauss[k] * bimg.image[(i-middle+1-k)*bimg.nc+j];
				else 
					gauss_sum += gauss[k] * bimg.image[(i-middle+1+k)*bimg.nc+j];
			}
			bimg.image[i*bimg.nc+j] = ceil(gauss_sum/(divisor+gauss_remove));
		}
	}
	// For each pixel in the image, apply horizontal filter

	delete [] gauss;
} // End gaussian_filter method