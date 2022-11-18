project (client)

# Tools for using NK parser.
include (platform/nk)

# Set compile flags.
project_header_default ("STANDARD_GNU_11:YES" "STRICT_WARNINGS:NO")
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -Wno-deprecated-declarations")

add_executable (Client "src/main.cpp" "src/subscriber.cpp" "src/IPCExchanger.cpp")
add_dependencies (Client echo_idl_files echo_client_edl_files)
add_dependencies (Client subscriber_edl_files)
add_dependencies (Client subscriber_edl_files echo_idl_files echo_client_edl_files)
target_link_libraries(Client 
                      ${mosquitto_CXX_LIB}
                      ${vfs_CLIENT_LIB}
)

set_target_properties (Client PROPERTIES ${vfs_ENTITY}_REPLACEMENT "")
set_target_properties (Client PROPERTIES LINK_FLAGS ${VULN_LDFLAGS})
