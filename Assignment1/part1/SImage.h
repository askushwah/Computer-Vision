#ifndef __SIMAGE_H__
#define __SIMAGE_H__

#include "DTwoDimArray.h"
#include <string.h>


// A very simple image class.
//
class SDoublePlane : public _DTwoDimArray<double>
{
 public:
  SDoublePlane() { }
  SDoublePlane(int _rows, int _cols)  : _DTwoDimArray<double>(_rows, _cols)
    { 
      memset(data_ptr(), 0, sizeof(double) * rows() * cols());
      // be nice and initialize plane to all 0's
      // if (rows() == cols())
      //   memset(data_ptr(), 0, sizeof(double) * rows() * cols());
      // else if(rows() > cols()){
      //   //if rows > cols, change rows = next 2^i form.
      //   int next_number = pow(2,ceil(log(rows())/log(2)));
      //   memset(data_ptr(), 0, sizeof(double) * next_number * next_number);
      // }else{
      //   //if cols > rows, change rows = next 2^i form.
      //   int next_number = pow(2,ceil(log(rows())/log(2)));
      //   memset(data_ptr(), 0, sizeof(double) * next_number * next_number);
      //}
    }
};

#endif
