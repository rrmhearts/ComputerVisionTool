/* 
 * Ryan McCoppin 's project 1
 * September 14, 2012
 * Computer Vision, CEG 7550
 * Due: October 11, 2012 @ 3:30pm

 */

#include "define.h"
#define PI 4*atan(1)
void suppression (ByteImage&,double);
void finish(ByteImage&);

void edge_detector(ByteImage& bimg, const double sigma) {

	// There is a copy constructor
	ByteImage bimg_161s = bimg;
	int * dog = new int[bimg.nr*bimg.nc];

	// Apply gaussian with sigma
	gaussian_filter(bimg, sigma);

	// Apply gaussian again with 1.61 * sigma
	gaussian_filter(bimg_161s, sigma * 1.61);

	// For each pixel, compute dog image
	for (int i = 0; i < bimg.nr*bimg.nc; i++)
	{
		dog[i] = (int)bimg.image[i] - (int)bimg_161s.image[i];
	}
	for (int i = 0; i < bimg.nr; i++)
	{
		for (int j = 0; j < bimg.nc; j++)
		{
			// Default set to black
			bimg.image[i*bimg.nc+j] = 0;

			// For x direction
			if ( j > 0 && j+1 < bimg.nc )
			{
				if ( dog[i*bimg.nc+j-1] < 0 != dog[i*bimg.nc+j+1] < 0)
					bimg.image[i*bimg.nc+j] = 255;
			}
			// For y direction
			if ( i > 0 && i+1 < bimg.nr )
			{
				 if (dog[(i-1)*bimg.nc+j] < 0 != dog[(i+1)*bimg.nc+j] < 0)
					 bimg.image[i*bimg.nc+j] = 255;
			}
			// For diagonals dir
			if ( i > 0 && i+1 < bimg.nr && j > 0 && j+1 < bimg.nc )
			{
				if (dog[(i-1)*bimg.nc+j-1] < 0 != dog[(i+1)*bimg.nc+j+1] < 0)
					 bimg.image[i*bimg.nc+j] = 255;
				else if (dog[(i+1)*bimg.nc+j-1] < 0 != dog[(i-1)*bimg.nc+j+1] < 0)
					 bimg.image[i*bimg.nc+j] = 255;
			}

		} // loop through columns
	} // loop through rows

	delete [] dog;
} // End edge_detector method

