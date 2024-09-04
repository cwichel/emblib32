/**
 *******************************************************************************
 * @file    emblib32_cobs.c
 * @author  Christian Wiche
 * @date    2024
 * @brief   COBS (Consistent Overhead Byte Stuffing) implementation
 * @note    Ref: https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing
 * @warning None
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 Christian Wiche. All rights reserved.
 *
 *******************************************************************************
 */
#include "emblib32_cobs.h"
#include "emblib32_core.h"

/*-------------------------------------------------------------------------*//**
* @addtogroup EmbLib32
* @{
* @addtogroup Middlewares
* @{
* @addtogroup COBS
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

int32_t cobs_encode(const uint8_t *dec, uint16_t dec_len, uint8_t *enc, uint16_t enc_len, uint16_t *encoded)
{
  /* Validate */
  if (!dec || !enc || !encoded || (enc_len < COBS_MAX_ENCODED_SIZE(0)))
  {
    return EMBLIB32_ERROR_PARAMETER;
  }
  
  /* Encode */
  /* Special case: Empty buffer */
  if (dec_len == 0)
  {
    enc[0]    = 1U;
    enc[1]    = 0U;
    *encoded  = 2U;
    return EMBLIB32_OK;
  }
  
  /* General case */
  const uint8_t *start = enc;
  const uint8_t *end   = dec + dec_len;
  uint8_t *code_ptr    = enc++;
  uint8_t code         = 1;
  while (dec < end)
  {
    if (code != 0xFFU)
    {
      uint8_t byte = *dec++;
      if (byte != 0x00U)
      {
        *enc++ = byte;
        code++;
        continue;
      }
    }
    *code_ptr = code;
    code_ptr  = enc++;
    code      = 0x01U;
  }
  *code_ptr++ = code;
  *enc++      = 0x00U;
  *encoded    = (uint16_t)(enc - start);
  return EMBLIB32_OK;
}

int32_t cobs_decode(const uint8_t *enc, uint16_t enc_len, uint8_t *dec, uint16_t dec_len, uint16_t *decoded)
{
  /* Validate */
  if (!dec || !enc || !decoded || (enc_len < COBS_MAX_ENCODED_SIZE(0)))
  {
    return EMBLIB32_ERROR_PARAMETER;
  }

  /* Decode */
  const uint8_t *start = dec;
  const uint8_t *end   = enc + enc_len;
  uint8_t       code   = 0xFFU;
  uint8_t       copy   = 0U;
  for (; enc < end; copy--)
  {
    if (copy != 0U)
    {
      *dec++ = *enc++;
    }
    else
    {
      /* Special case: Zero byte */
      if ((code != 0xFFU) || ((end - enc) == 1U))
      {
        *dec++ = 0U;
      }
      copy = code = *enc++;
      if (code == 0U)
      {
        break;
      }
    }
  }
  /* Exclude delimiter */
  *decoded = (uint16_t)(dec - start - 1U);
  return EMBLIB32_OK;
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
* @} <!-- End: COBS -->
*//*--------------------------------------------------------------------------*/
