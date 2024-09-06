/**
 *******************************************************************************
 * @file    emblib32_hash.c
 * @author  Christian Wiche
 * @date    2024
 * @brief   Hash computation utilities
 * @note    None
 * @warning None
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 Christian Wiche. All rights reserved.
 *
 *******************************************************************************
 */
#include "emblib32_hash.h"

/*-------------------------------------------------------------------------*//**
* @addtogroup EmbLib32
* @{
* @addtogroup Middlewares
* @{
* @addtogroup Hash
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

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PRIVATE_Types -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PRIVATE_Data
* @{
*//*--------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PRIVATE_Data -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PRIVATE_Functions
* @{
*//*--------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PRIVATE_Functions -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PUBLIC_API
* @{
*//*--------------------------------------------------------------------------*/

uint32_t hash_simple(const uint8_t* buff, uint16_t size)
{
  /* Validate */
  if (!buff || (size == 0U))
  {
    return 0x00U;
  }
  /* Compute hash
   * Note: Empirically, 31 and 37 are good choices for the multiplier when hashing
   * ASCII strings.
   */
  uint16_t idx = 0U;
  uint32_t hash = 0x00U;
  while (idx != size)
  {
    hash = (37U * hash) + buff[idx++];
  }
  return hash % size;
}

uint32_t hash_pearl(const uint8_t* buff, uint16_t size)
{
  /* Validate */
  if (!buff || (size == 0U))
  {
    return 0x00U;
  }
  
  /* Compute hash */
  uint16_t idx = 0U;
  uint32_t hash = 0x00U;
  while (idx != size)
  {
    hash += buff[idx++];
    hash += (hash << 10U);
    hash ^= (hash >> 6U);
  }
  hash += (hash << 3U);
  hash ^= (hash >> 11U);
  hash += (hash << 15U);
  return hash;
}

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PUBLIC_API -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PRIVATE_Functions
* @{
*//*--------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PRIVATE_Functions -->
*//*-----------------------------------------------------------------------*//**
* @} <!-- End: EmbLib32 -->
* @} <!-- End: Middlewares -->
* @} <!-- End: Hash -->
*//*--------------------------------------------------------------------------*/
