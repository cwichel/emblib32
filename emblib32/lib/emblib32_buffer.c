/**
 ******************************************************************************
 * @file    emblib32_buffer.c
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
#include <string.h>

#include "emblib32_buffer.h"

/*-------------------------------------------------------------------------*//**
* @addtogroup EmbLib32
* @{
* @addtogroup Middlewares
* @{
* @addtogroup Buffer
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

static uint32_t _buff_push_backend(t_buff* ctrl, const void* item);
static uint32_t _buff_pop_backend(t_buff* ctrl, void* item);

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PRIVATE_Functions -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PUBLIC_API
* @{
*//*--------------------------------------------------------------------------*/

uint32_t buff_init(t_buff* ctrl, void* buff, size_t buff_size, size_t item_size, uint16_t mode, bool clear)
{
  /* Sanity check */
  if (!ctrl || !buff || (item_size == 0) || (buff_size == 0))
  {
    return EMBLIB32_ERROR_PARAMETER;
  }

  /* Initialize buff */
  ctrl->buff      = (uint8_t*)buff;
  ctrl->buff_size = buff_size;
  ctrl->item_size = item_size;
  ctrl->capacity  = (buff_size * item_size);
  ctrl->mode      = mode;
  ctrl->lock      = NULL;
  ctrl->object    = NULL;

  /* Clear buffer if required */
  if (clear)
  {
    buff_clear(ctrl);
  }
  return EMBLIB32_OK;
}

uint32_t buff_set_lock(t_buff* ctrl, t_rtos_lock lock, void* object)
{
  /* Validate */
  if (!ctrl || !lock)
  {
    return EMBLIB32_ERROR_PARAMETER;
  }
  /* Update */
  ctrl->lock   = lock;
  ctrl->object = object;
  return EMBLIB32_OK;
}

uint32_t buff_lock(t_buff* ctrl, bool lock)
{
  /* Sanity check */
  if (!ctrl)
  {
    return EMBLIB32_ERROR_PARAMETER;
  }
  /* Lock */
  if (ctrl->lock)
  {
    ctrl->lock(ctrl->object, lock);
  }
  return EMBLIB32_OK;
}

uint32_t buff_clear(t_buff* ctrl)
{
  /* Sanity check */
  if (!ctrl || !ctrl->buff)
  {
    return EMBLIB32_ERROR_PARAMETER;
  }
  
  buff_lock(ctrl, true);

  /* Process */
  memset(ctrl->buff, 0, ctrl->capacity);
  ctrl->stored = 0;
  ctrl->head   = 0;
  ctrl->tail   = 0;
  
  buff_lock(ctrl, false);
  
  return EMBLIB32_OK;
}

uint32_t buff_push(t_buff* ctrl, const void* item)
{
  uint32_t status;

  /* Sanity check */
  if (!ctrl || !ctrl->buff || !item)
  {
    return EMBLIB32_ERROR_PARAMETER;
  }
  
  buff_lock(ctrl, true);
  
  /* Handle push */
  status = _buff_push_backend(ctrl, item);
  
  buff_lock(ctrl, false);
  
  return status;
}

uint32_t buff_push_chunk(t_buff* ctrl, const void* buff, size_t size, size_t* pushed)
{
  uint32_t      status = EMBLIB32_OK;
  size_t        count  = 0;
  const uint8_t *head  = (const uint8_t*)buff;

  /* Sanity check */
  if (!ctrl || !ctrl->buff || !buff)
  {
    return EMBLIB32_ERROR_PARAMETER;
  }
  
  buff_lock(ctrl, true);

  /* Handle push */
  while (count < size)
  {
    status = _buff_push_backend(ctrl, (head + (count * ctrl->item_size)));
    if (status != EMBLIB32_OK)
    {
      break;
    }
    count++;
  }
  
  buff_lock(ctrl, false);

  /* Update pushed count */
  if (pushed)
  {
    *pushed = count;
  }
  return status;
}

uint32_t buff_pop(t_buff* ctrl, void* item)
{
  uint32_t status;

  /* Sanity check */
  if (!ctrl || !ctrl->buff)
  {
    return EMBLIB32_ERROR_PARAMETER;
  }
  
  buff_lock(ctrl, true);

  /* Handle pop */
  status = _buff_pop_backend(ctrl, item);
  
  buff_lock(ctrl, false);
  
  return status;
}

uint32_t buff_pop_chunk(t_buff* ctrl, void* buff, size_t size, size_t* popped)
{
  uint32_t  status = EMBLIB32_OK;
  size_t    count  = 0;
  uint8_t   *head  = (uint8_t*)buff;

  /* Sanity check */
  if (!ctrl || !ctrl->buff || !buff)
  {
    return EMBLIB32_ERROR_PARAMETER;
  }
  
  buff_lock(ctrl, true);

  /* Handle pop */
  while (count < size)
  {
    status = _buff_pop_backend(ctrl, (head + (count * ctrl->item_size)));
    if (status != EMBLIB32_OK)
    {
      break;
    }
    count++;
  }
  
  buff_lock(ctrl, false);

  /* Update popped count */
  if (popped)
  {
    *popped = count;
  }
  return (count == 0)? EMBLIB32_ERROR_BUFFER_EMPTY : EMBLIB32_OK;
}

