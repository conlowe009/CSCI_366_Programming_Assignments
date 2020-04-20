//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <math.h>
#include "BitArray2D.hpp"

BitArray2D::BitArray2D(unsigned int rows, unsigned int columns) {
    /*Use function parameters*/
    this->rows = rows;
    this->columns = columns;

    /*Throw an exception if improper array size parameters are passed in*/
    if (rows <= 0 || columns <= 0)
        throw BitArray2DException("Invalid Array Size");

    /* Uses function parameters to get size of array and divides size of array
     * by the size of a char (8 bits) then rounds that number up
     * to the nearest whole integer to use in the calloc call */
    int char_size = 8; //8 bits in a byte, char is 1 byte
    double array_dim = rows*columns;
    double size = array_dim/char_size;
    int num = ceil(size);

    /*Initialize array*/
    array = (char*)calloc(num, sizeof(char));
}


BitArray2D::~BitArray2D() { /*Deconstructor */ }


bool BitArray2D::get(unsigned int row, unsigned int column){
    // check array bounds
    if (row < 0 or column < 0 || row >= rows or column >= columns)
        throw BitArray2DException("Invalid Array Coordinates");

    // get the element
   return get_bit_elem(array, columns, row, column);
}



void BitArray2D::set(unsigned int row, unsigned int column){
   // check array bounds
    if (row < 0 or column < 0 || row >= rows or column >= columns)
        throw BitArray2DException("Invalid Array Coordinates");

   // set the element
   set_bit_elem(array, columns, row, column);
}
