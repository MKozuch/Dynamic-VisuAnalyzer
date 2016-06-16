cmake_minimum_required(VERSION 2.8.8)
#set(CMAKE_GENERATOR "Visual Studio 14 2015 Win64")
set(CMAKE_GENERATOR "NMake Makefiles JOM Win64")

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW) # CMake 3.0
endif()
if(POLICY CMP0043)
  cmake_policy(SET CMP0043 NEW) # CMake 3.0
endif()
if(POLICY CMP0053)
  cmake_policy(SET CMP0053 NEW) # CMake 3.1
endif()

PROJECT(DynamicVisuAnalyzer)

set(VTK_DIR C:/VTK-7.0.0/bin)
set(ITK_DIR C:/lib/InsightToolkit-4.9.1/bin)
set (VTK_INSTALL_PREFIX C:/VTK-7.0.0/bin)

find_package(VTK 7.0.0 REQUIRED)
include(${VTK_USE_FILE})

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

if("${VTK_QT_VERSION}" STREQUAL "")
  message(FATAL_ERROR "VTK was not built with Qt")
endif()

# Set your files and resources here
set(Srcs
    main.cpp
    dynamicvisuanalyzer.cpp
    Widgets/dvawidget.cpp
    Widgets/dvafourpaneview.cpp
    Widgets/skullwidget.cpp
    Widgets/dvaprogrammablewidget.cpp
    dvadata.cpp
    )
set(Hdrs
    dynamicvisuanalyzer.h
    Widgets/dvawidget.h
    Widgets/dvafourpaneview.h
    Widgets/skullwidget.h
    Widgets/dvaprogrammablewidget.h
    dvadata.h
    )
set(MOC_Hdrs
    dynamicvisuanalyzer.h
    )
set(UIs
    dynamicvisuanalyzer.ui
    Widgets/dvafourpaneView.ui
    Widgets/skullwidget.ui
    Widgets/dvaprogrammablewidget.ui
    )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

if(VTK_QT_VERSION VERSION_GREATER "4")
  find_package(Qt5 COMPONENTS Widgets REQUIRED QUIET)
  qt5_wrap_ui(UI_Srcs ${UIs})

  # CMAKE_AUTOMOC in ON so the MocHdrs will be automatically wrapped.
  add_executable(DynamicVisuAnalyzer ${Srcs} ${Hdrs} ${UI_Srcs} ${MOC_Hdrs})
  qt5_use_modules(DynamicVisuAnalyzer Core Gui Widgets)
  target_link_libraries(DynamicVisuAnalyzer ${VTK_LIBRARIES}  ${ITK_LIBRARIES})
else()
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})
  # Use what VTK built with
  set(QT_QMAKE_EXECUTABLE ${VTK_QT_QMAKE_EXECUTABLE} CACHE FILEPATH "")
  set(QT_MOC_EXECUTABLE ${VTK_QT_MOC_EXECUTABLE} CACHE FILEPATH "")
  set(QT_UIC_EXECUTABLE ${VTK_QT_UIC_EXECUTABLE} CACHE FILEPATH "")
  qt4_wrap_ui(UI_Srcs ${UIs})

  add_executable(DynamicVisuAnalyzer ${Srcs} ${Hdrs} ${UI_Srcs} ${MOC_Hdrs})
  target_link_libraries(DynamicVisuAnalyzer ${QT_LIBRARIES} ${VTK_LIBRARIES}  ${ITK_LIBRARIES})
endif()
