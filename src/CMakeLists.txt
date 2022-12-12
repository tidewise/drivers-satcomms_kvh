rock_library(satcomms_kvh
    SOURCES Dummy.cpp
    HEADERS Dummy.hpp)

rock_executable(satcomms_kvh_bin Main.cpp
    DEPS satcomms_kvh)

