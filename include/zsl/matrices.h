/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/**
 * @file
 * @brief API header file for matrices in zscilib.
 *
 * This file contains the zscilib matrix APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_MATRICES_H_
#define ZEPHYR_INCLUDE_ZSL_MATRICES_H_

#include <zsl/zsl.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Represents a m x n matrix, with data stored in row-major order. */
struct zsl_mtx {
    /** The number of rows in the matrix (typically denoted as 'm'). */
    size_t sz_rows;
    /** The number of columns in the matrix (typically denoted as 'n'). */
    size_t sz_cols;
    /** The data assigned to the matrix, in row-major order (left to right). */
    zsl_real_t *data;
};

/** Macro to declare a matrix with static memory allocation. */
#define ZSL_MATRIX_DEF(name, m, n)\
  zsl_real_t name##_mtx[m*n] = { 0 };\
  struct zsl_mtx name = {\
      .sz_rows      = m,\
      .sz_cols      = n,\
      .data         = name##_mtx\
  }

/**
 * Function prototype called when populating a matrix via `zsl_mtx_init`.
 *
 * @param m     Pointer to the zsl_mtx to use.
 * @param i     The row number to write (0-based).
 * @param j     The column number to write (0-based).
 *
 * @return 0 on success, and non-zero error code on failure
 */
typedef int (*zsl_mtx_init_entry_fn_t)(struct zsl_mtx *m, size_t i, size_t j);

/**
 * @brief Assigns a zero-value to all entries in the matrix.
 *
 * @param m     Pointer to the zsl_mtx to use.
 * @param i     The row number to write (0-based).
 * @param j     The column number to write (0-based).
 *
 * @return 0 on success, and non-zero error code on failure
 */
int zsl_mtx_entry_fn_empty(struct zsl_mtx *m, size_t i, size_t j);

/**
 * @brief Sets the value to '1.0' if the entry is on the diagonal (row=col),
 * otherwise '0.0'.
 *
 * @param m     Pointer to the zsl_mtx to use.
 * @param i     The row number to write (0-based).
 * @param j     The column number to write (0-based).
 *
 * @return 0 on success, and non-zero error code on failure
 */
int zsl_mtx_entry_fn_diagonal(struct zsl_mtx *m, size_t i, size_t j);

/**
 * @brief Sets the value to a random number between -1.0 and 1.0.
 *
 * @param m     Pointer to the zsl_mtx to use.
 * @param i     The row number to write (0-based).
 * @param j     The column number to write (0-based).
 *
 * @return 0 on success, and non-zero error code on failure
 */
int zsl_mtx_entry_fn_random(struct zsl_mtx *m, size_t i, size_t j);

/**
 * Initialises matrix 'm' using the specified entry function to
 * assign values.
 *
 * @param m         Pointer to the zsl_mtx to use.
 * @param entry_fn  The zsl_mtx_init_entry_fn_t instance to call. If this
 *                  is set to NULL 'zsl_mtx_entry_fn_empty' will be called.
 *
 * @return 0 on success, and non-zero error code on failure
 */
int zsl_mtx_init(struct zsl_mtx *m, zsl_mtx_init_entry_fn_t entry_fn);

/**
 * @brief Converts an array of values into a matrix. The number of elements in
 *        array 'a' must match the number of elements in matrix 'm'
 *        (m.sz_rows * m.sz_cols). As such, 'm' should be a previously
 *        initialised matrix with appropriate values assigned to m.sz_rows
 *        and m.sz_cols. Assumes array values are in row-major order.
 *
 * @param m The matrix that the contents of array 'a' should be assigned to.
 *          The m.sz_rows and m.sz_cols dimensions must match the number of
 *          elements in 'a', meaning that the matrix should be initialised
 *          before being passed in to this function.
 * @param a Pointer to the array containing the values to assign to 'm' in
 *          row-major order (left-to-right, top-to-bottom). The array will be
 *          read m.sz_rows * m.sz_cols elements deep.
 *
 * @return 0 on success, and non-zero error code on failure
 */
int zsl_mtx_from_arr(struct zsl_mtx *m, zsl_real_t *a);

/**
 * @brief Gets a single value from the specified row (i) and column (j).
 *
 * @param m     Pointer to the zsl_mtx to use.
 * @param i     The row number to read (0-based).
 * @param j     The column number to read (0-based).
 * @param x     Pointer to where the value should be stored.
 *
 * @return  0 if everything executed correctly, or -EINVAL on an out of
 *          bounds error.
 */
int zsl_mtx_get(struct zsl_mtx *m, size_t i, size_t j, zsl_real_t *x);

