/**
 *******************************************************************************
 * @file    test_emblib32_cobs.c
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
#include "emblib32_rand.h"
#include "emblib32_trace.h"

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

#define TRACE(...)  trace("TEST", TRACE_LEVEL_DEBUG, 0U, "COBS", NULL, 0U, __VA_ARGS__)

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

static void test_cobs_encode_validation(void);
static void test_cobs_decode_validation(void);

static void test_cobs_wk0_6(void);
static void test_cobs_wk7(void);
static void test_cobs_wk8(void);
static void test_cobs_wk9(void);
static void test_cobs_wk10(void);
static void test_cobs_wk11(void);

static void test_cobs_multi_block(void);

static void run_test(const t_test_item *data, const t_test_item *expected);

/*-------------------------------------------------------------------------*//**
* @} <!-- End: PRIVATE_Functions -->
*//*-----------------------------------------------------------------------*//**
* @addtogroup PUBLIC_API
* @{
*//*--------------------------------------------------------------------------*/

int main(void)
{
  trace_init(TRACE_LEVEL_ALL, TRACE_TOPICS_ALL);
  
  UNITY_BEGIN();
  
  RUN_TEST(test_cobs_encode_validation);
  RUN_TEST(test_cobs_decode_validation);
  
  RUN_TEST(test_cobs_wk0_6);
  RUN_TEST(test_cobs_wk7);
  RUN_TEST(test_cobs_wk8);
  RUN_TEST(test_cobs_wk9);
  RUN_TEST(test_cobs_wk10);
  RUN_TEST(test_cobs_wk11);
  
  RUN_TEST(test_cobs_multi_block);
  
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

static void test_cobs_encode_validation(void)
{
  t_test_item dec = {"\x11\x22\x00\x33", 4U};
  t_test_item enc = { 0U };
  
  TRACE("Running test: Encoder validations");
  
  /* Null pointers */
  TEST_ASSERT_EQUAL(EMBLIB32_ERROR_PARAMETER, cobs_encode(NULL    , dec.size, enc.buff, BUFFER_SIZE, &enc.size));
  TEST_ASSERT_EQUAL(EMBLIB32_ERROR_PARAMETER, cobs_encode(dec.buff, dec.size, NULL    , BUFFER_SIZE, &enc.size));
  TEST_ASSERT_EQUAL(EMBLIB32_ERROR_PARAMETER, cobs_encode(dec.buff, dec.size, enc.buff, BUFFER_SIZE, NULL));
  
  /* Invalid size */
  TEST_ASSERT_EQUAL(EMBLIB32_ERROR_PARAMETER, cobs_encode(dec.buff, dec.size, enc.buff, 0U, &enc.size));
  
  /* Buffer overflow */
  TEST_ASSERT_EQUAL(EMBLIB32_ERROR_BUFFER_OVERFLOW, cobs_encode(dec.buff, dec.size, enc.buff, 4U, &enc.size));  /* Not enough space for encoded body */
  TEST_ASSERT_EQUAL(EMBLIB32_ERROR_BUFFER_OVERFLOW, cobs_encode(dec.buff, dec.size, enc.buff, 5U, &enc.size));  /* Not enough space for final zero */
}

static void test_cobs_decode_validation(void)
{
  t_test_item dec = { 0U };
  t_test_item enc = {"\x05\x11\x22\x33\x44\x00", 6U};
  
  TRACE("Running test: Decoder validations");
  
  /* Null pointers */
  TEST_ASSERT_EQUAL(EMBLIB32_ERROR_PARAMETER, cobs_decode(NULL    , enc.size, dec.buff, BUFFER_SIZE, &dec.size));
  TEST_ASSERT_EQUAL(EMBLIB32_ERROR_PARAMETER, cobs_decode(enc.buff, enc.size, NULL    , BUFFER_SIZE, &dec.size));
  TEST_ASSERT_EQUAL(EMBLIB32_ERROR_PARAMETER, cobs_decode(enc.buff, enc.size, dec.buff, BUFFER_SIZE, NULL));
  
  /* Invalid size */
  TEST_ASSERT_EQUAL(EMBLIB32_ERROR_PARAMETER, cobs_decode(enc.buff, enc.size, dec.buff, 0U, &dec.size));
  
  /* Buffer overflow */
  TEST_ASSERT_EQUAL(EMBLIB32_ERROR_BUFFER_OVERFLOW, cobs_decode(enc.buff, enc.size, dec.buff, 3U, &dec.size));  /* Not enough space for decoded body */
  
  /* Decoding error */
  enc.buff[3U] = 0x00U;
  TEST_ASSERT_EQUAL(EMBLIB32_ERROR_DECODE, cobs_decode(enc.buff, enc.size, dec.buff, BUFFER_SIZE, &dec.size));
}

static void test_cobs_wk0_6(void)
{
  /* Wikipedia examples: extra + 1 - 6 */
  struct
  {
    t_test_item data;
    t_test_item expected;
  } test[] = {
    {{"", 0U}, {"\x01\00", 2U}},  /* Extra case: zero length */
    {{"\x00", 1U}, {"\x01\x01\00", 3U}},
    {{"\x00\x00", 2U}, {"\x01\x01\x01\00", 4U}},
    {{"\x00\x11\x00", 3U}, {"\x01\x02\x11\x01\x00", 5U}},
    {{"\x11\x22\x00\x33", 4U}, {"\x03\x11\x22\x02\x33\x00", 6U}},
    {{"\x11\x22\x33\x44", 4U}, {"\x05\x11\x22\x33\x44\x00", 6U}},
    {{"\x11\x00\x00\x00", 4U}, {"\x02\x11\x01\x01\x01\x00", 6U}},
  };
  
  for (size_t idx = 0; idx < ARRAY_SIZE(test); idx++)
  {
    TRACE("Running test: wk%u", idx);
    run_test(&test[idx].data, &test[idx].expected);
  }
}

static void test_cobs_wk7(void)
{
  /* Wikipedia example: 7 */
  t_test_item data = { 0 };
  t_test_item expected = { 0 };
  
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
  TRACE("Running test: wk7");
  run_test(&data, &expected);
}

static void test_cobs_wk8(void)
{
  /* Wikipedia example: 8 */
  t_test_item data = { 0 };
  t_test_item expected = { 0 };
  
  /* Prepare */
  data.size = 0xFFU;
  for (uint16_t idx = 0U; idx < data.size; idx++)
  {
    data.buff[idx] = (uint8_t)(idx);
    expected.buff[idx + 1U] = (uint8_t)(idx);
  }
  expected.buff[0U] = 0x01U;
  expected.buff[1U] = 0xFFU;
  expected.size = COBS_MAX_ENCODED_SIZE(data.size);
  
  /* Run */
  TRACE("Running test: wk8");
  run_test(&data, &expected);
}

static void test_cobs_wk9(void)
{
  /* Wikipedia example: 9 */
  t_test_item data = { 0 };
  t_test_item expected = { 0 };
  
  /* Prepare */
  data.size = 0xFFU;
  for (uint16_t idx = 0U; idx < data.size; idx++)
  {
    data.buff[idx] = (uint8_t)(idx + 1U);
    expected.buff[idx + 1U] = (uint8_t)(idx + 1U);
  }
  expected.buff[0U]   = 0xFFU;
  expected.buff[255U] = 0x02U;
  expected.buff[256U] = 0xFFU;
  expected.size = COBS_MAX_ENCODED_SIZE(data.size);
  
  /* Run */
  TRACE("Running test: wk9");
  run_test(&data, &expected);
}

static void test_cobs_wk10(void)
{
  /* Wikipedia example: 10 */
  t_test_item data = { 0 };
  t_test_item expected = { 0 };
  
  /* Prepare */
  data.size = 0xFFU;
  for (uint16_t idx = 0U; idx < data.size; idx++)
  {
    data.buff[idx] = (uint8_t)(idx + 2U);
    expected.buff[idx + 1U] = (uint8_t)(idx + 2U);
  }
  expected.buff[0U]   = 0xFFU;
  expected.buff[255U] = 0x01U;
  expected.buff[256U] = 0x01U;
  expected.size = COBS_MAX_ENCODED_SIZE(data.size);
  
  /* Run */
  TRACE("Running test: wk10");
  run_test(&data, &expected);
}

static void test_cobs_wk11(void)
{
  /* Wikipedia example: 11 */
  t_test_item data = { 0 };
  t_test_item expected = { 0 };
  
  /* Prepare */
  data.size = 0xFFU;
  for (uint16_t idx = 0U; idx < data.size; idx++)
  {
    data.buff[idx] = (uint8_t)(idx + 3U);
    expected.buff[idx + 1U] = (uint8_t)(idx + 3U);
  }
  expected.buff[0U]   = 0xFEU;
  expected.buff[254U] = 0x02U;
  expected.buff[255U] = 0x01U;
  expected.size = COBS_MAX_ENCODED_SIZE(data.size);
  
  /* Run */
  TRACE("Running test: wk11");
  run_test(&data, &expected);
}

static void test_cobs_multi_block(void)
{
  uint8_t aux;
  t_test_item data = { 0U };
  
  data.size = 1024U;
  
  /* Run (all-zero) */
  TRACE("Running test: All-zero multi-block");
  run_test(&data, NULL);
  
  /* Run (non-zero) */
  aux = 0U;
  for (uint16_t idx = 0U; idx < data.size; idx++)
  {
    aux++;
    aux = (aux == 0U) ? 1U : aux;
    data.buff[idx] = aux;
  }
  TRACE("Running test: Non-zero multi-block");
  run_test(&data, NULL);
  
  /* Run (Random) */
  for (uint16_t idx = 0U; idx < data.size; idx++)
  {
    data.buff[idx] = (uint8_t)rand_get(0x00, 0xFF);
  }
  TRACE("Running test: Random multi-block");
  run_test(&data, NULL);
}

static void run_test(const t_test_item *data, const t_test_item *expected)
{
  /* Validate expected encoding */
  cobs_encode(data->buff, data->size, encoded.buff, BUFFER_SIZE, &encoded.size);
  if (expected)
  {
    TEST_ASSERT_LESS_OR_EQUAL(expected->size, encoded.size);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected->buff, encoded.buff, encoded.size);
  }
  
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
