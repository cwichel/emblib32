/**
 *******************************************************************************
 * @file    common.h
 * @author  Christian Wiche
 * @date    2024
 * @brief   Common definitions for the firmware project
 * @note    None
 * @warning None
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 Christian Wiche. All rights reserved.
 *
 *******************************************************************************
 */
#ifndef _COMMON_H_
#define _COMMON_H_
#ifdef  __cplusplus
extern "C" {
#endif

#include "emblib32_core.h"
#include "emblib32_trace.h"

/*-------------------------------------------------------------------------*//**
* @addtogroup EmbLib32
* @{
* @addtogroup Middlewares
* @{
* @addtogroup Common
* @{
*//*--------------------------------------------------------------------------*/

/*-->> TURN ON/OFF FEATURES <<---------------------------*/

#define ENABLE_TRACER         1U

/*-->> TRACER <<-----------------------------------------*/

/** Tracer topics (Max = 32) */
typedef enum
{
  TRACE_TOPIC_SYSTEM = 0x00U,
  TRACE_TOPIC_NUM,
} t_trace_topic;

/** Tracer default level */
#ifdef DEBUG
  #define TRACE_DEFAULT_LEVEL   TRACE_LEVEL_DEBUG
#else
  #define TRACE_DEFAULT_LEVEL   TRACE_LEVEL_INFO
#endif

/** Tracer default topic */
#define TRACE_DEFAULT_TOPIC     TRACE_TOPICS_ALL

/** Tracer name */
#define TRACE_NAME              "EMBLIB32"

/* Tracing macros */
#if ENABLE_TRACER == 1U
  #define LFATAL(topic, ...)    trace(TRACE_NAME, TRACE_LEVEL_FATAL  , topic, __FILE__, __LINE__, __VA_ARGS__)
  #define LERROR(topic, ...)    trace(TRACE_NAME, TRACE_LEVEL_ERROR  , topic, __FILE__, __LINE__, __VA_ARGS__)
  #define LWARNING(topic, ...)  trace(TRACE_NAME, TRACE_LEVEL_WARNING, topic, __FILE__, __LINE__, __VA_ARGS__)
  #define LINFO(topic, ...)     trace(TRACE_NAME, TRACE_LEVEL_INFO   , topic, __FILE__, __LINE__, __VA_ARGS__)
  #define LDEBUG(topic, ...)    trace(TRACE_NAME, TRACE_LEVEL_DEBUG  , topic, __FILE__, __LINE__, __VA_ARGS__)
#else
  #define LFATAL(...)           ( (void)0 )
  #define LERROR(...)           ( (void)0 )
  #define LWARNING(...)         ( (void)0 )
  #define LINFO(...)            ( (void)0 )
  #define LDEBUG(...)           ( (void)0 )
#endif /* ENABLE_TRACER */

/*-->> VERSION <<----------------------------------------*/
/* Version format: MM.mm.bb
 *   MM: Major
 *   mm: Minor
 *   bb: Build/commit
 * Example: 1.0.5439210
 *-----------------------
 * Major:
 *    0...80 : Production
 *   80...99 : Reserved
 *   99      : Engineering
 */
#ifdef DEBUG
  #define VERSION_MAJOR       99U
  #define VERSION_MINOR       0U
  #define VERSION_BUILD       0U
#else
  #include "version.h"
#endif

/** Firmware version string size: MM.mm.0123456789 */
#define VERSION_SIZE          17U

/** Firmware version string */
extern char FW_VERSION[VERSION_SIZE];

/*-------------------------------------------------------------------------*//**
* @} <!-- End: EmbLib32 -->
* @} <!-- End: Middlewares -->
* @} <!-- End: Common -->
*//*--------------------------------------------------------------------------*/
#ifdef  __cplusplus
}
#endif
#endif /* _COMMON_H_ */