/**
 * @brief Sets a single value at the specified row (i) and column (j).
 *
 * @param m     Pointer to the zsl_mtx to use.
 * @param i     The row number to update (0-based).
 * @param j     The column number to update (0-based).
 * @param x     The value to assign.
 *
 * @return  0 if everything executed correctly, or -EINVAL on an out of
 *          bounds error.
 */
int zsl_mtx_set(struct zsl_mtx *m, size_t i, size_t j, zsl_real_t x);

/**
 * @brief Gets the contents of row 'i' from matrix 'm', assigning the array
 *        of data to 'v'.
 *
 * @param m     Pointer to the zsl_mtx to use.
 * @param i     The row number to read (0-based).
 * @param v     Pointer to the array where the row vector should be written.
 *              Must be at least m->sz_cols elements long!
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_mtx_get_row(struct zsl_mtx *m, size_t i, zsl_real_t *v);

/**
 * @brief Sets the contents of row 'i' in matrix 'm', assigning the values
 *        found in array 'v'.
 *
 * @param m     Pointer to the zsl_mtx to use.
 * @param j     The row number to write to (0-based).
 * @param v     Pointer to the array where the row vector data is stored.
 *              Must be at least m->sz_cols elements long!
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_mtx_set_row(struct zsl_mtx *m, size_t i, zsl_real_t *v);

/**
 * @brief Gets the contents of column 'j' from matrix 'm', assigning the array
 *        of data to 'v'.
 *
 * @param m     Pointer to the zsl_mtx to use.
 * @param j     The column number to read (0-based).
 * @param v     Pointer to the array where the column vector should be written.
 *              Must be at least m->sz_rows elements long!
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_mtx_get_col(struct zsl_mtx *m, size_t j, zsl_real_t *v);

/**
 * @brief Sets the contents of column 'j' in matrix 'm', assigning the values
 *        found in array 'v'.
 *
 * @param m     Pointer to the zsl_mtx to use.
 * @param j     The column number to write to (0-based).
 * @param v     Pointer to the array where the column vector data is stored.
 *              Must be at least m->sz_rows elements long!
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_mtx_set_col(struct zsl_mtx *m, size_t j, zsl_real_t *v);

/**
 * @brief Adds matrices 'ma' and 'mb', assigning the output to 'mc'.
 *        Matrices 'ma', 'mb' and 'mc' must all be identically shaped.
 *
 * @param ma    Pointer to the first input zsl_mtx.
 * @param mb    Pointer to the second input zsl_mtx.
 * @param mc    Pointer to the output zsl_mtx.
 *
 * @return  0 if everything executed correctly, or -EINVAL if the three
 *          matrices are not all identically shaped.
 */
int zsl_mtx_add(struct zsl_mtx *ma, struct zsl_mtx *mb, struct zsl_mtx *mc);

/**
 * @brief Adds matrices 'ma' and 'mb', assigning the output to 'ma'.
 *        Matrices 'ma', and 'mb' must be identically shaped.
 *
 * @param ma    Pointer to the first input zsl_mtx. This matrix will be
 *              overwritten with the results of the addition operations!
 * @param mb    Pointer to the second input zsl_mtx.
 *
 * @warning     This function is destructive to 'ma'!
 *
 * @return  0 if everything executed correctly, or -EINVAL if the two input
 *          matrices are not identically shaped.
 */
int zsl_mtx_add_d(struct zsl_mtx *ma, struct zsl_mtx *mb);

/**
 * @brief Subtracts matrices 'mb' from 'ma', assigning the output to 'mc'.
 *        Matrices 'ma', 'mb' and 'mc' must all be identically shaped.
 *
 * @param ma    Pointer to the first input zsl_mtx.
 * @param mb    Pointer to the second input zsl_mtx.
 * @param mc    Pointer to the output zsl_mtx.
 *
 * @return  0 if everything executed correctly, or -EINVAL if the three
 *          matrices are not all identically shaped.
 */
int zsl_mtx_sub(struct zsl_mtx *ma, struct zsl_mtx *mb, struct zsl_mtx *mc);

/**
 * @brief Subtracts matrix 'mb' from 'ma', assigning the output to 'ma'.
 *        Matrices 'ma', and 'mb' must be identically shaped.
 *
 * @param ma    Pointer to the first input zsl_mtx. This matrix will be
 *              overwritten with the results of the subtraction operations!
 * @param mb    Pointer to the second input zsl_mtx.
 *
 * @warning     This function is destructive to 'ma'!
 *
 * @return  0 if everything executed correctly, or -EINVAL if the two input
 *          matrices are not identically shaped.
 */
int zsl_mtx_sub_d(struct zsl_mtx *ma, struct zsl_mtx *mb);

