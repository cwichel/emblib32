#include <stdio.h>
#include <string.h>

#include "common.h"
#include "emblib32_bitmask.h"
#include "emblib32_buffer.h"
#include "emblib32_trace.h"

#define BUFF_SIZE  1024

const char *TRACE_TOPIC_NAME[TOPIC_NUM] = {
  "SYSTEM",
};

t_buff  ctrl;
uint8_t buff[BUFF_SIZE];

int main(void)
{
  size_t aux;
  
  printf("%d\n", BIT_MASK(2));
  printf("\n");
  
  trace_init(TRACE_LEVEL_ALL, TRACE_TOPICS_ALL);
  LFATAL  (TOPIC_SYSTEM, "Hello, World!");
  LERROR  (TOPIC_SYSTEM, "Hello, World!");
  LWARNING(TOPIC_SYSTEM, "Hello, World!");
  LINFO   (TOPIC_SYSTEM, "Hello, World!");
  LDEBUG  (TOPIC_SYSTEM, "Hello, World!");
  printf("\n");
  
  buff_init(&ctrl, buff, BUFF_SIZE, 1, BUFF_OPMODE_DEFAULT, true);
  buff_push_chunk(&ctrl, "Hello, World!", 13, &aux);
  printf("%d\n", buff_get_count(&ctrl));
  
  return 0;
}
