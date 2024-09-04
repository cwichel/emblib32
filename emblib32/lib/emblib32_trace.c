/**
 *******************************************************************************
 * @file    emblib32_trace.c
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
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "emblib32_bitmask.h"
#include "emblib32_core.h"
#include "emblib32_trace.h"

/*-------------------------------------------------------------------------*//**
* @addtogroup EmbLib32
* @{
* @addtogroup Middlewares
* @{
* @addtogroup Trace
* @{
* @defgroup PUBLIC_Definitions          PUBLIC constants
* @defgroup PUBLIC_Macros               PUBLIC macros
* @defgroup PUBLIC_Types                PUBLIC data-types
* @defgroup PUBLIC_Data                 PUBLIC data / variables
* @defgroup PUBLIC_API                  PUBLIC API
* @defgroup PRIVATE_TUNABLES            PRIVATE compile-time tunables
* @defgroup PRIVATE_Definitions         PRIVATE constants
* @defgroup PRIVATE_Macros              PRIVATE macros
* @defgroup PRIVATE_Types               PRIVATE data-types
* @defgroup PRIVATE_Data                PRIVATE data / variables
* @defgroup PRIVATE_Functions           PRIVATE functions
*//*-----------------------------------------------------------------------*//**
* @addtogroup PRIVATE_TUNABLES
* @{
*//*--------------------------------------------------------------------------*/

/** End-Of-Line */
#define TRACE_EOL         "\n"

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PRIVATE_TUNABLES -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PRIVATE_Definitions
* @{
*//*--------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PRIVATE_Definitions -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PRIVATE_Macros
* @{
*//*--------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PRIVATE_Macros -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PRIVATE_Types
* @{
*//*--------------------------------------------------------------------------*/

/** @brief Trace control block */
typedef struct
{
  bool        quiet;    /*!< Quiet mode */
  uint8_t     level;    /*!< Trace level */
  uint32_t    topics;   /*!< Trace topics mask */
  /* RTOS support */
  t_rtos_lock lock;     /*!< Lock handler function */
  void*       object;   /*!< Lock object */
} t_trace;

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PRIVATE_Types -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PRIVATE_Data
* @{
*//*--------------------------------------------------------------------------*/

static const char *_trace_level_str[TRACE_LEVEL_ALL] = {
  "FATAL",
  "ERROR",
  "WARNING",
  "INFO",
  "DEBUG",
};

static t_trace _trace = {
  .quiet  = false,
  .level  = TRACE_LEVEL_ALL,
  .topics = TRACE_TOPICS_ALL,
  .lock   = NULL,
  .object = NULL,
};

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PRIVATE_Data -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PRIVATE_Functions
* @{
*//*--------------------------------------------------------------------------*/

static void _trace_lock(bool lock);

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PRIVATE_Functions -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PUBLIC_API
* @{
*//*--------------------------------------------------------------------------*/

void trace_init(uint8_t level, uint32_t topics)
{
  trace_set_level(level);
  trace_set_topic_mask(topics);
}

void trace_set_quiet(bool quiet)
{
  _trace.quiet = quiet;
}

void trace_set_level(uint8_t level)
{
  _trace.level = level;
}

void trace_set_topic_mask(uint32_t mask)
{
  _trace.topics = mask;
}

uint32_t trace_set_topic(uint8_t topic, bool enable)
{
  /* Validate */
  if (topic >= TRACE_TOPIC_MAX)
  {
    return EMBLIB32_ERROR_PARAMETER;
  }
  /* Update */
  bitmask_update(&_trace.topics, topic, enable);
  return EMBLIB32_OK;
}

uint32_t trace_set_lock(t_rtos_lock lock, void* object)
{
  /* Validate */
  if (!lock)
  {
    return EMBLIB32_ERROR_PARAMETER;
  }
  /* Update */
  _trace.lock   = lock;
  _trace.object = object;
  return EMBLIB32_OK;
}

uint32_t trace(const char *name, uint8_t level, uint8_t topic, const char *file, uint32_t line, ...)
{
  /* Validate */
  if (_trace.quiet || (level > _trace.level) || !bitmask_get(&_trace.topics, topic))
  {
    return EMBLIB32_ERROR_PARAMETER;
  }
  
  _trace_lock(true);

  /* Print trace */
  va_list args;
  va_start(args, line);
  char const* format = va_arg(args, char*);
  
  if (name)
  {
    printf("%s: ", name);
  }
  printf("%-7s: ", _trace_level_str[level]);
  if (file)
  {
    char *pf = strrchr(file, '/');
    while (pf && (pf != file))
    {
      if ((*pf == '/') || (*pf == '\\'))
      {
        pf++;
        break;
      }
      pf--;
    }
    #if EMBLIB32_HOST
    printf("(%s:%u) ", pf, line);
    #else
    printf("(%s:%lu) ", pf, line);
    #endif
  }
  vprintf(format, args);
  printf(TRACE_EOL);
  
  _trace_lock(false);
  
  return EMBLIB32_OK;
}

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PUBLIC_API -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PRIVATE_Functions
* @{
*//*--------------------------------------------------------------------------*/

/**
 * @brief Handle trace lock
 * @param lock true to lock, false to unlock
 */
static void _trace_lock(bool lock)
{
  if (_trace.lock)
  {
    _trace.lock(_trace.object, lock);
  }
}

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PRIVATE_Functions -->
*//*-----------------------------------------------------------------------*//**
* @} <!-- End: EmbLib32 -->
* @} <!-- End: Middlewares -->
* @} <!-- End: Trace -->
*//*--------------------------------------------------------------------------*/
