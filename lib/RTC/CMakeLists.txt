add_library(RTC STATIC
  src/RtcDateTime.cpp
  src/RtcLocaleEnUs.cpp
  src/RtcUtility.cpp
)

target_include_directories(RTC
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(RTC
  PUBLIC
  hardware_gpio
  pico_time

  PRIVATE
  met_compile_options
)
