// B657 assignment 2 skeleton code
//
// Compile with: "make"
//
// See assignment handout for command line and project specifications.

//Link to the header file
#include "CImg.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <Sift.h>
#include <armadillo>
#include <sstream>

//Use the cimg namespace to access functions easily
using namespace cimg_library;
using namespace arma;
using namespace std;

template <typename T>
string ToString(T val)
{
    stringstream stream;
    stream << val;
    return stream.str();
}

void draw_descriptor_image(CImg<double> image, const vector<SiftDescriptor> descriptors, const char *filename)
{
  for (unsigned int i = 0; i < descriptors.size(); i++)
  {
    int tx1 = 0, ty1 = 0, tx2 = 0, ty2 = 0;
    double color_point[] = {255.0, 255.0, 0};
    for (int x = -2; x < 3; x++)
      for (int y = -2; y < 3; y++)
        if (x == 0 || y == 0)
          for (int c = 0; c < 3; c++)
          {
            //Find if coordinates are in workspace to draw crosshair
            tx1 = (descriptors[i].col + y - 1);
            ty1 = (descriptors[i].row + x - 1);
            if (tx1 >= 0 && tx1 < image.width() && ty1 >= 0 && ty1 < image.height())
              image(tx1, ty1, 0, c) = color_point[c];
          }
  }
  image.get_normalize(0, 255).save(filename);
}

// Create 8*8 matrix
mat Matrix_right(int source_image[2][4], int destination_image[2][4])
{
  mat Matrix_right;
  Matrix_right << source_image[0][0] << source_image[1][0] << 1 << 0 << 0 << 0 << -(source_image[0][0] * destination_image[0][0]) << -(source_image[1][0] * destination_image[0][0]) << endr

               << 0 << 0 << 0 << source_image[0][0] << source_image[1][0] << 1 << -(source_image[0][0] * destination_image[1][0]) << -(source_image[1][0] * destination_image[1][0]) << endr

               << source_image[0][1] << source_image[1][1] << 1 << 0 << 0 << 0 << -(source_image[0][1] * destination_image[0][1]) << -(source_image[1][1] * destination_image[0][1]) << endr

               << 0 << 0 << 0 << source_image[0][1] << source_image[1][1] << 1 << -(source_image[0][1] * destination_image[1][1]) << -(source_image[1][1] * destination_image[1][1]) << endr

               << source_image[0][2] << source_image[1][2] << 1 << 0 << 0 << 0 << -(source_image[0][2] * destination_image[0][2]) << -(source_image[1][2] * destination_image[0][2]) << endr

               << 0 << 0 << 0 << source_image[0][2] << source_image[1][2] << 1 << -(source_image[0][2] * destination_image[1][2]) << -(source_image[1][2] * destination_image[1][2]) << endr

               << source_image[0][3] << source_image[1][3] << 1 << 0 << 0 << 0 << -(source_image[0][3] * destination_image[0][3]) << -(source_image[1][3] * destination_image[0][3]) << endr

               << 0 << 0 << 0 << source_image[0][3] << source_image[1][3] << 1 << -(source_image[0][3] * destination_image[1][3]) << -(source_image[1][3] * destination_image[1][3]) << endr;

  return Matrix_right;
}

// Create 8*1 matrix
mat Matrix_left(int destination_image[2][4])
{
  mat Matrix_left;
  Matrix_left << destination_image[0][0] << endr
              << destination_image[1][0] << endr
              << destination_image[0][1] << endr
              << destination_image[1][1] << endr
              << destination_image[0][2] << endr
              << destination_image[1][2] << endr
              << destination_image[0][3] << endr
              << destination_image[1][3] << endr;

  return Matrix_left;
}

mat Transformation_martrix(mat unknown_param)
{
  mat Transformation_matrix;
  Transformation_matrix << unknown_param[0] << unknown_param[1] << unknown_param[2] << endr
                        << unknown_param[3] << unknown_param[4] << unknown_param[5] << endr
                        << unknown_param[6] << unknown_param[7] << 1 << endr;
  return Transformation_matrix;
}


