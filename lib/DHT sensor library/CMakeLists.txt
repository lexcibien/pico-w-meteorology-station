add_library(DHT_Sensor_Library STATIC
  DHT.cpp
  DHT_U.cpp
)

target_include_directories(DHT_Sensor_Library
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(DHT_Sensor_Library
  PUBLIC
  pico_stdlib
  Adafruit_Unified_Sensor
)
