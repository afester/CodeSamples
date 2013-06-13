
// forward declaration of Array2D
template<class T> class Array2D;

// One row of a 2D array. Instances of this class can only be
// returned by operator[] of Array2D.
template <class T>
class Row {
	T* rowData;

	// construction only allowed by Array2D
	Row() {};

	Row(T* data) {
		rowData = data;
	}

public:

    T& operator[](int column) {
    	return rowData[column];
    }

	friend class Array2D<T>;
};


template <class T>
class Array2D {

    T *array;
    int rowCount;

    /**
     * Copy constructor.
     */
    Array2D(const Array2D& other) {
    }

    /**
     * Copy assignment operator.
     */
    Array2D& operator=(const Array2D& other) {
   }

public:

    /**
     * Creates a two-dimensional array.
     *
     * @param rows		The number of rows for the array.
     * @param columns 	The number of columns for the array.
     */
    Array2D(int rows, int columns) {
        rowCount = rows;
        array = new T[rows * columns]();	// create a zero-initialized array
    }

    /**
     * Destroys the array.
     */
    ~Array2D() {
    	at(2,2) = 53;
        delete[] array;
    }





    /**
     * @param row	The row of the element
     * @param column	The column of the element
     *
     * @return a reference to the array element at a specific row and column
     */
    T& at(int row, int column) {
        return array[row * rowCount + column];
    }


    /**
     * @param row The row to return.
     *
     * @return A specific row of the array.
     */
    Row<T> operator[](int row) {
    	return Row<T>(&array[row * rowCount]);
    }
};
