#ifndef __ARRAY_2D_H__
#define __ARRAY_2D_H__

#include <iostream>
#include <cstddef>
#include <stdlib.h>
#include <sstream>

template <class T>
class Array2D {

    T *array;
    size_t rowCount;
    size_t columnCount;

    // One row of a 2D array. Instances of this class can only be
    // returned by operator[] of Array2D.
    class Row {
    	T* rowData;
        size_t columnCount;

    	// construction only allowed by Array2D
    	Row();
    	Row(T* data, size_t columns);

    public:

        /**
         * @param column The column to return.
         *
         * @return A specific column of this row.
         */
        T& operator[](size_t column);

    	friend class Array2D<T>;
    };


    /**
     * Copy constructor.
     */
    Array2D(const Array2D& other) : array(0), rowCount(0), columnCount(0) {
    }

    /**
     * Copy assignment operator.
     */
    Array2D& operator=(const Array2D& other) {
    	return *this;
    }

public:

    /**
     * Creates a two-dimensional array.
     *
     * @param rows		The number of rows for the array.
     * @param columns 	The number of columns for the array.
     */
    Array2D(size_t rows, size_t columns);


    /**
     * Destroys the array.
     */
    ~Array2D();


    /**
     * @param row	The row of the element
     * @param column	The column of the element
     *
     * @return a reference to the array element at a specific row and column
     */
    T& at(size_t row, size_t column);


    /**
     * @return The number of rows in the array.
     */
    size_t rows();


    /**
     * @return The number of columms in the array.
     */
    size_t columns();


    /**
     * @param row The row to return.
     *
     * @return A specific row of the array.
     */
    Array2D<T>::Row operator[](size_t row);


    /**
     * @return A string representation of the array
     */
    std::string toString();
};

#endif