uint32_t buff_peek(t_buff* ctrl, void* item, size_t index)
{
  size_t offset;
  
  /* Sanity check */
  if (!ctrl || !ctrl->buff)
  {
    return EMBLIB32_ERROR_PARAMETER;
  }
  if ((index >= ctrl->buff_size) || (index >= buff_get_count(ctrl)))
  {
    return EMBLIB32_ERROR_BUFFER_INDEX;
  }
  
  buff_lock(ctrl, true);

  /* Read item */
  if (ctrl->mode & BUFF_OPMODE_R_FIFO)
  {
    offset = (ctrl->head + (index * ctrl->item_size)) % ctrl->capacity;
  }
  else
  {
    offset = (ctrl->capacity + ctrl->tail - ((index + 1) * ctrl->item_size)) % ctrl->capacity;
  }
  memcpy(item, (ctrl->buff + offset), ctrl->item_size);
  
  buff_lock(ctrl, false);
  
  return EMBLIB32_OK;
}

bool buff_is_full(const t_buff* ctrl)
{
  /* Sanity check */
  if (!ctrl)
  {
    return false;
  }

  return ctrl->stored == ctrl->capacity;
}

bool buff_is_empty(const t_buff* ctrl)
{
  /* Sanity check */
  if (!ctrl)
  {
    return false;
  }
  
  return ctrl->stored == 0;
}

size_t buff_get_size(const t_buff* ctrl)
{
  /* Sanity check */
  if (!ctrl)
  {
    return false;
  }
  
  return ctrl->buff_size;
}

size_t buff_get_count(const t_buff* ctrl)
{
  /* Sanity check */
  if (!ctrl)
  {
    return false;
  }
  
  return ctrl->stored / ctrl->item_size;
}

size_t buff_get_available(const t_buff* ctrl)
{
  /* Sanity check */
  if (!ctrl)
  {
    return false;
  }
  
  return (ctrl->capacity - ctrl->stored) / ctrl->item_size;
}

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PUBLIC_API -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PRIVATE_Functions
* @{
*//*--------------------------------------------------------------------------*/

/**
 * @brief Pushes an item into the buffer
 * @note  This function is the backend for the public API. It is not intended to be called directly.
 *        This function is NOT performing any sanity check.
 * @param ctrl Buffer controller
 * @param item Incoming item
 * @return Error code
 */
static uint32_t _buff_push_backend(t_buff* ctrl, const void* item)
{
  /* Handle overflow */
  if (!(ctrl->mode & BUFF_OPMODE_W_OVERFLOW) && buff_is_full(ctrl))
  {
    return EMBLIB32_ERROR_BUFFER_OVERFLOW;
  }

  /* Handle push */
  memcpy((ctrl->buff + ctrl->tail), item, ctrl->item_size);
  ctrl->tail = (ctrl->tail + ctrl->item_size) % ctrl->capacity;

  /* Handle storage and read offset */
  if (ctrl->stored == ctrl->capacity)
  {
    ctrl->head = ctrl->tail;
  }
  else
  {
    ctrl->stored += ctrl->item_size;
  }
  return EMBLIB32_OK;
}

/**
 * @brief Pops an item from the buffer
 * @note  This function is the backend for the public API. It is not intended to be called directly.
 *        This function is NOT performing any sanity check.
 * @param ctrl Buffer controller
 * @param item  Receiving item
 * @return Error code
 */
static uint32_t _buff_pop_backend(t_buff* ctrl, void* item)
{
  /* Handle empty */
  if (buff_is_empty(ctrl))
  {
    memset(item, 0x00, ctrl->item_size);
    return EMBLIB32_ERROR_BUFFER_EMPTY;
  }

  /* Handle read */
  if (ctrl->mode & BUFF_OPMODE_R_FIFO)
  {
    memcpy(item, (ctrl->buff + ctrl->head), ctrl->item_size);
    ctrl->head = (ctrl->head + ctrl->item_size) % ctrl->capacity;
  }
  else
  {
    ctrl->tail = (ctrl->capacity + ctrl->tail - ctrl->item_size) % ctrl->capacity;
    memcpy(item, (ctrl->buff + ctrl->tail), ctrl->item_size);
  }
  ctrl->stored -= ctrl->item_size;
  return EMBLIB32_OK;
}

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PRIVATE_Functions -->
*//*-----------------------------------------------------------------------*//**
* @} <!-- End: EmbLib32 -->
* @} <!-- End: Middlewares -->
* @} <!-- End: Buffer -->
*//*--------------------------------------------------------------------------*/
