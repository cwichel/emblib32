/**
 *******************************************************************************
 * @file    test_emblib32_cobs.h
 * @author  Christian Wiche
 * @date    2024
 * @brief   COBS (Consistent Overhead Byte Stuffing) testing
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
#include "unity.h"

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

#define BUFFER_SIZE  COBS_MAX_ENCODED_SIZE(1024U)

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

typedef struct
{
  uint8_t   buff[BUFFER_SIZE];
  uint16_t  size;
} t_test_item;

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PRIVATE_Types -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PRIVATE_Data
* @{
*//*--------------------------------------------------------------------------*/

t_test_item encoded;
t_test_item decoded;

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PRIVATE_Data -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PRIVATE_Functions
* @{
*//*--------------------------------------------------------------------------*/

static void test_cobs_wk0_6(void);
static void test_cobs_wk7(void);
static void test_cobs_wk8(void);
static void test_cobs_wk9(void);

static void run_test(const t_test_item *data, const t_test_item *expected);

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PRIVATE_Functions -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PUBLIC_API
* @{
*//*--------------------------------------------------------------------------*/

int main(void)
{
  UNITY_BEGIN();
  
  RUN_TEST(test_cobs_wk0_6);
  RUN_TEST(test_cobs_wk7);
  RUN_TEST(test_cobs_wk8);
  RUN_TEST(test_cobs_wk9);
  
  UNITY_END();
  return 0;
}

void setUp(void)
{
  /* Not required */
}

void tearDown(void)
{
  /* Not required */
}

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PUBLIC_API -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PRIVATE_Functions
* @{
*//*--------------------------------------------------------------------------*/

static void test_cobs_wk0_6(void)
{
  /* Wikipedia examples: extra + 1 - 6 */
  struct
  {
    t_test_item data;
    t_test_item expected;
  } test[] = {
    {{"", 0}, {"\x01\00", 2}},  /* Extra case: zero length */
    {{"\x00", 1}, {"\x01\x01\00", 3}},
    {{"\x00\x00", 2}, {"\x01\x01\x01\00", 4}},
    {{"\x00\x11\x00", 3}, {"\x01\x02\x11\x01\x00", 5}},
    {{"\x11\x22\x00\x33", 4}, {"\x03\x11\x22\x02\x33\x00", 6}},
    {{"\x11\x22\x33\x44", 4}, {"\x05\x11\x22\x33\x44\x00", 6}},
    {{"\x11\x00\x00\x00", 4}, {"\x02\x11\x01\x01\x01\x00", 6}},
  };
  
  for (size_t idx = 0; idx < ARRAY_SIZE(test); idx++)
  {
    run_test(&test[idx].data, &test[idx].expected);
  }
}

static void test_cobs_wk7(void)
{
  /* Wikipedia example: 7 */
  t_test_item data;
  t_test_item expected;
  
  /* Prepare */
  data.size = 0xFEU;
  for (uint16_t idx = 0U; idx < data.size; idx++)
  {
    data.buff[idx] = (uint8_t)(idx + 1U);
    expected.buff[idx + 1U] = (uint8_t)(idx + 1U);
  }
  expected.buff[0] = 0xFFU;
  expected.size = COBS_MAX_ENCODED_SIZE(data.size);
  
  /* Run */
  run_test(&data, &expected);
}

static void test_cobs_wk8(void)
{
  /* Wikipedia example: 8 */
  t_test_item data;
  t_test_item expected;
  
  /* Prepare */
  data.size = 0xFFU;
  for (uint16_t idx = 0U; idx < data.size; idx++)
  {
    data.buff[idx] = (uint8_t)(idx);
    expected.buff[idx + 1U] = (uint8_t)(idx);
  }
  expected.buff[0] = 0x01U;
  expected.buff[1] = 0xFFU;
  expected.size = COBS_MAX_ENCODED_SIZE(data.size);
  
  /* Run */
  run_test(&data, &expected);
}

static void test_cobs_wk9(void)
{
  /* Wikipedia example: 9 */
  t_test_item data;
  t_test_item expected;
  
  /* Prepare */
  data.size = 0xFFU;
  for (uint16_t idx = 0U; idx < data.size; idx++)
  {
    data.buff[idx] = (uint8_t)(idx + 1U);
    expected.buff[idx + 1U] = (uint8_t)(idx + 1U);
  }
  expected.buff[0x00] = 0xFFU;
  expected.buff[0xFF] = 0x02U;
  expected.buff[0xFF + 1U] = 0xFFU;
  expected.size = COBS_MAX_ENCODED_SIZE(data.size);
  
  /* Run */
  run_test(&data, &expected);
}

static void run_test(const t_test_item *data, const t_test_item *expected)
{
  /* Validate expected encoding */
  cobs_encode(data->buff, data->size, encoded.buff, BUFFER_SIZE, &encoded.size);
  TEST_ASSERT_LESS_OR_EQUAL(expected->size, encoded.size);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(expected->buff, encoded.buff, encoded.size);
  
  /* Validate expected decoding */
  cobs_decode(encoded.buff, encoded.size, decoded.buff, BUFFER_SIZE, &decoded.size);
  TEST_ASSERT_EQUAL_UINT(data->size, decoded.size);
  if (data->size > 0)
  {
    TEST_ASSERT_EQUAL_UINT8_ARRAY(data->buff, decoded.buff, decoded.size);
  }
}

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PRIVATE_Functions -->
*//*-----------------------------------------------------------------------*//**
* @} <!-- End: EmbLib32 -->
* @} <!-- End: Middlewares -->
* @} <!-- End: COBS -->
*//*--------------------------------------------------------------------------*/
