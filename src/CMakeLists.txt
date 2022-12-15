find_package(Boost REQUIRED COMPONENTS regex)
include_directories(${Boost_INCLUDE_DIRS})

rock_library(satcomms_kvh
    SOURCES SatcommsStatusManager.cpp
    HEADERS SatcommsStatusManager.hpp SatcommsStatus.hpp
    DEPS_PKGCONFIG base-types base-logging
    DEPS_CMAKE CURL)

target_link_libraries(satcomms_kvh ${Boost_LIBRARIES})

rock_executable(satcomms_kvh_bin Main.cpp
    DEPS satcomms_kvh
    DEPS_CMAKE CURL)