void edge_gradients(ByteImage& bimg, const double sigma) {

	double * dx = new double[bimg.nc*bimg.nr];
	double dy;
	short grad;
	// Apply gaussian with sigma
	if (sigma > 0 )
		gaussian_filter(bimg, sigma);

	// Compute gradient image

	// Horizontal derivatives
	for (int i = 0; i < bimg.nr; i++)
	{
		for (int j = 0; j < bimg.nc; j++)
		{
			// Minimum edge
			if (j == 0 || j == 1)
			{
				dx[i*bimg.nc+j] = bimg.image[i*bimg.nc+j+1] - bimg.image[i*bimg.nc+j];
			// Maximum edge
			} else if (j == bimg.nc-1 || j == bimg.nc-2)
			{
				dx[i*bimg.nc+j] = bimg.image[i*bimg.nc+j] - bimg.image[i*bimg.nc+j-1];
			} else 
			{
				// A stable gradient measure
				dx[i*bimg.nc+j] = bimg.image[i*bimg.nc+j+1] - bimg.image[i*bimg.nc+j];//(bimg.image[i*bimg.nc+j+1] - bimg.image[i*bimg.nc+j-1])/2;
			}
		} // loop through columns
	} // loop through rows

	// Vertical derivatives, compute gradient grad = sqrt( hor^2 + ver^2 )
	for (int i = 0; i < bimg.nr; i++)
	{
		for (int j = 0; j < bimg.nc; j++)
		{
			// Minimum edge
			if (i == 0 || i == 1)
			{
				dy = bimg.image[(i+1)*bimg.nc+j] - bimg.image[i*bimg.nc+j];

				// Compute absolute gradient
				grad = floor(sqrt(dx[i*bimg.nc+j]*dx[i*bimg.nc+j] + dy*dy));
				if (grad > 255) grad = 255;
				bimg.image[i*bimg.nc+j] = grad;
			// Maximum edge
			} else if (i == bimg.nr-1 || i == bimg.nr-2)
			{
				dy = bimg.image[i*bimg.nc+j] - bimg.image[(i-1)*bimg.nc+j];
				//std::cout << dy << std::endl;
				// Compute absolute gradient
				grad = floor(sqrt(dx[i*bimg.nc+j]*dx[i*bimg.nc+j] + dy*dy));
				if (grad > 255) grad = 255;
				bimg.image[i*bimg.nc+j] =  grad;
			} else 
			{
				// A stable gradient measure
				dy = (bimg.image[(i+1)*bimg.nc+j] - bimg.image[i*bimg.nc+j]);//bimg.image[(i-1)*bimg.nc+j])/2;
				//std::cout << dy << std::endl;
				// Compute absolute gradient
				grad = floor(sqrt(dx[i*bimg.nc+j]*dx[i*bimg.nc+j] + dy*dy));
				if (grad > 255) grad = 255;
				bimg.image[i*bimg.nc+j] =  grad;//dx[i*bimg.nc+j];//grad;
			}

		} // loop through columns
	} // loop through rows

	// Find max value, off edges
	double max = 0, min = 255;
	for (int i = 1; i < bimg.nr-3; i++)
	{
		for (int j = 1; j < bimg.nc-3; j++)
		{
			if (bimg.image[i*bimg.nc+j] > max)
				max = bimg.image[i*bimg.nc+j];
			if (bimg.image[i*bimg.nc+j] < min)
				min = bimg.image[i];
		}
	}

	// Locally weak gradient suppression (for funsies)
	for (int i = 0; i < bimg.nr; i++)
	{
		for (int j = 0; j < bimg.nc; j++)
		{
			//bool xtrue=false, ytrue=false, diagtrue=false;
			//// For x direction
			//if ( j > 0 && j+1 < bimg.nc )
			//{
			//	if (bimg.image[i*bimg.nc+j-1] > bimg.image[i*bimg.nc+j] || bimg.image[i*bimg.nc+j+1] > bimg.image[i*bimg.nc+j])
			//		xtrue = true;//bimg.image[i*bimg.nc+j] = 0;
			//}
			//// For y direction
			//if ( i > 0 && i+1 < bimg.nr )
			//{
			//	if (bimg.image[(i-1)*bimg.nc+j] > bimg.image[i*bimg.nc+j] || bimg.image[(i+1)*bimg.nc+j] > bimg.image[i*bimg.nc+j])
			//		ytrue = true;//bimg.image[i*bimg.nc+j] = 0;
			//}
			//// For diagonals dir
			//if ( i > 0 && i+1 < bimg.nr && j > 0 && j+1 < bimg.nc )
			//{
			//	if (bimg.image[(i-1)*bimg.nc+j-1] > bimg.image[i*bimg.nc+j] || bimg.image[(i+1)*bimg.nc+j+1] > bimg.image[i*bimg.nc+j])
			//		diagtrue = true;//bimg.image[i*bimg.nc+j] = 0;
			//	if (bimg.image[(i-1)*bimg.nc+j+1] > bimg.image[i*bimg.nc+j] || bimg.image[(i+1)*bimg.nc+j-1] > bimg.image[i*bimg.nc+j])
			//		diagtrue = true; //bimg.image[i*bimg.nc+j] = 0;
			//}
			//if (xtrue && ytrue && diagtrue)
			//	bimg.image[i*bimg.nc+j] = 0;

			// Set gradient
			bimg.image[i*bimg.nc+j] = floor((double)bimg.image[i*bimg.nc+j]/max*255);

		} // loop through columns
	} // loop through rows
	delete [] dx;

} // End edge_gradients method

