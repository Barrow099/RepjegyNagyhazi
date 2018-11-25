cmake_minimum_required(VERSION 3.12)
project(nagyhazi C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(c-econio)
include_directories(FlightData)
include_directories(Menu)

add_executable(nagyhazi
        c-econio/econio.c
        c-econio/econio.h
        FlightData/FlightData.c
        FlightData/FlightData.h
        Menu/intelligent_menu.c
        Menu/intelligent_menu.h
        Menu/menu.c
        Menu/menu.h
        GenericList.c
        GenericList.h
        main.c
        string-utils.c
        string-utils.h
        FlightData/FlightBook.c FlightData/FlightBook.h FlightData/Structs.h)
target_link_libraries(nagyhazi m)
