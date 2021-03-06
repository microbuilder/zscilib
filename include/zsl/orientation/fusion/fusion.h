/*
 * Copyright (c) 2021 Kevin Townsend
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup FUSION Sensor Fusion
 *
 * @brief Sensor fusion algorithms
 *
 * @ingroup ORIENTATION
 *  @{ */

/**
 * @file
 * @brief API header file for sensor fusion algorithms in zscilib.
 *
 * This file contains the zscilib sensor fusion API.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_FUSION_H_
#define ZEPHYR_INCLUDE_ZSL_FUSION_H_

#include <zsl/zsl.h>
#include <zsl/matrices.h>
#include <zsl/orientation/ahrs.h>
#include <zsl/orientation/euler.h>
#include <zsl/orientation/quaternions.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @typedef zsl_fus_init_cb_t
 * @brief Init callback prototype for sensor fusion implementations.
 *
 * @param freq      Sample frequency in Hz (samples per second).
 *
 * @return 0 on success, negative error code on failure
 */
typedef int (*zsl_fus_init_cb_t)(uint32_t freq);

/**
 * @typedef zsl_fus_feed_cb_t
 * @brief Update callback prototype for sensor fusion implementations.
 *
 * @param accel     Pointer to the accelerometer XYZ data. NULL for none.
 * @param mag       Pointer to the magnetometer XYZ data. NULL for none.
 * @param gyro      Pointer to the gyroscope XYZ data. NULL for none.
 *
 * @return 0 on success, negative error code on failure
 */
typedef int (*zsl_fus_feed_cb_t)(struct zsl_vec *accel,
				 struct zsl_vec *mag, struct zsl_vec *gyro);

/**
 * @typedef zsl_fus_get_quat_cb_t
 * @brief Get quaternion callback prototype for sensor fusion implementations.
 *
 * @param q         Pointer to the @ref zsl_quat to populate.
 *
 * @return 0 on success, negative error code on failure
 */
typedef int (*zsl_fus_get_quat_cb_t)(struct zsl_quat *q);

/**
 * @typedef zsl_fus_error_cb_t
 * @brief Callback prototype when a fusion algorithm fails to properly feed.
 *
 * @param cfg       Pointer to the config struct/value being used.
 * @param error     Negative error code produced during node execution.
 */
typedef void (*zsl_fus_error_cb_t)(void *cfg, int error);

/**
 * @brief Sensor fusion algorithm implementation.
 */
struct zsl_fus_drv {
	/**
	 * @brief Callback to fire when initialising the driver.
	 */
	zsl_fus_init_cb_t init_handler;

	/**
	 * @brief Callback to fire when feeding/updating the driver.
	 */
	zsl_fus_feed_cb_t feed_handler;

	/**
	 * @brief Callback to fire when a @brief zsl_quat reading is requested.
	 */
	zsl_fus_get_quat_cb_t get_quat_handler;

	/**
	 * @brief Callback to fire when the 'feed' command fails.
	 */
	zsl_fus_error_cb_t error_handler;

	/**
	 * @brief Config settings for the driver. The exact struct or
	 *        value(s) defined here are driver-specific and defined
	 *        by the implementing module.
	 */
	void *config;
};

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_FUSION_H_ */

/** @} */ /* End of fusion group */
