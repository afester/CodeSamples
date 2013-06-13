#include "Array2D.h"

template <class T>
Array2D<T>::Row::Row() : rowData(0), columnCount(0) {

}


template <class T>
Array2D<T>::Row::Row(T* data, size_t columns) : rowData(data), columnCount(columns) {
}


template <class T>
T& Array2D<T>::Row::operator[](size_t column) {

	if (column >= columnCount) {
		std::cerr << "Array out of bounds: column " << column << " > " << columnCount-1;
		abort();
	}

	return rowData[column];
}


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
typename Array2D<T>::Row Array2D<T>::operator[](size_t row) {
	if (row >= rowCount) {
		std::cerr << "Array out of bounds: row " << row << " > " << rowCount-1;
		abort();
	}

	return Array2D<T>::Row(&array[row * rowCount], columnCount);
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
