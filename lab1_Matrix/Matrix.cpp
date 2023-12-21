#include "Matrix.h"
#include <exception>
#include <algorithm>

using namespace std;

//theta(m*n) + theta(m) = O(n^2)
Matrix::Matrix(int nrLines, int nrCols) {
	   
	lines = nrLines;
    cols = nrCols;

    rowsPointers = new int[lines + 1];
    nonZeroElems = new int[lines];
    for(int index = 0; index < lines; index++)
    {
        nonZeroElems[index] = 0;
        rowsPointers[index] = 0;
    }
    rowsPointers[lines] = 0;

    colsIndices = new int[lines * cols];
    values = new TElem[lines * cols];
    for(int index = 0; index < lines * cols; index++)
    {
        colsIndices[index] = -1;
        values[index] = NULL_TELEM;
    }
}

//O(n^2)
void Matrix::resize(int new_nr_lines, int new_nr_cols) {

    int new_lines = new_nr_lines;
    int new_cols = new_nr_cols;

    int* new_pointers_to_rows = new int[new_lines + 1];
    int non_zero_count = 0;
    int last_position = 0;

    for(int index = 0; index < lines; index++)
    {
        non_zero_count += nonZeroElems[index];
        new_pointers_to_rows[index] = last_position;
        last_position += nonZeroElems[index];
    }

    new_pointers_to_rows[lines] = last_position;
    for(int index = lines + 1; index <= new_lines; index++)
        new_pointers_to_rows[index] = last_position;

    /*
    for(int index = 0; index <= lines; index++)
    {
        new_pointers_to_rows[index] = rowsPointers[index];
    }

    for(int index = lines + 1; index <= new_lines; index++)
        new_pointers_to_rows[index] = size;
    */

    delete[] rowsPointers;
    rowsPointers = new_pointers_to_rows;

    int* new_non_zero_elems = new int[new_lines];
    for(int index = 0; index < lines; index++)
        new_non_zero_elems[index] = nonZeroElems[index];

    for(int index = lines; index < new_lines; index++)
        new_non_zero_elems[index] = 0;

    delete[] nonZeroElems;
    nonZeroElems = new_non_zero_elems;

    int new_size = new_lines * new_cols;
    int* new_cols_indices = new int[new_size];
    auto* new_values = new TElem[new_size];

    for(int index = 0; index < new_size; index++)
    {
        new_cols_indices[index] = -1;
        new_values[index] = NULL_TELEM;
    }
    int pos = 0;
    for(int index = 0; index < lines; index++)
    {
        for(int index2 = rowsPointers[index]; index2 < rowsPointers[index + 1]; index2++)
        {
            int col = colsIndices[index2];
            new_cols_indices[index * new_cols + col] = col;
            new_values[index * new_cols + col] = values[index2];
            pos++;
        }
    }

    /*
    int position = 0;
    for(int index = 0; index < new_lines; index++)
    {
        for(int index2 = 0; index2 < new_cols; index2++)
        {
            new_cols_indices[position] = colsIndices[position];
            new_values[position] = values[position];
            position++;
        }
    }*/
    delete[] colsIndices;
    colsIndices = new_cols_indices;
    delete[] values;
    values = new_values;

    lines = new_lines;
    cols = new_cols;
    size = new_size;
}


int Matrix::nrLines() const {
	return lines;
}


int Matrix::nrColumns() const {
	return cols;
}

//worst case O(n) best case O(1)
TElem Matrix::element(int i, int j) const {
	if (i >= lines || j >= cols || i < 0 || j < 0)
    {
        throw exception();
    }
    int current_row = -1;
    for(int row_index = rowsPointers[i]; row_index <= rowsPointers[i + 1]; row_index++)
    {
        if(colsIndices[row_index] == j)
        {
            current_row = row_index;
            break;
        }
    }
    if(current_row == -1)
        return NULL_TELEM;
    return values[current_row];
}

//O(n)
TElem Matrix::modify(int i, int j, TElem e) {
	if(i >= lines || j >= cols || i < 0 || j < 0)
        throw exception();
    int current_row = -1;
    for(int row_index = rowsPointers[i]; row_index <= rowsPointers[i + 1]; row_index++)
    {
        if(colsIndices[row_index] == j)
        {
            current_row = row_index;
            break;
        } else if(colsIndices[row_index] > j)
        {
            current_row = row_index;
            break;
        }
    }
    if (current_row == -1 && e == NULL_TELEM)
        return NULL_TELEM;
    else if(current_row == -1 && e != NULL_TELEM)
    {
        if(nonZeroElems[i] + 1 >= cols)
            resize(lines, cols + cols/2);

        for (int row_index = rowsPointers[i + 1]; row_index >= rowsPointers[i]; row_index--)
        {
            if(colsIndices[row_index] > j) {
                colsIndices[row_index + 1] = colsIndices[row_index];
                values[row_index + 1] = values[row_index];
            } else {
                colsIndices[row_index] = j;
                values[row_index] = e;
                break;
            }
        }
        return NULL_TELEM;

    } else if(current_row != -1 && e != NULL_TELEM)
    {
        TElem prev_value = values[current_row];
        values[current_row] = e;

        nonZeroElems[i]++;
        /*rowsPointers[i]++;
        colsIndices[current_row] = j;*/
        return prev_value;
    } else {
        TElem prev_value = values[current_row];
        int position_to_delete = current_row;
        for(int row_index = position_to_delete; row_index < rowsPointers[i + 1] - 1; row_index++)
        {
            colsIndices[row_index] = colsIndices[row_index + 1];
            values[row_index] = values[row_index + 1];
        }
        rowsPointers[i + 1]--;
        nonZeroElems[i]--;

        return prev_value;
    }
}

Matrix::~Matrix(){
    delete[] rowsPointers;
    delete[] nonZeroElems;
    delete[] colsIndices;
    delete[] values;
}

void Matrix::transpose() {
    Matrix transposed_matrix(cols, lines);
    /*int* col_counts = new int[cols];
    for(int index = 0; index < lines; index++)
    {
        for(int index2 = rowsPointers[index]; index2 < rowsPointers[index + 1]; index2++) {
            int col = colsIndices[index2];
            transposed_matrix.values[col * lines + col_counts[col]] = values[index2];
            transposed_matrix.colsIndices[col * lines + col_counts[col]] = colsIndices[index2];
            col_counts++;
        }
    }

    for(int index = 0; index < cols; index++) {
        this->colsIndices = transposed_matrix.colsIndices;
        this->values = transposed_matrix.values;
    }*/
    /*for (int index = 0; index < lines; index++) {
        for (int index2 = 0; index2 < cols; index2++) {
            if(element(index, index2) != NULL_TELEM && transposed_matrix.element())
            int col = colsIndices[index2];
            TElem val = values[index2];
            transposed_matrix.modify(col, index, val);
        }
    }*/

    for(int i = 0; i < lines; i++)
        for(int row_index = rowsPointers[i]; row_index <= rowsPointers[i + 1]; row_index++)
        {
            transposed_matrix.modify(colsIndices[row_index], row_index, element(row_index, colsIndices[row_index]));
            //transposed_matrix.modify(row_index, colsIndices[row_index], element(colsIndices[row_index], row_index));
        }


    for (int index = 0; index < lines; index++) {
        for(int row_index = transposed_matrix.rowsPointers[index]; row_index <= transposed_matrix.rowsPointers[index + 1]; row_index++) {
            TElem val = transposed_matrix.element(index, row_index);
            modify(index, row_index, val);
        }
    }
}



