project(plugin_demo LANGUAGES C)

add_library(plugin_demo SHARED
  dynamic_threshold.c
)