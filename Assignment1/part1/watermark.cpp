//
// Watermark.cpp : Add watermark to an image, or inspect if a watermark is present.
//
// Based on skeleton code by D. Crandall, Spring 2018
//
// PUT YOUR NAMES HERE
//
//

//Link to the header file
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <string>
#include "SImage.h"
#include "SImageIO.h"
#include "fft.h"
#include <cstdlib>
#include <bitset>
#include <vector>
#include <cmath>
#include <cstdio>
#include <algorithm>
#include <iomanip>
using namespace std;
#define PI 3.14159265

// This code requires that input be a *square* image, and that each dimension
//  is a power of 2; i.e. that input.width() == input.height() == 2^k, where k
//  is an integer. You'll need to pad out your image (with 0's) first if it's
//  not a square image to begin with. (Padding with 0's has no effect on the FT!)
//
// Forward FFT transform: take input image, and return real and imaginary parts.
//

void fft(const SDoublePlane &input, SDoublePlane &fft_real, SDoublePlane &fft_imag)
{
	fft_real = input;
	fft_imag = SDoublePlane(input.rows(), input.cols());

	FFT_2D(1, fft_real, fft_imag);
}

// Inverse FFT transform: take real and imaginary parts of fourier transform, and return
//  real-valued image.
//
void ifft(const SDoublePlane &input_real, const SDoublePlane &input_imag, SDoublePlane &output_real)
{
	output_real = input_real;
	SDoublePlane output_imag = input_imag;

	FFT_2D(0, output_real, output_imag);
}

// Write this in Part 1.1
SDoublePlane fft_magnitude(const SDoublePlane &fft_real, const SDoublePlane &fft_imag);

// Write this in Part 1.2
SDoublePlane remove_interference(const SDoublePlane &fft_real, const SDoublePlane &fft_imag);

// Write this in Part 1.3 -- add watermark N to image
SDoublePlane mark_image(const SDoublePlane &input, int N);

// Write this in Part 1.3 -- check if watermark N is in image
void check_image(const SDoublePlane &input, int N);

// Helper function
float correlationCoefficient(vector<double> X, vector<double> Y, int n);

int main(int argc, char **argv)
{
	try
	{

		if (argc < 4)
		{
			cout << "Insufficent number of arguments; correct usage:" << endl;
			cout << "    p2 problemID inputfile outputfile" << endl;
			return -1;
		}

		string part = argv[1];
		string inputFile = argv[2];
		string outputFile = argv[3];
		cout << "In: " << inputFile << "  Out: " << outputFile << endl;

		SDoublePlane input_image = SImageIO::read_png_file(inputFile.c_str());

		SDoublePlane real_part;
		SDoublePlane imag_part;
		SDoublePlane mag_part;
		SDoublePlane output_real;

		//Generating a Binary Vector.
		if (part == "1.1")
		{
			//TODO
			// Get real and imag part.
			fft(input_image, real_part, imag_part);
			SDoublePlane magnitude_matrix(real_part.rows(), imag_part.cols());

			// Generate spectogram.
			magnitude_matrix = fft_magnitude(real_part, imag_part);
			SImageIO::write_png_file("noise_spectrum.png", magnitude_matrix, magnitude_matrix, magnitude_matrix);
		}
		else if (part == "1.2")
		{
			//TODO
			// Get fft_real and fft_imag for noise1.png.
			fft(input_image, real_part, imag_part);

			// Generate image with the new fft_real and fft_image values.
			SDoublePlane new_image = remove_interference(real_part, imag_part);

			// Generate no noise image.
			SImageIO::write_png_file("nonoise.png", new_image, new_image, new_image);
		}
		else if (part == "1.3")
		{
			if (argc < 6)
			{
				cout << "Need 6 parameters for watermark part:" << endl;
				cout << "    p2 1.3 inputfile outputfile operation N" << endl;
				return -1;
			}
			string op(argv[4]);
			int N = atoi(argv[5]);
			if (op == "add")
			{
				int next_number = 0;
				int col_value = 0;
				int row_value = 0;
				cout << "Hmmm! Let's see if the image is square or not! If not, lets make it\n";
				if (input_image.rows() > input_image.cols())
				{
					next_number = pow(2, ceil(log(input_image.rows()) / log(2)));
					row_value = next_number - input_image.rows();
					col_value = next_number - input_image.cols();
				}
				else
				{
					next_number = pow(2, ceil(log(input_image.cols()) / log(2)));
					row_value = next_number - input_image.rows();
					col_value = next_number - input_image.cols();
				}

				const SDoublePlane temp_plane(input_image.rows() + row_value, input_image.cols() + col_value);
				for (int i = 0; i < input_image.rows(); i++)
				{
					for (int j = 0; j < input_image.cols(); j++)
					{
						temp_plane[i + row_value][j + col_value] = input_image[i][j];
					}
				}

				fft(temp_plane, real_part, imag_part);
				SDoublePlane magnitude_matrix(real_part.rows() + row_value, imag_part.cols() + col_value);

				magnitude_matrix = mark_image(temp_plane, N);

				SImageIO::write_png_file(outputFile.c_str(), magnitude_matrix, magnitude_matrix, magnitude_matrix);
				cout << "The file has been generated with the name " << outputFile.c_str() << " " << endl;
			}
			else if (op == "check")
			{
				cout << "Now when we have added the watermark, lets check if it is present or not!" << endl;
				check_image(input_image, N);
			}
			else
				throw string("Bad operation!");
		}
		else
			throw string("Bad part!");
	}
	catch (const string &err)
	{
		cerr << "Error: " << err << endl;
	}
}