int main(int argc, char **argv)
{
  try
  {
    (void)argc;
    /*
      TEST CODE - STARTS
    */
    string part = argv[1];

    if (part == "part1")
    {

      int x;
      do
      {
        cout << "**************************** Welcome to part 1 of the Assignment 2: ****************************\n\n";
        cout << "Please choose from the options below:\n";
        cout << "1. Warping the lincoln image.\n";
        cout << "2. Solve the homography using four point of a book image.\n";
        cout << "3. Project a rectangular image onto the billboard (Before you continue this part, make sure you have image in present /images/part1/ folder).\n";
        cout << "4. Exit\n\n";
        cout << "Please choose the number between 1-4 to continue with the corresponding section\n";
        cin >> x;
        if (x == 1)
        {
          /********************** PART 1 **************************/
          //Input the image from folder
          CImg<double> img("images/part1/lincoln.png");
          CImg<double> img_black(img.width(), img.height(), 1, 3, 0);
          img_black.get_normalize(0, 1);
          mat Transformation_Martrix;
          // Transformation Matrix
          Transformation_Martrix << 0.907 << 0.258 << -182 << endr
                                 << -0.153 << 1.44 << 58 << endr
                                 << -0.000306 << 0.000731 << 1 << endr;

          mat temp;
          //Loop over all the pixels and perform the tranformation
          cimg_forXYC(img, x, y, c)
          {
            temp << x << endr
                 << y << endr
                 << 1 << endr;
            //Inverse warping
            mat B = inv(Transformation_Martrix) * temp;
            // Condition to ignore segmentation fault
            if (B[0] > 0 and B[1] > 0 and B[0] / B[2] < img.width() and B[1] / B[2] < img.height())
              img_black(x, y, c) = img(B[0] / B[2], B[1] / B[2], c);
            else
              img_black(x, y, c) = 0; 
          }
          //output the image
          img_black.get_normalize(0, 255).save("lincolnwarped.png");
          cout << "\nThe lincolnwarped.png image has been generated\n\n";
        }
        if (x == 2)
        {
          /********************** PART 2 **************************/

          CImg<double> img_transform("images/part1/book1.jpg");
          CImg<double> img_black_book(img_transform.width(), img_transform.height(), 1, 3, 0);
          img_black_book.get_normalize(0, 1);
          //Coordinates for the image that has to be warped
          int destination_image[2][4] = {{318, 534, 316, 73},
                                         {256, 372, 670, 473}};
          //Coordinates for the source image
          int source_image[2][4] = {{141, 480, 493, 64},
                                    {131, 159, 630, 601}};

          mat unknown_param;
          // Creating the unkown paramenter a1,a2.......a8.
          unknown_param = inv(Matrix_right(source_image, destination_image)) * Matrix_left(destination_image);
          // Homography matrix
          mat Transformation_matrix = Transformation_martrix(unknown_param);

          mat temp_part2;
          //Loop over all the pixels and perform the tranformation
          cimg_forXYC(img_transform, x, y, c)
          {
            temp_part2 << x << endr
                       << y << endr
                       << 1 << endr;
            //Inverse warping
            mat B = inv(Transformation_matrix) * temp_part2;
            // Condition to ignore segmentation fault
            if (B[0] / B[2] > 0 and B[1] / B[2] > 0 and B[0] / B[2] < img_transform.width() and B[1] / B[2] < img_transform.height())
              img_black_book(x, y, c) = img_transform(B[0] / B[2], B[1] / B[2], c);
          }
          cout << "\n The homography matrix is : \n";
          cout << Transformation_matrix;
          img_black_book.get_normalize(0, 255).save("book_result.png");
          cout << "\nThe book_result.png image has been generated\n\n";
        }
        if (x == 3)
        {
          /********************** PART 3 **************************/
          string input_image = "images/part1/";
          input_image += argv[2];
          CImg<double> img_rectangle(input_image.c_str());
          CImg<double> img_billboard1("images/part1/billboard1.jpg");
          CImg<double> img_billboard2("images/part1/billboard2.png");
          CImg<double> img_billboard3("images/part1/billboard3.jpg");
          vector< CImg<double> > billboard_vector;
          //adding an item to a vector
          billboard_vector.push_back(img_billboard1);
          billboard_vector.push_back(img_billboard2);
          billboard_vector.push_back(img_billboard3);
          
          //Coordinates for all the 3 billboards
          int destination_billboard[][2][4] = {{{100, 533, 533, 100},
                                                {60, 60, 206, 206}},
                                               {{177, 1109, 1126, 145},
                                                {54, 257, 704, 626}},
                                               {{617, 1258, 1260, 611},
                                                {285, 261, 606, 606}}};
          //Coordinates of the input image
          int source_image[2][4] = {{0, img_rectangle.width(), img_rectangle.width(), 0},
                                    {0, 0, img_rectangle.height(), img_rectangle.height()}};

          for (int i = 0; i < 3; i++)
          {
            mat unknown_param;
            unknown_param = inv(Matrix_right(source_image, destination_billboard[i])) * Matrix_left(destination_billboard[i]);

            mat Transformation_matrix = Transformation_martrix(unknown_param);

            mat temp_part2;
            //Loop over all the pixels and perform the tranformations
            cimg_forXYC(img_rectangle, x, y, c)
            {
              temp_part2 << x << endr
                         << y << endr
                         << 1 << endr;
              //Inverse warping
              mat B = inv(Transformation_matrix) * temp_part2;
              // Condition to ignore segmentation fault
              if (B[0] / B[2] > 0 and B[1] / B[2] > 0 and B[0] / B[2] < img_rectangle.width() and B[1] / B[2] < img_rectangle.height())
                billboard_vector[i](x, y, c) = img_rectangle(B[0] / B[2], B[1] / B[2], c);
            }
            //img_billboard1.display();
            //Saving the image after projecting the image onto the billboard
            string fileName = "syntheticbillboard" + ToString(i + 1) + ".png";
            billboard_vector[i].get_normalize(0, 255).save(fileName.c_str());
            cout << "\n"
                 << fileName << "Image has been generated";
          }cout << "\n";
        }
        if (x == 4){
            cout <<"\nExiting the program\n";
            break;
        }
      } while (0 < x <= 4);
    }
    else if (part == "part2")
    {

      // Blending`
    }
    else if (part == "part3")
    {
      // RANSAC
    }
    else if (part == "part4")
    {
      // Panorama
    }

    // feel free to add more conditions for other parts (e.g. more specific)
    //  parts, for debugging, etc.
  }
  catch (const string &err)
  {
    cerr << "Error: " << err << endl;
  }
}
