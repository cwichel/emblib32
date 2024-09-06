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
  int32_t status;
  t_cobs  ctx;
  
  status = cobs_encode_start(&ctx, enc, enc_len);
  if (status != EMBLIB32_OK)
  {
    return status;
  }
  status = cobs_encode_increment(&ctx, dec, dec_len);
  if (status != EMBLIB32_OK)
  {
    return status;
  }
  return cobs_encode_end(&ctx, encoded);
}

int32_t cobs_encode_start(t_cobs *ctx, uint8_t *enc, uint16_t enc_len)
{
  /* Validate */
  if (!ctx || !enc || (enc_len < COBS_MAX_ENCODED_SIZE(0)))
  {
    return EMBLIB32_ERROR_PARAMETER;
  }
  
  /* Configure */
  ctx->start    = enc;
  ctx->end      = enc + enc_len;
  ctx->out      = enc;
  ctx->code     = 1U;
  ctx->code_ptr = ctx->out++;
  return EMBLIB32_OK;
}

int32_t cobs_encode_increment(t_cobs *ctx, const uint8_t *dec, uint16_t dec_len)
{
  /* Validate */
  if (!ctx || !dec)
  {
    return EMBLIB32_ERROR_PARAMETER;
  }
  if (dec_len == 0U)
  {
    return EMBLIB32_OK;
  }
  
  /* Encode */
  uint8_t byte;
  const uint8_t *end = dec + dec_len;
  while (dec < end)
  {
    /* Validate output */
    if (ctx->out >= ctx->end)
    {
      return EMBLIB32_ERROR_BUFFER_OVERFLOW;
    }
    /* Continue */
    if (ctx->code != 0xFFU)
    {
      byte = *dec++;
      if (byte != 0x00U)
      {
        *ctx->out++ = byte;
        ctx->code++;
        continue;
      }
    }
    *ctx->code_ptr = ctx->code;
    ctx->code_ptr  = ctx->out++;
    ctx->code      = 0x01U;
  }
  return EMBLIB32_OK;
}

int32_t cobs_encode_end(t_cobs *ctx, uint16_t *encoded)
{
  /* Validate */
  if (!ctx || !encoded)
  {
    return EMBLIB32_ERROR_PARAMETER;
  }
  
  /* Finish */
  *ctx->code_ptr++ = ctx->code;
  *ctx->out++      = 0x00U;
  if (ctx->out > ctx->end)
  {
    return EMBLIB32_ERROR_BUFFER_OVERFLOW;
  }
  *encoded = (uint16_t)(ctx->out - ctx->start);
  return EMBLIB32_OK;
}

int32_t cobs_decode(const uint8_t *enc, uint16_t enc_len, uint8_t *dec, uint16_t dec_len, uint16_t *decoded)
{
  int32_t status;
  t_cobs  ctx;
  
  status = cobs_decode_start(&ctx, dec, dec_len);
  if (status != EMBLIB32_OK)
  {
    return status;
  }
  status = cobs_decode_increment(&ctx, enc, enc_len);
  if (status != EMBLIB32_OK)
  {
    return status;
  }
  return cobs_decode_end(&ctx, decoded);
}

int32_t cobs_decode_start(t_cobs *ctx, uint8_t *dec, uint16_t dec_len)
{
  /* Validate */
  if (!ctx || !dec || (dec_len < COBS_MAX_ENCODED_SIZE(0)))
  {
    return EMBLIB32_ERROR_PARAMETER;
  }
  
  /* Configure */
  ctx->start    = dec;
  ctx->end      = dec + dec_len;
  ctx->out      = dec;
  ctx->code     = 0xFF;
  ctx->copy     = 0x00U;
  return EMBLIB32_OK;
}

int32_t cobs_decode_increment(t_cobs *ctx, const uint8_t *enc, uint16_t enc_len)
{
  /* Validate */
  if (!ctx || !enc || (enc_len == 0U))
  {
    return EMBLIB32_ERROR_PARAMETER;
  }

  /* Decode */
  const uint8_t *end = enc + enc_len;
  while (enc < end)
  {
    /* Validate output */
    if (ctx->out >= ctx->end)
    {
      return EMBLIB32_ERROR_BUFFER_OVERFLOW;
    }
    /* Continue */
    if (ctx->copy > 0U)
    {
      if (*enc == 0x00U)
      {
        return EMBLIB32_ERROR_DECODE;
      }
      *ctx->out++ = *enc++;
    }
    else
    {
      if ((ctx->code != 0xFFU) || ((end - enc) == 1U))
      {
        *ctx->out++ = 0x00U;
      }
      ctx->copy = ctx->code = *enc++;
      if (ctx->code == 0x00U)
      {
        break;
      }
    }
    ctx->copy--;
  }
  return EMBLIB32_OK;
}

int32_t cobs_decode_end(const t_cobs *ctx, uint16_t *decoded)
{
  /* Validate */
  if (!ctx || !decoded)
  {
    return EMBLIB32_ERROR_PARAMETER;
  }
  
  /* Finish */
  if (ctx->copy != 0U)
  {
    return EMBLIB32_ERROR_DECODE;
  }
  *decoded = (uint16_t)(ctx->out - ctx->start - 1U);
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
