# ==============================================================================
# Test CMake File
# ==============================================================================

# program source  code  --------------------------------------------------------
SET(PROGRAM_SRC
    main.cpp
    mainwindow.cpp
    )

SET(PROGRAM_UI
    mainwindow.ui
    )
include(${VTK_USE_FILE})
# final build ------------------------------------------------------------------
QT5_WRAP_UI(PROGRAM_SRC ${PROGRAM_UI})

ADD_EXECUTABLE(myprog ${PROGRAM_SRC})
QT5_USE_MODULES(myprog Core Widgets OpenGL)

TARGET_LINK_LIBRARIES(myprog 
    ${VTK_LIBRARIES}
    ${QT_LIBRARIES}
    ${OPEN_BABEL_LIB}
) 

INSTALL(TARGETS
        myprog
    RUNTIME DESTINATION
        bin
    )