// Produces binary edge image from gradient image
void edge_gradients(ByteImage& bimg, const double sigma, double threshold) {

	ByteImage original = bimg;
	double * dx = new double[bimg.nc*bimg.nr];
	double dy;
	short grad;
	// Apply gaussian with sigma
	gaussian_filter(bimg, sigma);

	// Compute gradient image

	// Horizontal derivatives
	for (int i = 0; i < bimg.nr; i++)
	{
		for (int j = 0; j < bimg.nc; j++)
		{
			// Minimum edge
			if (j == 0 || j == 1)
			{
				dx[i*bimg.nc+j] = bimg.image[i*bimg.nc+j+1] - bimg.image[i*bimg.nc+j];
			// Maximum edge
			} else if (j == bimg.nc-1 || j == bimg.nc-2)
			{
				dx[i*bimg.nc+j] = bimg.image[i*bimg.nc+j] - bimg.image[i*bimg.nc+j-1];
			} else 
			{
				// A stable gradient measure
				dx[i*bimg.nc+j] = (bimg.image[i*bimg.nc+j+1] - bimg.image[i*bimg.nc+j-1])/2;
			}
		} // loop through columns
	} // loop through rows

	// Vertical derivatives, compute gradient grad = sqrt( hor^2 + ver^2 )
	for (int i = 0; i < bimg.nr; i++)
	{
		for (int j = 0; j < bimg.nc; j++)
		{
			// Minimum edge
			if (i == 0 || i == 1)
			{
				dy = (int)bimg.image[(i+1)*bimg.nc+j] - bimg.image[i*bimg.nc+j];

				// Compute absolute gradient
				grad = floor(sqrt(dx[i*bimg.nc+j]*dx[i*bimg.nc+j] + dy*dy));
				if (grad > 255) grad = 255;
				bimg.image[i*bimg.nc+j] = grad;
			// Maximum edge
			} else if (i == bimg.nr-1 || i == bimg.nr-2)
			{
				dy = (int)bimg.image[i*bimg.nc+j] - bimg.image[(i-1)*bimg.nc+j];

				// Compute absolute gradient
				grad = floor(sqrt(dx[i*bimg.nc+j]*dx[i*bimg.nc+j] + dy*dy));
				if (grad > 255) grad = 255;
				bimg.image[i*bimg.nc+j] =  grad;
			} else 
			{
				// A stable gradient measure
				//dy = (bimg.image[(i+1)*bimg.nc+j] - bimg.image[(i-1)*bimg.nc+j])/2;
				dy = (bimg.image[(i+1)*bimg.nc+j] - bimg.image[i*bimg.nc+j]);
				// Compute absolute gradient
				grad = floor(sqrt(dx[i*bimg.nc+j]*dx[i*bimg.nc+j] + dy*dy));
				if (grad > 255) grad = 255;
				bimg.image[i*bimg.nc+j] =  grad;
			}

		} // loop through columns
	} // loop through rows

	// Find max value
	double max = 0, min = 255;
	for (int i = 0; i < bimg.nr*bimg.nc; i++)
	{
		if (bimg.image[i] > max)
			max = bimg.image[i];
		if (bimg.image[i] < min)
			min = bimg.image[i];
	}
	std::cout << "Image gradient range: [" << min << ", " << max << "]" << std::endl;
	//std::cout << max << ", " << min << std::endl;

	// Convert threshold if a percent
	if (threshold < 1)
		threshold = floor((max-min)*threshold);

	// You can remove this line for the boring version ***************
	//suppression(bimg, threshold);

	// Remove locally weak gradients interactively
	for (int i = 0; i < bimg.nr; i++)
	{
		for (int j = 0; j < bimg.nc; j++)
		{
			if ( bimg.image[i*bimg.nc+j] < threshold )
			{
				bimg.image[i*bimg.nc+j] = 0;
			} else {
				bimg.image[i*bimg.nc+j] = 255;
			}

		} // loop through columns
	} // loop through rows

	// You can remove this line for the boring version ***********************
	//finish(bimg);
	delete [] dx;

} // End edge_gradients method