// Task 1.1.
SDoublePlane fft_magnitude(const SDoublePlane &fft_real, const SDoublePlane &fft_imag)
{
	// TODO
	// Create an SDoublePlain for storing the magnitude values
	SDoublePlane magnitude(fft_real.rows(), fft_imag.cols());

	// Get start address pointers.
	double *magnitude_ptr = magnitude[0];
	double *real_part_data = fft_real[0];
	double *imag_part_data = fft_imag[0];

	int sz = fft_real.rows() * fft_imag.cols();

	// Calculating magnitude.
	for (int i = 0; i < sz; i++)
	{
		magnitude_ptr[i] = log(sqrt(real_part_data[i] * real_part_data[i] + imag_part_data[i] * imag_part_data[i]));
	}

	return magnitude;
}

// Task 1.2.
SDoublePlane remove_interference(const SDoublePlane &fft_real, const SDoublePlane &fft_imag)
{

	//TODO
	SDoublePlane output_real(fft_real.rows(), fft_imag.cols());
	SDoublePlane magnitude(fft_real.rows(), fft_imag.cols());

	int j, k;

	// Removing from the Spectrum.
	for (j = 156; j < 161; j++)
	{
		for (k = 0; k < fft_real.cols(); k++)
		{
			fft_real[j][k] = 0;
			fft_imag[j][k] = 0;
		}
	}

	// Removing from the Spectrum.
	for (j = 352; j < 357; j++)
	{
		for (k = 0; k < fft_real.cols(); k++)
		{
			fft_real[j][k] = 0;
			fft_imag[j][k] = 0;
		}
	}

	for (j = 0; j < fft_real.rows(); j++)
	{
		for (k = 0; k < fft_real.cols(); k++)
		{
			fft_real[j][k] = fft_real[j][k] * 4.40;
			fft_imag[j][k] = fft_imag[j][k] * 4.40;
		}
	}

	fft_real[255][255] = fft_real[255][255] * 1.1;
	fft_imag[255][255] = fft_imag[255][255] * 1.1;

	double *magnitude_ptr = magnitude[0];
	double *real_part_data = fft_real[0];
	double *imag_part_data = fft_imag[0];

	int sz = fft_real.rows() * fft_imag.cols();

	// Calculating magnitude to generate spectrum of no noise image.
	for (int i = 0; i < sz; i++)
	{
		magnitude_ptr[i] = log(sqrt(real_part_data[i] * real_part_data[i] + imag_part_data[i] * imag_part_data[i]));
	}

	// Writing Image output.
	SImageIO::write_png_file("updated_spectrum.png", magnitude, magnitude, magnitude);
	ifft(fft_real, fft_imag, output_real);

	// Last minute Image adjustments goes here.
	// for( j = 0 ; j < output_real.rows() ; j++){
	// 	for(  k = 0 ; k < output_real.rows() ; k++){
	// 		output_real[j][k] = output_real[j][k];
	// 	}
	// }

	return output_real;
}
//Function to watermark the image with the N vector.
SDoublePlane mark_image(const SDoublePlane &temp_plane, int N)
{
	SDoublePlane new_plane, real_image_part, imaginary_part, real_image_part_dash, imaginary_part_dash;
	//Initialize the seed function for random variable
	srand(N);
	int random = rand();
	char binary_vector[16];
	float radius = 0.0;
	double alpha = 10;

	//Define the vector for the value generated by converting it into 32 bit long binary(char array)
	std::string binary = std::bitset<16>(rand()).to_string();
	strcpy(binary_vector, binary.c_str());
	long long n = atoll(binary_vector);
	// Create a grey scale image which will be used used to create the plane
	SImageIO::write_png_file("generatedImage.png", temp_plane, temp_plane, temp_plane);
	SDoublePlane input_image = SImageIO::read_png_file("generatedImage.png");
	SDoublePlane input_image1 = SImageIO::read_png_file("generatedImage.png");

	//Convert the image into real and imaginary part

	fft(input_image, real_image_part, imaginary_part);
	fft(input_image1, real_image_part_dash, imaginary_part_dash);

	// Find the mid point of the image
	int mid_point = input_image1.rows() / 2;

	std::vector<double> arr;

	//Find points on the circle. This circle will be 16 point cirlce
	double num = 16.0f;
	double angle = 360.0f / (float)num;

	double x[16];
	double y[16];
	double val;

	val = PI / 180.0;

	double r = mid_point / 8, a = mid_point, b = mid_point;

	float temp_angle = 0.0f;

	for (int i = 0; i < num; i++)
	{
		x[i] = a + (r * cos((temp_angle)*val));
		y[i] = b + (r * sin((temp_angle)*val));

		temp_angle = temp_angle + angle;
	}
	//Implementing the formula R'(x,y) = R(x,y) + alpha * |R(x,y)| * Vi

	for (int i = 0; i < num; i++)
	{
		real_image_part_dash[int(x[i])][int(y[i])] = real_image_part[int(x[i])][int(y[i])] + alpha * real_image_part[int(x[i])][int(y[i])] * int(n % 10);
		arr.push_back(real_image_part_dash[int(x[i])][int(y[i])]);
		n = n / 10;
	}

	ifft(real_image_part_dash, imaginary_part, new_plane);

	double max = new_plane[0][0];
	double min = new_plane[0][0];

	//Find the min and max from the distribution
	for (int row = 0; row < new_plane.rows(); row++)
	{
		for (int col = 0; col < new_plane.cols(); col++)
			if (min > new_plane[row][col])
			{
				min = new_plane[row][col];
			}
			else if (max < new_plane[row][col])
			{
				max = new_plane[row][col];
			}
	}

	// Normalize the data points
	for (int i = 0; i < new_plane.rows(); i++)
	{
		for (int j = 0; j < new_plane.cols(); j++)
		{

			new_plane[i][j] = (new_plane[i][j] - min) / (max - min);
			new_plane[i][j] *= 255;
		}
	}

	return new_plane;
}

