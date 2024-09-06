/**
 *******************************************************************************
 * @file    emblib32_cobs.h
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
#ifndef _EMBLIB32_COBS_H_
#define _EMBLIB32_COBS_H_
#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*-------------------------------------------------------------------------*//**
* @addtogroup EmbLib32
* @{
* @addtogroup Middlewares
* @{
* @addtogroup COBS
* @{
*//*-----------------------------------------------------------------------*//**
* @addtogroup PUBLIC_Definitions
* @{
*//*--------------------------------------------------------------------------*/

/** Maximum block size */
#define COBS_MAX_BLOCK_SIZE   254U

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PUBLIC_Definitions -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PUBLIC_Macros
* @{
*//*--------------------------------------------------------------------------*/

/** Maximum size of the encoded buffer */
#define COBS_MAX_ENCODED_SIZE(src_len) \
  (((src_len) == 0U ? 0U : (src_len) + ((src_len) / COBS_MAX_BLOCK_SIZE)) + 2U)

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PUBLIC_Macros -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PUBLIC_Types
* @{
*//*--------------------------------------------------------------------------*/

/** COBS context */
typedef struct
{
  const uint8_t *start;
  const uint8_t *end;
  uint8_t       *out;
  uint8_t       *code_ptr;
  uint8_t       code;
  uint8_t       copy;
} t_cobs;

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
 * @brief COBS encode
 * @param dec Decoded buffer (source)
 * @param dec_len Decoded buffer length
 * @param enc Encoded buffer (destination)
 * @param enc_len Encoded buffer length
 * @param encoded Number of bytes encoded
 * @return Error code
 */
int32_t cobs_encode(const uint8_t *dec, uint16_t dec_len, uint8_t *enc, uint16_t enc_len, uint16_t *encoded);

/**
 * @brief COBS encode start: Prepare context for encoding
 * @param ctx COBS context
 * @param enc Encoded buffer
 * @param enc_len Encoded buffer length (max
 * @return Error code
 */
int32_t cobs_encode_start(t_cobs *ctx, uint8_t *enc, uint16_t enc_len);

/**
 * @brief COBS encode increment: Add new decoded data to the encoding process
 * @param ctx COBS context
 * @param dec Decoded buffer
 * @param dec_len Decoded buffer length
 * @return Error code
 */
int32_t cobs_encode_increment(t_cobs *ctx, const uint8_t *dec, uint16_t dec_len);

/**
 * @brief COBS encode end: Finish the encoding process
 * @param ctx COBS context
 * @param encoded Number of bytes encoded
 * @return Error code
 */
int32_t cobs_encode_end(t_cobs *ctx, uint16_t *encoded);

/**
 * @brief COBS decode
 * @param enc Encoded buffer (source)
 * @param enc_len Encoded buffer length
 * @param dec Decoded buffer (destination)
 * @param dec_len Decoded buffer length
 * @param decoded Number of bytes decoded
 * @return Error code
 */
int32_t cobs_decode(const uint8_t *enc, uint16_t enc_len, uint8_t *dec, uint16_t dec_len, uint16_t *decoded);

/**
 * @brief COBS decode start: Prepare context for decoding
 * @param ctx COBS context
 * @param dec Decoded buffer
 * @param dec_len Decoded buffer length (max)
 * @return Error code
 */
int32_t cobs_decode_start(t_cobs *ctx, uint8_t *dec, uint16_t dec_len);

/**
 * @brief COBS decode increment: Add new encoded data to the decoding process
 * @param ctx COBS context
 * @param enc Encoded buffer
 * @param enc_len Encoded buffer length
 * @return Error code
 */
int32_t cobs_decode_increment(t_cobs *ctx, const uint8_t *enc, uint16_t enc_len);

/**
 * @brief COBS decode end: Finish the decoding process
 * @param ctx COBS context
 * @param decoded Number of bytes decoded
 * @return Error code
 */
int32_t cobs_decode_end(const t_cobs *ctx, uint16_t *decoded);

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PUBLIC_API -->
*//*-----------------------------------------------------------------------*//**
* @} <!-- End: EmbLib32 -->
* @} <!-- End: Middlewares -->
* @} <!-- End: COBS -->
*//*--------------------------------------------------------------------------*/
#ifdef  __cplusplus
}
#endif
#endif /* _EMBLIB32_COBS_H_ */
