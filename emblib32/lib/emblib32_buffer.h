/**
 ******************************************************************************
 * @file    emblib32_buffer.h
 * @author  Christian Wiche
 * @date    2024
 * @brief   Generic buffer implementation.
 * @note    None
 * @warning None
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 Christian Wiche. All rights reserved.
 *
 ******************************************************************************
 */
#ifndef _EMBLIB32_BUFFER_H_
#define _EMBLIB32_BUFFER_H_
#ifdef  __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "emblib32_core.h"
#include "emblib32_rtos.h"

/*-------------------------------------------------------------------------*//**
* @addtogroup EmbLib32
* @{
* @addtogroup Middlewares
* @{
* @addtogroup Buffer
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

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PUBLIC_Macros -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PUBLIC_Types
* @{
*//*--------------------------------------------------------------------------*/

/** Enumeration with buffer operation modes */
typedef enum
{
  BUFF_OPMODE_R_FIFO      = 0x01,    /*!< R_FIFO: Reading mode: FIFO -> Queue */
  BUFF_OPMODE_R_LIFO      = 0x02,    /*!< R_LIFO: Reading mode: LIFO -> Stack */
  BUFF_OPMODE_W_OVERFLOW  = 0x04,    /*!< W_OVF:  Write mode:   Overflow > Oldest items are overwritten */
  BUFF_OPMODE_DEFAULT     = BUFF_OPMODE_R_FIFO | BUFF_OPMODE_W_OVERFLOW,
} t_buff_opmode;

/** Generic buff controller structure */
typedef struct
{
  uint8_t*        buff;       /*!< Array of items */
  uint32_t        mode;       /*!< Buffer operation mode */
  size_t          buff_size;  /*!< Buffer size (# items) */
  size_t          item_size;  /*!< Item size (bytes) */
  size_t          capacity;   /*!< Buffer size (bytes) */
  volatile size_t stored;     /*!< Buffer usage (bytes) */
  volatile size_t head;       /*!< Oldest item */
  volatile size_t tail;       /*!< Store new items */
  /* RTOS support */
  t_rtos_lock     lock;       /*!< Lock handler function */
  void*           object;     /*!< Lock object */
} t_buff;

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
 * @brief Initializes a buffer instance
 * @note  This function is thread unsafe. Use with care
 * @param ctrl      Buffer controller
 * @param buff      Array of items
 * @param buff_size Buffer size (# items)
 * @param item_size Item size (bytes)
 * @param mode      Operation mode (see t_buff_mode)
 * @param clear     True to purge the items buffer (fill with zeros)
 * @return Error code
 */
uint32_t buff_init(t_buff* ctrl, void* buff, size_t buff_size, size_t item_size, uint16_t mode, bool clear);

/**
 * @brief Configures the buffer lock function
 * @param ctrl      Buffer controller
 * @param lock      Lock function
 * @param object    Lock object
 * @return Error code
 */
uint32_t buff_set_lock(t_buff* ctrl, t_rtos_lock lock, void* object);

/**
 * @brief Locks/unlocks the buffer
 * @param ctrl      Buffer controller
 * @param lock      True to lock, false to unlock
 * @return Error code
 */
uint32_t buff_lock(t_buff* ctrl, bool lock);

/**
 * @brief Clears the buffer
 * @param ctrl      Buffer controller
 * @return Error code
 */
uint32_t buff_clear(t_buff* ctrl);

/**
 * @brief Pushes an item into the buffer
 * @note  Depending if the buffer is on overflow mode, the oldest item may be overwritten (an error is returned otherwise)
 * @param ctrl      Buffer controller
 * @param item      Incoming item
 * @return Error code
 */
uint32_t buff_push(t_buff* ctrl, const void* item);

/**
 * @brief Pushes several items into the buffer
 * @note  Depending if the buffer is on overflow mode, the oldest item may be overwritten (an error is returned otherwise)
 *        If the number of items to push is bigger than the capacity, only the last items are pushed
 * @param ctrl      Buffer controller
 * @param buff      Incoming array of items
 * @param size      Number of items to push
 * @param pushed    Number of items actually pushed
 * @return Error code
 */
uint32_t buff_push_chunk(t_buff* ctrl, const void* buff, size_t size, size_t* pushed);

/**
 * @brief Pops an item from the buffer
 * @note  Depending on the read mode (FIFO/LIFO), the oldest or newest item is popped
 * @param ctrl      Buffer controller
 * @param item      Receiving item
 * @return Error code
 */
uint32_t buff_pop(t_buff* ctrl, void* item);

/**
 * @brief Pops several items from the buffer
 * @note  Depending on the read mode (FIFO/LIFO), the oldest or newest item is popped
 * @param ctrl      Buffer controller
 * @param buff      Receiving array of items
 * @param size      Number of items to pop
 * @param popped    Number of items actually popped
 * @return Error code
 */
uint32_t buff_pop_chunk(t_buff* ctrl, void* buff, size_t size, size_t* popped);

/**
 * @brief Reads an item from the buffer without popping it
 * @note  Depending on the read mode (FIFO/LIFO), the oldest or newest item is read
 * @param ctrl      Buffer controller
 * @param item      Receiving item
 * @param index     Item index
 * @return Error code
 */
uint32_t buff_peek(t_buff* ctrl, void* item, size_t index);

/**
 * @brief Returns if the buffer is full
 * @param ctrl      Buffer controller
 * @return True if buffer is full
 */
bool buff_is_full(const t_buff* ctrl);

/**
 * @brief Returns if the buffer is empty
 * @param ctrl      Buffer controller
 * @return True if buffer is empty
 */
bool buff_is_empty(const t_buff* ctrl);

/**
 * @brief Get the buffer total size (capacity)
 * @param ctrl      Buffer controller
 * @return Buffer size (# items)
 */
size_t buff_get_size(const t_buff* ctrl);

/**
 * @brief Get the number of items stored in the buffer
 * @param ctrl      Buffer controller
 * @return Items stored (# items)
 */
size_t buff_get_count(const t_buff* ctrl);

/**
 * @brief Get the number of free slots in the buffer
 * @param ctrl      Buffer controller
 * @return Free slots (# items)
 */
size_t buff_get_available(const t_buff* ctrl);

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PUBLIC_API -->
*//*-----------------------------------------------------------------------*//**
* @} <!-- End: EmbLib32 -->
* @} <!-- End: Middlewares -->
* @} <!-- End: Buffer -->
*//*--------------------------------------------------------------------------*/
#ifdef  __cplusplus
}
#endif
#endif /* _EMBLIB32_BUFFER_H_ */
