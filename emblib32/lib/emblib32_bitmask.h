/**
 *******************************************************************************
 * @file    emblib32_bitmask.h
 * @author  Christian Wiche
 * @date    2024
 * @brief   Bitmask and bit manipulation utilities
 * @note    None
 * @warning None
 *******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 Christian Wiche. All rights reserved.
 *
 *******************************************************************************
 */
#ifndef _EMBLIB32_BITMASK_H_
#define _EMBLIB32_BITMASK_H_
#ifdef  __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/*-------------------------------------------------------------------------*//**
* @addtogroup EmbLib32
* @{
* @addtogroup Middlewares
* @{
* @addtogroup Bitmask
* @{
*//*-----------------------------------------------------------------------*//**
* @addtogroup PUBLIC_Definitions
* @{
*//*--------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PUBLIC_Definitions -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PUBLIC_Macros
* @{
*//*--------------------------------------------------------------------------*/

/** Get a mask with the Nth bit set.
 * Examples:
 * - BIT(0) -> 0b00000001
 * - BIT(5) -> 0b00100000
 */
#ifndef BIT
  #define BIT(n)            (1U << (n))
#endif

/** Get a mask with the first N bits set.
 * Examples:
 * - BIT_MASK(0) -> 0b00000000
 * - BIT_MASK(5) -> 0b00011111
 */
#ifndef BIT_MASK
  #define BIT_MASK(n)       (BIT(n) - 1U)
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

/**
 * @brief Set a bit in the bitmask
 *
 * @param mask Ptr. to mask
 * @param bit Bit to set
 * @return Bitmask
 */
uint32_t bitmask_set(uint32_t *mask, uint8_t bit);

/**
 * @brief Clear a bit in the bitmask
 *
 * @param mask Ptr. to mask
 * @param bit Bit to clear
 * @return Bitmask
 */
uint32_t bitmask_clear(uint32_t *mask, uint8_t bit);

/**
 * @brief Update a bit in the bitmask
 *
 * @param mask Ptr. to mask
 * @param bit Bit to update
 * @param state New state
 * @return Bitmask
 */
uint32_t bitmask_update(uint32_t *mask, uint8_t bit, bool value);

/**
 * @brief Check if any bit is set
 *
 * @param mask Ptr. to mask
 * @return True if any bit is set
 */
bool bitmask_any(const uint32_t *mask);

/**
 * @brief Check if a bits is set
 *
 * @param mask Ptr. to mask
 * @param bit Bit to read
 * @return True if bit is set
 */
bool bitmask_get(const uint32_t *mask, uint8_t bit);

/**
 * @brief Check if a bits is set and clear it
 *
 * @param mask Ptr. to mask
 * @param bit Bit to read and clear
 * @return True if bit was set
 */
bool bitmask_get_and_clear(uint32_t *mask, uint8_t bit);

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PUBLIC_API -->
*//*-----------------------------------------------------------------------*//**
* @} <!-- End: EmbLib32 -->
* @} <!-- End: Middlewares -->
* @} <!-- End: Bitmask -->
*//*--------------------------------------------------------------------------*/
#ifdef  __cplusplus
}
#endif
#endif /* _EMBLIB32_BITMASK_H_ */
