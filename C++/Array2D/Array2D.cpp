#include "Array2D.h"


template <class T>
Array2D<T>::Array2D(size_t rows, size_t columns) : rowCount(rows), columnCount(columns) {
	array = new T[rows * columns]();	// create a zero-initialized array
}


template <class T>
Array2D<T>::~Array2D() {
	delete[] array;
}


template <class T>
T& Array2D<T>::at(size_t row, size_t column) {
	return (*this)[row][column];		// includes the bounds checks, but might add some overhead!
	//return array[row * rowCount + column];
}


template <class T>
size_t Array2D<T>::rows() {
	return rowCount;
}


template <class T>
size_t Array2D<T>::columns() {
	return columnCount;
}


template <class T>
Row<T> Array2D<T>::operator[](size_t row) {
	if (row >= rowCount) {
		std::cerr << "Array out of bounds: row " << row << " > " << rowCount-1;
		abort();
	}

	return Row<T>(&array[row * rowCount], columnCount);
}


template <class T>
std::string Array2D<T>::toString() {
	std::ostringstream result;
	for (size_t row = 0;  row < rowCount;  row++) {
		for (size_t col = 0;  col < columnCount; col++) {
			result << at(row, col) << ", ";
		}
		result << "\n";
	}

	return result.str();
}
