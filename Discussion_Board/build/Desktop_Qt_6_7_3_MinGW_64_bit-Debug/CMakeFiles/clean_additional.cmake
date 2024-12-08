# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Discussion_Board_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Discussion_Board_autogen.dir\\ParseCache.txt"
  "Discussion_Board_autogen"
  )
endif()
