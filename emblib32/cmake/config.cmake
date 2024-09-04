#------------------------------------------------
set(FLAGS
  O0 g3 pedantic MMD MP m32
)

set(DEFINES
  DEBUG HOST
)

set(WARNINGS
  all extra write-strings init-self cast-qual pointer-arith strict-aliasing
  format=2 uninitialized missing-declarations no-long-long no-unused-parameter
  no-variadic-macros no-format-extra-args no-conversion
)

#------------------------------------------------
append_list(LIST CMAKE_C_FLAGS PREFIX "-"   SOURCE ${FLAGS})
append_list(LIST CMAKE_C_FLAGS PREFIX "-D"  SOURCE ${DEFINES})
append_list(LIST CMAKE_C_FLAGS PREFIX "-W"  SOURCE ${WARNINGS})

#------------------------------------------------
# message(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
