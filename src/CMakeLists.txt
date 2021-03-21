set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

project(serialPortTools LANGUAGES CXX)
find_package(Qt5 COMPONENTS Widgets SerialPort REQUIRED)

include_directories(.)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie")

if(CMAKE_HOST_WIN32)
  add_compile_definitions(_TTY_WIN_)
	if(MSVC)
		set_target_properties(${PROJECT_NAME} PROPERTIES
			WIN32_EXECUTABLE YES
			LINK_FLAGS "/ENTRY:mainCRTStartup"
		)
	elseif(CMAKE_COMPILER_IS_GNUCXX)
			SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
	endif(MSVC)
endif()

add_executable(serialPortTools
  main.cpp
  utilities.cpp
  utilities.h
  
  main_window/main_window.cpp
  main_window/main_window.h
  main_window/main_window.ui

  main_window/about_dialog.cpp
  main_window/about_dialog.h
  main_window/about_dialog.ui
  
  com/com_tab.cpp
  com/com_tab.h
  com/com_tab.ui
  
  oscilloscope/oscilloscope_tab.cpp
  oscilloscope/oscilloscope_tab.h
  oscilloscope/oscilloscope_tab.ui
  oscilloscope/oscilloscope_screen.cpp
  oscilloscope/oscilloscope_screen.h

  linear_ccd/linear_ccd_tab.cpp
  linear_ccd/linear_ccd_tab.h
  linear_ccd/linear_ccd_tab.ui
  linear_ccd/line_view.cpp
  linear_ccd/line_view.h
  linear_ccd/wave_view.cpp
  linear_ccd/wave_view.h
  
  ccd/ccd_tab.cpp
  ccd/ccd_tab.h
  ccd/ccd_tab.ui
  ccd/pic_view.cpp
  ccd/pic_view.h

  resource/resource.qrc
  #resource/app.rc
)

target_link_libraries(serialPortTools PRIVATE Qt5::Widgets Qt5::SerialPort)