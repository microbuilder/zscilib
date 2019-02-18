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
 * @brief API header file for zscilib.
 *
 * This file contains the entry points to the zscilib APIs.
 */

#ifndef ZEPHYR_INCLUDE_ZSL_H_
#define ZEPHYR_INCLUDE_ZSL_H_

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#if CONFIG_ZSL_SINGLE_PRECISION
typedef float zsl_data_t;
#else
typedef double zsl_data_t;
#endif

/* TODO: Define common errors like shape mismatch, etc. */

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ZSL_H_ */
