/* 
 * Ryan McCoppin 's project 1
 * September 14, 2012
 * Computer Vision, CEG 7550
 * Due: October 11, 2012 @ 3:30pm

 */

#include "define.h"
clock_t init, final;

// See if the new image is different from the old one
bool check_diff(const ByteImage& a, const ByteImage& b)
{
	int sum = 0;
	for (int i = 0; i < a.nr; i++)
	{
		for (int j = 0; j < a.nc; j++)
		{
			sum += abs(a.image[i*a.nc+j]-b.image[i*a.nc+j]);
		}


	}
	return sum != 0;
}

// Bilateral filter, changes original image
void bilateral_filter(ByteImage& bimg, double sigmad, double sigmar) {

	init=clock();
	// Distance to move away from center
	int dist_away = floor(3*sigmad), f, f_local;

	// Calculate denominators
	double d_denom = 2*sigmad*sigmad;
	double r_denom = 2*sigmar*sigmar;

	double d, r, w, weight_sum, f_weight_sum, diff_squares;

	// For each pixel in the image
	for (int i = 0; i < bimg.nr; i++)
	{
		for (int j = 0; j < bimg.nc; j++)
		{
			// Resets weight totals
			weight_sum = 0; f_weight_sum = 0;

			// Save pixel value once
			f = bimg.image[i*bimg.nc+j];

			// For a single pixel region
			for (int k = i-dist_away; k < i+dist_away; k++)
			{
				// Only pixels in bound on x
				if (k > -1 && k < bimg.nr)

					// Abs(k-i) causes a circle using Manhattan distance, also faster than a square
					for (int l = j-dist_away+abs(k-i); l < j+dist_away-abs(k-i); l++)
					{
						// Only pixel in bound on y
						if (l > -1 && l < bimg.nc)
						{
							// Save local pixel once for speed
							f_local = bimg.image[k*bimg.nc+l];

							// Compute difference of squares for range kernel
							diff_squares = (double) (f-f_local)*(f-f_local);//(double)f*f - f_local*f_local;

							// Compute distance metric for domain kernel
							d = - ( (i-k)*(i-k) + (j-l)*(j-l) ) / d_denom;

							// Compute range kernel
							r = - abs(diff_squares) / r_denom;

							// Compute weight function
							w = exp (d + r);

							// Compute denominator of bilateral filter
							weight_sum += w;

							// Compute numerator of bilateral filter
							f_weight_sum += f_local*w;
						}

				} // for each column (kernel)


			} // End single pixel region

			// Apply to pixel
			bimg.image[i*bimg.nc+j] = floor(f_weight_sum/weight_sum);


		} // for each column (picture)
	} // End pixel array
	final = clock() - init;
	std::cout << "Clock time: " << (double)final/((double)CLOCKS_PER_SEC) << " seconds" << std::endl;
} // End bilateral_filter method
