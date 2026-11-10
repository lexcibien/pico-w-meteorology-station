add_library(Adafruit_Unified_Sensor STATIC
  Adafruit_Sensor.cpp
)

target_include_directories(Adafruit_Unified_Sensor
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Adafruit_Unified_Sensor
  PRIVATE
  met_compile_options
)