/**
 * @brief Multiplies matrix 'ma' by 'mb', assigning the output to 'mc'.
 *        Matrices 'ma' and 'mb' must be compatibly shaped, meaning that
 *        'ma' must have the same numbers of columns as there are rows
 *        in 'mb'.
 *
 * @param ma    Pointer to the first input zsl_mtx.
 * @param mb    Pointer to the second input zsl_mtx.
 * @param mc    Pointer to the output zsl_mtx.
 *
 * @return  0 if everything executed correctly, or -EINVAL if the input
 *          matrices are not compatibly shaped.
 */
int zsl_mtx_mult(struct zsl_mtx *ma, struct zsl_mtx *mb, struct zsl_mtx *mc);

/**
 * @brief Multiplies all elements in vector 'm' by scalar value 's'.
 *
 * @param m     Pointer to the zsl_mtz to adjust.
 * @param s     The scalar value to multiply elements in matrix 'm' with.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_mtx_scalar_mult(struct zsl_mtx *m, zsl_real_t s);

/**
 * @brief Transposes the matrix 'ma' into matrix 'mb'. Note that output
 *        matrix 'mb' must have 'ma->sz_rows' columns, and 'ma->sz_cols' rows.
 *
 * @param ma    Pointer to the input matrix to transpose.
 * @param mb    Pointer to the output zsl_mtz.
 *
 * @return  0 if everything executed correctly, or -EINVAL if ma and mb are
 *          not compatibly shaped.
 */
int zsl_mtx_trans(struct zsl_mtx *ma, struct zsl_mtx *mb);

int zsl_mtx_minor(struct zsl_mtx *m, size_t i, size_t j, zsl_real_t *minor);
int zsl_mtx_cofactor(struct zsl_mtx *m, size_t i, size_t j, zsl_real_t *c);
int zsl_mtx_adjoint(struct zsl_mtx *m, size_t i, size_t j, zsl_real_t *a);
int zsl_mtx_deter(struct zsl_mtx *m, size_t i, size_t j, zsl_real_t *d);
int zsl_mtx_deter_recur(struct zsl_mtx *m, zsl_real_t *d);
int zsl_mtx_inv(struct zsl_mtx *m, struct zsl_mtx *mi);
int zsl_mtx_eigen(struct zsl_mtx *m, zsl_real_t *val, struct zsl_mtx *vec);

/**
 * @brief Traverses the matrix elements to find the minimum element value.
 *
 * @param m     Pointer to the zsl_mtz to traverse.
 * @param x     The minimum element value found in matrix 'm'.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_mtx_min(struct zsl_mtx *m, zsl_real_t *x);

/**
 * @brief Traverses the matrix elements to find the maximum element value.
 *
 * @param m     Pointer to the zsl_mtz to traverse.
 * @param x     The maximum element value found in matrix 'm'.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_mtx_max(struct zsl_mtx *m, zsl_real_t *x);

/**
 * @brief Traverses the matrix elements to find the (i,j) index of the minimum
 *        element value. If multiple identical minimum values are founds,
 *        the (i, j) index values returned will refer to the first element.
 *
 * @param m     Pointer to the zsl_mtz to traverse.
 * @param i     Pointer to the row index of the minimum element value found
 *              in matrix 'm'.
 * @param j     Pointer to the column index of the minimum element value
 *              found in matrix 'm'.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_mtx_min_idx(struct zsl_mtx *m, size_t *i, size_t *j);

/**
 * @brief Traverses the matrix elements to find the (i,j) index of the maximum
 *        element value. If multiple identical maximum values are founds,
 *        the (i, j) index values returned will refer to the first element.
 *
 * @param m     Pointer to the zsl_mtz to traverse.
 * @param i     Pointer to the row index of the maximum element value found
 *              in matrix 'm'.
 * @param j     Pointer to the column index of the maximum element value
 *              found in matrix 'm'.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_mtx_max_idx(struct zsl_mtx *m, size_t *i, size_t *j);

/**
 * @brief Checks if two matrices are identical in shape and content.
 *
 * @param ma The first matrix.
 * @param mb The second matrix.
 *
 * @return true if the two matrices have the same shape and values,
 *         otherwise false.
 */
bool zsl_mtx_is_equal(struct zsl_mtx *ma, struct zsl_mtx *mb);

/**
 * @brief Checks if all elements in matrix m are >= zero and non-null.
 *
 * @param m The matrix to check.
 *
 * @return true if the all matrix elements are zero, positive and non-null,
 *         otherwise false.
 */
bool zsl_mtx_is_notneg(struct zsl_mtx *m);

//int      zsl_mtx_fprint(FILE *stream, zsl_mtx *m);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_MATRICES_H_ */