// (Extra credit) Used in Binary threshold method for FUN, better results?
void suppression (ByteImage& bimg, double threshold) 
{
	// Find max value
	double max = 0, min = 255;
	for (int i = 0; i < bimg.nr*bimg.nc; i++)
	{
		if (bimg.image[i] > max)
			max = bimg.image[i];
		if (bimg.image[i] < min)
			min = bimg.image[i];
	}
	// Locally weak gradient suppression (for funsies)
	for (int i = 0; i < bimg.nr; i++)
	{
		for (int j = 0; j < bimg.nc; j++)
		{
			bool xtrue=false, ytrue=false, diagtrue=false;
			// For x direction
			if ( j > 0 && j+1 < bimg.nc )
			{
				if (bimg.image[i*bimg.nc+j-1] > bimg.image[i*bimg.nc+j] || bimg.image[i*bimg.nc+j+1] > bimg.image[i*bimg.nc+j])
				{
					bimg.image[i*bimg.nc+j] = 0;
					if (bimg.image[i*bimg.nc+j] > threshold)
					if (bimg.image[i*bimg.nc+j-1] > bimg.image[i*bimg.nc+j+1])
						bimg.image[i*bimg.nc+j-1] = 255;
					else
						bimg.image[i*bimg.nc+j+1] = 255;
				}
			}
			// For y direction
			if ( i > 0 && i+1 < bimg.nr )
			{
				if (bimg.image[(i-1)*bimg.nc+j] > bimg.image[i*bimg.nc+j] || bimg.image[(i+1)*bimg.nc+j] > bimg.image[i*bimg.nc+j])
				{
					bimg.image[i*bimg.nc+j] = 0;
					if (bimg.image[i*bimg.nc+j] > threshold)
					if (bimg.image[(i-1)*bimg.nc+j] > bimg.image[(i+1)*bimg.nc+j])
						bimg.image[(i-1)*bimg.nc+j] = 255;
					else
						bimg.image[(i+1)*bimg.nc+j] = 255;
				}
			}
			// For diagonals dir
			if ( i > 0 && i+1 < bimg.nr && j > 0 && j+1 < bimg.nc )
			{
				if (bimg.image[(i-1)*bimg.nc+j-1] > bimg.image[i*bimg.nc+j] || bimg.image[(i+1)*bimg.nc+j+1] > bimg.image[i*bimg.nc+j])
				{
					bimg.image[i*bimg.nc+j] = 0;
					if (bimg.image[i*bimg.nc+j] > threshold)
					if (bimg.image[(i-1)*bimg.nc+j-1] > bimg.image[(i+1)*bimg.nc+j+1])
						bimg.image[(i-1)*bimg.nc+j-1] = 255;
					else
						bimg.image[(i+1)*bimg.nc+j+1] = 255;
				}
				if (bimg.image[(i-1)*bimg.nc+j+1] > bimg.image[i*bimg.nc+j] || bimg.image[(i+1)*bimg.nc+j-1] > bimg.image[i*bimg.nc+j])
				{
					bimg.image[i*bimg.nc+j] = 0;
					if (bimg.image[i*bimg.nc+j] > threshold)
					if (bimg.image[(i-1)*bimg.nc+j+1] > bimg.image[(i+1)*bimg.nc+j-1])
						bimg.image[(i-1)*bimg.nc+j+1] = 255;
					else
						bimg.image[(i+1)*bimg.nc+j-1] = 255;
				}
			}
		} // loop through columns
	} // loop through rows
}

// (Extra credit) Used in Binary threshold method for FUN, better results?
void finish(ByteImage& bimg)
{

	// Fill in missing values (sort of make lines)
	for (int i = 0; i < bimg.nr; i++)
	{
		for (int j = 0; j < bimg.nc; j++)
		{
			bool morezeros = false;
			// For x direction
			if ( j > 0 && j+1 < bimg.nc )
			{
				if (bimg.image[i*bimg.nc+j-1] == 255 && bimg.image[i*bimg.nc+j+1] == 255)
					bimg.image[i*bimg.nc+j] = 255;
				else
					morezeros = true;
			}
			// For y direction
			if ( i > 0 && i+1 < bimg.nr )
			{
				if (bimg.image[(i-1)*bimg.nc+j] == 255 && bimg.image[(i+1)*bimg.nc+j] == 255)
				{
					bimg.image[i*bimg.nc+j] = 255;
					morezeros = false && morezeros;
				}
				else
					morezeros = true && morezeros;
			}
			// For diagonals dir
			if ( i > 0 && i+1 < bimg.nr && j > 0 && j+1 < bimg.nc )
			{
				if (bimg.image[(i-1)*bimg.nc+j-1] == 255 && bimg.image[(i+1)*bimg.nc+j+1] == 255)
					bimg.image[i*bimg.nc+j] = 255;
				if (bimg.image[(i-1)*bimg.nc+j+1] == 255 && bimg.image[(i+1)*bimg.nc+j-1] == 255)
					bimg.image[i*bimg.nc+j] = 255;

				//if (bimg.image[(i-1)*bimg.nc+j-1] == 0 && bimg.image[(i+1)*bimg.nc+j+1] == 0 && morezeros)
				//	bimg.image[i*bimg.nc+j] = 0;
				//if (bimg.image[(i-1)*bimg.nc+j+1] == 0 && bimg.image[(i+1)*bimg.nc+j-1] == 0 && morezeros)
				//	bimg.image[i*bimg.nc+j] = 0;
			}

		} // loop through columns
	} // loop through rows
}