/**
 *******************************************************************************
 * @file    emblib32_core.h
 * @author  Christian Wiche
 * @date    2024
 * @brief   Core definitions
 * @note    None
 * @warning None
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 Christian Wiche. All rights reserved.
 *
 *******************************************************************************
 */
#ifndef _EMBLIB32_CORE_H_
#define _EMBLIB32_CORE_H_
#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*-------------------------------------------------------------------------*//**
* @addtogroup EmbLib32
* @{
* @addtogroup Middlewares
* @{
* @addtogroup Core
* @{
*//*-----------------------------------------------------------------------*//**
* @addtogroup PUBLIC_Definitions
* @{
*//*--------------------------------------------------------------------------*/

/** @brief True if built from host */
#define EMBLIB32_HOST                   defined(HOST) || defined(SIMULATOR)

/* Error codes */
#define EMBLIB32_OK                     0x00U    /*!< No error */
#define EMBLIB32_ERROR_PARAMETER        0x02U    /*!< Invalid parameter */

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PUBLIC_Definitions -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PUBLIC_Macros
* @{
*//*--------------------------------------------------------------------------*/

/** Align 32bit */
#ifndef ALIGN_32
  #define ALIGN_32    __attribute__ ((aligned(32)))
#endif

/** Weak implementation */
#ifndef WEAK
  #if __WIN32 && __GNUC__
    #define WEAK      /* Not supported */
  #elif defined(__weak)
    #define WEAK      __weak
  #else
    #define WEAK      __attribute__ ((weak))
  #endif
#endif

/** Explicitly indicates a parameter as unused */
#ifndef UNUSED
  #define UNUSED(x)   ((void)(x))
#endif

/** Get absolute value */
#ifndef ABS
  #define ABS(a)      ((a) < (0)? -(a) : (a))
#endif

/** Get minimum value between a and b */
#ifndef MIN
  #define MIN(a, b)   (((a) < (b)) ? (a) : (b))
#endif

/** Get maximum value between a and b */
#ifndef MAX
  #define MAX(a, b)   (((a) > (b)) ? (a) : (b))
#endif

/** Get sign of a value */
#ifndef SIGN
  #define SIGN(a)     ((a) < (0)? (-1) : (1))
#endif

/** Get the number of elements in an array */
#ifndef ARRAY_SIZE
  #define ARRAY_SIZE(a)  (sizeof(a) / sizeof(a[0]))
#endif

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PUBLIC_Macros -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PUBLIC_Types
* @{
*//*--------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PUBLIC_Types -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PUBLIC_DATA
* @{
*//*--------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PUBLIC_DATA -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PUBLIC_API
* @{
*//*--------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PUBLIC_API -->
*//*-----------------------------------------------------------------------*//**
* @} <!-- End: EmbLib32 -->
* @} <!-- End: Middlewares -->
* @} <!-- End: Core -->
*//*--------------------------------------------------------------------------*/
#ifdef  __cplusplus
}
#endif
#endif /* _EMBLIB32_CORE_H_ */