void check_image(const SDoublePlane &input_image, int N)
{

	std::vector<double> v, c;
	char temp[16];

	srand(N);
	int random = rand();

	std::string binary = std::bitset<16>(rand()).to_string();
	strcpy(temp, binary.c_str());
	long long vector_number = atoll(temp);

	int next_number = 0;
	int col_value = 0;
	int row_value = 0;

	//Check if the image is square or not!
	if (input_image.rows() > input_image.cols())
	{
		next_number = pow(2, ceil(log(input_image.rows()) / log(2)));
		row_value = next_number - input_image.rows();
		col_value = next_number - input_image.cols();
	}
	else
	{
		next_number = pow(2, ceil(log(input_image.cols()) / log(2)));
		row_value = next_number - input_image.rows();
		col_value = next_number - input_image.cols();
	}
	// Normalizing the rows and colums with the difference or rows and cols
	int new_rows = input_image.rows() + row_value;
	int new_cols = input_image.cols() + col_value;
	SDoublePlane temp_plane(new_rows, new_cols);
	SDoublePlane real_part(new_rows, new_cols);
	SDoublePlane imaginary_part(new_rows, new_cols);

	//Scale the image if not scaled
	for (int i = 0; i < input_image.rows(); i++)
	{
		for (int j = 0; j < input_image.cols(); j++)
		{
			temp_plane[i + row_value][j + col_value] = input_image[i][j];
		}
	}

	fft(temp_plane, real_part, imaginary_part);

	int mid_point = real_part.rows() / 2;
	double num = 16.0f;
	double angle = 360.0f / (float)num;

	double x[16];
	double y[16];
	double val;

	val = PI / 180.0;

	double r = mid_point / 8, a = mid_point, b = mid_point;

	float temp_angle = 0.0f;

	//Calculate the different points on the circle
	for (int i = 0; i < num; i++)
	{
		x[i] = a + (r * cos((temp_angle)*val));
		y[i] = b + (r * sin((temp_angle)*val));

		temp_angle = temp_angle + angle;
	}

	for (int i = 0; i < num; i++)
	{
		c.push_back(real_part[int(x[i])][int(y[i])]);
	}

	//Simply copying the values from a binary integer to the vector
	int array_value[16];
	for (int x = 0; x <= c.size(); x++)
	{
		array_value[x] = int(vector_number % 10);

		vector_number = vector_number / 10;
	}
	for (int x = c.size() - 1; x >= 0; x--)
		v.push_back(array_value[x]);

	int n = c.size();

	//Function call to correlationCoefficient.
	double precision_value = correlationCoefficient(v, c, n);
	if (abs(precision_value) >= 0.3)
	{
		cout << "There is a water mark that is present with the given N value :)";
	}
	else
	{
		cout << "We were not able to find the water mark in the image irrespective it was present or not :(";
	}
}

float correlationCoefficient(vector<double> X, vector<double> Y, int n)
{

	double sum_X = 0.0, sum_Y = 0.0, sum_XY = 0.0;
	double squareSum_X = 0.0, squareSum_Y = 0.0;

	for (int i = 0; i < n; i++)
	{
		// sum of elements of array X.
		sum_X = sum_X + X[i];

		// sum of elements of array Y.
		sum_Y = sum_Y + Y[i];

		// sum of X[i] * Y[i].
		sum_XY = sum_XY + X[i] * Y[i];

		// sum of square of array elements.
		squareSum_X = squareSum_X + X[i] * X[i];
		squareSum_Y = squareSum_Y + Y[i] * Y[i];
	}

	// use formula for calculating correlation coefficient.
	double correlation = (double)(n * sum_XY - sum_X * sum_Y) / sqrt((n * squareSum_X - sum_X * sum_X) * (n * squareSum_Y - sum_Y * sum_Y));

	return correlation;
}
