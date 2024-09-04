#------------------------------------------------
# Append items to a list
# param LIST  : the list to append to
# param PREFIX: the prefix to append to each item
# param SUFFIX: the suffix to append to each item
# param SOURCE: the items to append
function(append_list)
  set(ARG_OPT "")
  set(ARG_SGL LIST PREFIX SUFFIX)
  set(ARG_MUL SOURCE)
  cmake_parse_arguments(ARG "${ARG_OPT}" "${ARG_SGL}" "${ARG_MUL}" ${ARGN})
  # Generate list
  set(tmp "${${ARG_LIST}}")
  foreach(src ${ARG_SOURCE})
    set(tmp "${tmp} ${ARG_PREFIX}${src}${ARG_SUFFIX}")
  endforeach()
  # Update parent scope
  set(${ARG_LIST} ${tmp} PARENT_SCOPE)
endfunction()

#------------------------------------------------
# Converts the target output to a different format
# param TARGET   : the target to generate the file from
# param EXTENSION: the extension of the generated file
# param FORMATTER: the format of the generated file (OBJCOPY)
# param GENERATED: return the name of the generated file
function(convert_to TARGET EXTENSION FORMATTER GENERATED)
  # Define output paths
  get_filename_component(BASE "${PROJECT_BINARY_DIR}/${TARGET}" ABSOLUTE)
  set(SRC "${BASE}${CMAKE_EXECUTABLE_SUFFIX}")
  set(DST "${BASE}${EXTENSION}")
  # Propagate filename if requested
  if(GENERATED)
    set(${GENERATED} "${DST}" PARENT_SCOPE)
  endif()
  # Add generation command
  add_custom_command(
      TARGET ${TARGET} POST_BUILD
      COMMAND ${CMAKE_OBJCOPY} -O ${FORMATTER} ${SRC} ${DST}
      COMMENT "Generating ${FORMATTER} file for ${TARGET}..."
      BYPRODUCTS "${DST}"
  )
endfunction()

#------------------------------------------------
# Converts the output to a binary file
# param TARGET   : the target to generate the file from
# param GENERATED: return the name of the generated file
function(convert_to_bin TARGET GENERATED)
  convert_to(${TARGET} ".bin" "binary" OUTPUT)
  if(GENERATED)
    set(${GENERATED} "${OUTPUT}" PARENT_SCOPE)
  endif()
endfunction()

#------------------------------------------------
# Converts the output to a hex file
# param TARGET   : the target to generate the file from
# param GENERATED: return the name of the generated file
function(convert_to_hex TARGET GENERATED)
  convert_to(${TARGET} ".hex" "ihex" OUTPUT)
  if(GENERATED)
    set(${GENERATED} "${OUTPUT}" PARENT_SCOPE)
  endif()
endfunction()

#------------------------------------------------
# Converts the output to a srec file
# param TARGET   : the target to generate the file from
# param GENERATED: return the name of the generated file
function(convert_to_srec TARGET GENERATED)
  convert_to(${TARGET} ".srec" "srec" OUTPUT)
  if(GENERATED)
    set(${GENERATED} "${OUTPUT}" PARENT_SCOPE)
  endif()
endfunction()

# -----------------------------------------------
# Generate a signed binary/hex file for the target
# param TARGET : the target to generate the file from
# param ADDRESS: the address to start the binary at
# param SEC_BIN: return the name of the generated binary file
# param SEC_HEX: return the name of the generated hex file
function(sign_binaries TARGET ADDRESS SEC_BIN SEC_HEX)
  # Ensure binary generation is in place
  convert_to_bin(${TARGET} NSEC_BIN)
  # Define output paths
  get_filename_component(OUT_PATH "${NSEC_BIN}" DIRECTORY ABSOLUTE)
  get_filename_component(OUT_NAME "${NSEC_BIN}" NAME_WLE)
  set(SBIN "${OUT_PATH}/${OUT_NAME}.secure.bin")
  set(SHEX "${OUT_PATH}/${OUT_NAME}.secure.hex")
  # Propagate filename if requested
  if(SEC_BIN)
    set(${SEC_BIN} "${SBIN}" PARENT_SCOPE)
  endif()
  if(SEC_HEX)
    set(${SEC_HEX} "${SHEX}" PARENT_SCOPE)
  endif()
  # Add generation command
  add_custom_command(
      TARGET ${TARGET} POST_BUILD
      COMMAND ${SIGNER_TOOL} -bin ${NSEC_BIN} -nk -of 0x80000000 -t fsbl -hv 2.1 -s -o ${SBIN} -dump ${SBIN}
      COMMAND ${CMAKE_OBJCOPY} -I binary ${SBIN} --change-address ${ADDRESS} -O ihex ${SHEX}
      COMMENT "Generating signed binaries for ${TARGET}..."
      BYPRODUCTS "${BIN_SEC}" "${HEX_SEC}"
      DEPENDS ${BIN_NSEC}
  )
endfunction()

#------------------------------------------------
# Print the size of a target after build
# param TARGET: the target to print the size of
function(print_size TARGET)
  add_custom_command(
      TARGET ${TARGET} POST_BUILD
      COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:${TARGET}>
      COMMENT "Target sizes:"
  )
endfunction()

#------------------------------------------------
# Create a new target to print the size of a target at any time
# param TARGET: the target to print the size of
function(add_print_size_target TARGET)
  add_custom_target(${TARGET}_size
      ALL COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:${TARGET}>
      COMMENT "Target sizes:"
      DEPENDS ${TARGET}
  )
endfunction()
