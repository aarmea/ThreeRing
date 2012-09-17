#ifndef TABLE_H
#define TABLE_H

#include <QObject>
#include <stdint.h>

typedef uintptr_t table_size_type;

template <class T> class Table : public QObject {
public:
    //Constructors, Assignments, Destructors
    Table();
    Table(table_size_type numRows, table_size_type numCols, const T& fillValue);
    Table(const Table& oldTable);
    const Table<T>& operator=(const Table<T>& oldTable);
    ~Table();

    //Accessors
    const T& get(table_size_type row, table_size_type col) const;
    table_size_type numRows() const;
    table_size_type numColumns() const;

    //Modifiers
    void set(table_size_type row, table_size_type col, const T& fillValue);
    void resize(table_size_type newRows, table_size_type newCols, const T& fillValue = T());
    void pop_back_row();
    void pop_back_column();

private:
    //Private member functions
    void copy(const Table& oldTable);

    //Representation
    table_size_type rows_, cols_;
    T **values_;
};

// Begin implementations

template <class T> Table<T>::Table()
{
    rows_ = 0;
    cols_ = 0;
    values_ = NULL;
}

template <class T> Table<T>::Table(table_size_type numRows, table_size_type numCols, const T& fillValue)
{
    rows_ = numRows;
    cols_ = numCols;
    values_ = new T*[rows_];
    for(int i = 0; i < rows_; i++){
        values_[i] = new T[cols_];
        for(int j = 0; j < cols_; j++){
            values_[i][j] = fillValue;
        }
    }
}

template <class T> Table<T>::Table(const Table& oldTable)
{
    copy(oldTable);
}

template <class T> void Table<T>::copy(const Table& oldTable)
{
    rows_ = oldTable.rows_;
    cols_ = oldTable.cols_; //this might not work
    values_ = new T*[rows_];
    for(int i = 0; i < rows_; i++){
        values_[i] = new T[cols_];
        for(int j = 0; j < cols_; j++){
            //maybe use memcpy instead
            values_[i][j] = oldTable.values_[i][j];
            //this->set(i,j, oldTable.get(i,j));
        }
    }
}

template <class T> const Table<T>& Table<T>::operator=(const Table<T>& oldTable)
{
    //TODO: handle self-assignment!
    this->~Table();
    this->copy(oldTable);
    return *this;
}

template <class T> Table<T>::~Table()
{
    for(int i = 0; i < rows_; i++){
        delete[] values_[i];
    }
    delete[] values_;
    values_ = NULL;
}

template <class T> table_size_type Table<T>::numRows() const {
    return rows_;
}

template <class T> table_size_type Table<T>::numColumns() const {
    return cols_;
}

template <class T> const T& Table<T>::get(table_size_type row, table_size_type col) const
{
    return values_[row][col];
}

template <class T> void Table<T>::set(table_size_type row, table_size_type col, const T& fillValue)
{
    values_[row][col] = fillValue;
}

template <class T> void Table<T>::resize(table_size_type newRows, table_size_type newCols, const T& fillValue)
{
    T** newValues = new T*[newRows];
    table_size_type i,j;
    for (i = 0; i < newRows; ++i) {
        newValues[i] = new T[newCols];
        for (j = 0; j < newCols; j++) {
            newValues[i][j] = (i<rows_ && j<cols_) ? values_[i][j] : fillValue;
        }
    }
    for (i = 0; i < rows_; ++i) {
        delete[] values_[i];
    }
    delete[] values_;

    rows_ = newRows;
    cols_ = newCols;
    values_ = newValues;
}

template <class T> void Table<T>::pop_back_row()
{
    resize(rows_-1, cols_);
}

template <class T> void Table<T>::pop_back_column()
{
    resize(rows_, cols_-1);
}

#endif // TABLE_H
