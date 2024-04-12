
if(TARGET ips4o)
    return()
endif()

find_package(TBB REQUIRED)
include(FetchContent)
FetchContent_Declare(
    ips4o
    GIT_REPOSITORY https://github.com/rbneiman/ips4o.git # forked to resolve TBB issue
    GIT_TAG b31cabdba42f51946af339ca3ce4f6fea9abb2c4
)
FetchContent_MakeAvailable(ips4o)

