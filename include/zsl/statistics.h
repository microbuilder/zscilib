/*
 * Copyright (c) 2019-2020 Kevin Townsend (KTOWN)
 * Copyright (c) 2021 Marti Riba Pons
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * \defgroup STATISTICS Statistics
 *
 * @brief Statistics-related functions.
 */

/**
 * @file
 * @brief API header file for statistics in zscilib.
 *
 * This file contains the zscilib statistics APIs
 */

#ifndef ZEPHYR_INCLUDE_ZSL_STATISTICS_H_
#define ZEPHYR_INCLUDE_ZSL_STATISTICS_H_

#include <zsl/zsl.h>
#include <zsl/vectors.h>
#include <zsl/matrices.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Simple linear regression coefficients. */
struct zsl_sta_linreg {
	/**
	 * @brief The estimated slope.
	 */
	zsl_real_t slope;
	/**
	 * @brief The estimated intercept.
	 */
	zsl_real_t intercept;
	/**
	 * @brief The correlation coefficient, where closer to 1.0 is better.
	 */
	zsl_real_t correlation;
};

/**
 * @brief Computes the arithmetic mean (average) of a vector.
 *
 * @param v  The vector to use.
 * @param m  The arithmetic mean of the components of v.
 *
 * @return int
 */
int zsl_sta_mean(struct zsl_vec *v, zsl_real_t *m);

/**
 * @brief Subtracts the mean of vector v from every component of the vector.
 *        The output vector w then has a zero mean.
 *
 * @param v  The vector to use.
 * @param w  The output vector with zero mean.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_demean(struct zsl_vec *v, struct zsl_vec *w);

/**
 * @brief Computes the given percentile of a vector.
 *
 * @param v    The input vector.
 * @param p    The percentile to be calculated.
 * @param val  The output value.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_percentile(struct zsl_vec *v, size_t p, zsl_real_t *val);

/**
 * @brief Computes the median of a vector (the value separating the higher half
 *        from the lower half of a data sample).
 *
 * @param v  The vector to use.
 * @param m  The median of the components of v.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_median(struct zsl_vec *v, zsl_real_t *m);

/**
 * @brief Calculates the first, second and third quartiles of a vector v.
 *
 * @param v   The vector to use.
 * @param q1  The first quartile of v.
 * @param q1  The second quartile of v, also the median of v.
 * @param q1  The third quartile of v.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_quart(struct zsl_vec *v, zsl_real_t *q1, zsl_real_t *q2,
		  zsl_real_t *q3);

/**
 * @brief Calculates the numeric difference between the third and the first
 *        quartiles of a vector v.
 *
 * @param v  The input vector.
 * @param r  The interquartile range of v.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_quart_range(struct zsl_vec *v, zsl_real_t *r);

/**
 * @brief Computes the mode or modes of a vector v.
 *
 * @param v  The vector to use.
 * @param w  Output vector whose components are the modes. If there is only
 *           one mode, the length of w will be 1.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_mode(struct zsl_vec *v, struct zsl_vec *w);

/**
 * @brief Computes the difference between the greatest value and the lowest in
 *        a vector v.
 *
 * @param v The vector to use.
 * @param r The range of the data in v.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_data_range(struct zsl_vec *v, zsl_real_t *r);

/**
 * @brief Computes the variance of a vector v (the average of the squared
 *        differences from the mean).
 *
 * @param v     The vector to use.
 * @param var   The variance of v.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_var(struct zsl_vec *v, zsl_real_t *var);

/**
 * @brief Computes the standard deviation of vector v.
 * 
 * Standard deviation is an indication of how spread-out numbers in 'v' are,
 * relative to the mean. It helps differentiate what is in the "standard"
 * range (1 standard deviation from mean), and what is outside (above or below)
 * this range, to pick out statistical outliers.
 *
 * @param v  The vector to use.
 * @param s  The output standard deviation of the vector v.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_sta_dev(struct zsl_vec *v, zsl_real_t *s);

/**
 * @brief Computes the variance of two sets of data: v and w.
 *
 * @param v  First set of data.
 * @param w  Second set of data.
 * @param c  Covariance of the vectors v and w.
 *
 * @return 0 on success, and -EINVAL if the vectors aren't identically sized.
 */
int zsl_sta_covar(struct zsl_vec *v, struct zsl_vec *w, zsl_real_t *c);

/**
 * @brief Calculates the nxn covariance matrix of a set of n vectors of the
 *        same length.
 *
 * @param m   Input matrix, whose columns are the different data sets.
 * @param mc  Output nxn covariance matrix.
 *
 * @return 0 on success, and -EINVAL if 'mc' is not a square matrix with the
 * 		   same number of columns as 'm'.
 */
int zsl_sta_covar_mtx(struct zsl_mtx *m, struct zsl_mtx *mc);

/**
 * @brief Calculates the slope, intercept and correlation coefficient of the
 *        linear regression of two vectors, allowing us to make a prediction
 *        of w on the basis of v.
 *
 * Simple linear regression is useful for predicting a quantitative response.
 * It assumes that there is an approximately linear relationship between vector
 * v and vector w, and calculates a series of coefficients to project this
 * relationship in either direction.
 *
 * The output of this function is a slope and intercept value, such
 * that the resulting line closely tracks the linear progression of the input
 * samples. The correlation coefficient  estimates the 'closeness' of the
 * match.
 *
 * Given the equation 'y = slope * x + intercept', where we provide x, we can
 * estimate the y value for a arbitrary value of x, where x is related to the
 * range of values provided in vector 'v' (the x axis), and y is related to the
 * values provided in vector 'w' (the y axis).
 *
 * @param v   The first input vector, corresponding to the x-axis.
 * @param w   The second input vector, corresponding to the y-axis.
 * @param c   Pointer to the calculated linear regression coefficients.
 *
 * @return 0 on success, and -EINVAL if the vectors aren't identically sized.
 */
int zsl_sta_linear_reg(struct zsl_vec *v, struct zsl_vec *w,
		       struct zsl_sta_linreg *c);

/**
 * @brief Calculates the absolute error given a value and its expected value.
 *
 * @param val       Input value.
 * @param exp_val   Input expected value.
 * @param err		Output absolute error.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_abs_err(zsl_real_t *val, zsl_real_t *exp_val, zsl_real_t *err);

/**
 * @brief Calculates the relative error given a value and its expected value.
 *
 * @param val       Input value.
 * @param exp_val   Input expected value.
 * @param err		Output relative error.
 *
 * @return  0 if everything executed correctly, otherwise an appropriate
 *          error code.
 */
int zsl_sta_rel_err(zsl_real_t *val, zsl_real_t *exp_val, zsl_real_t *err);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_STATISTICS_H_ */

/** @} */ /* End of statistics group */
