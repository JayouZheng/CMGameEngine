﻿add_executable(Example.DataType.MapTest MapTest.cpp)

TARGET_LINK_LIBRARIES(Example.DataType.MapTest ${HGL_CONSOLE_LIB})

add_executable(Example.DataType.PAttribTest PAttribTest.cpp)

TARGET_LINK_LIBRARIES(Example.DataType.PAttribTest ${HGL_CONSOLE_LIB})

add_executable(Example.DataType.StrCharTest strchr_test.cpp)

TARGET_LINK_LIBRARIES(Example.DataType.StrCharTest ${HGL_CONSOLE_LIB})

add_executable(Example.DataType.HexToUint HexToUintTest.cpp)
