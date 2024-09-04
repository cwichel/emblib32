/**
 *******************************************************************************
 * @file    emblib32_trace.h
 * @author  Christian Wiche
 * @date    2024
 * @brief   Tracer utilities
 * @note    None
 * @warning None
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 Christian Wiche. All rights reserved.
 *
 *******************************************************************************
 */
#ifndef _EMBLIB32_TRACE_H_
#define _EMBLIB32_TRACE_H_
#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "emblib32_rtos.h"

/*-------------------------------------------------------------------------*//**
* @addtogroup EmbLib32
* @{
* @addtogroup Middlewares
* @{
* @addtogroup Trace
* @{
*//*-----------------------------------------------------------------------*//**
* @addtogroup PUBLIC_Definitions
* @{
*//*--------------------------------------------------------------------------*/

/** Tracer topic maximum value (Max = 32) */
#define TRACE_TOPIC_MAX         32U

/** Tracer topics full enabled mask */
#define TRACE_TOPICS_ALL        0xFFFFFFFFU

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PUBLIC_Definitions -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PUBLIC_Macros
* @{
*//*--------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PUBLIC_Macros -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PUBLIC_Types
* @{
*//*--------------------------------------------------------------------------*/

/** @brief Tracer levels */
typedef enum
{
  TRACE_LEVEL_FATAL = 0x00U,
  TRACE_LEVEL_ERROR,
  TRACE_LEVEL_WARNING,
  TRACE_LEVEL_INFO,
  TRACE_LEVEL_DEBUG,
  TRACE_LEVEL_ALL,
} t_trace_level;

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

/**
 * @brief Initialize the tracer
 * @param level Trace level
 * @param topics Trace topics
 */
void trace_init(uint8_t level, uint32_t topics);

/**
 * @brief Enable or disable the tracer
 *
 * @note  If the tracer is disabled the trace function will do nothing
 *
 * @param quiet true to disable the tracer, false to enable it
 */
void trace_set_quiet(bool quiet);

/**
 * @brief Set the trace level
 * @param level Trace level
 */
void trace_set_level(uint8_t level);

/**
 * @brief Set the trace topic mask
 * @param mask Trace topic mask
 */
void trace_set_topic_mask(uint32_t mask);

/**
 * @brief Set the trace topic
 * @param topic Trace topic
 * @param enable true to enable, false to disable
 * @return Error code
 */
uint32_t trace_set_topic(uint8_t topic, bool enable);

/**
 * @brief Configure the trace lock function
 * @param lock    Lock function
 * @param object  Lock object
 * @return Error code
 */
uint32_t trace_set_lock(t_rtos_lock lock, void* object);

/**
 * @brief Trace a message
 * @param name Name of the trace
 * @param level Trace level
 * @param topic Trace topic
 * @param file File name
 * @param line Line number
 * @param args Trace format and arguments
 * @return Error code
 */
uint32_t trace(const char *name, uint8_t level, uint8_t topic, const char *file, uint32_t line, ...);

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PUBLIC_API -->
*//*-----------------------------------------------------------------------*//**
* @} <!-- End: EmbLib32 -->
* @} <!-- End: Middlewares -->
* @} <!-- End: Trace -->
*//*--------------------------------------------------------------------------*/
#ifdef  __cplusplus
}
#endif
#endif /* _EMBLIB32_TRACE_H_ */
