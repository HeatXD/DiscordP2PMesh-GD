cmake_minimum_required(VERSION 3.16)

project(DiscordP2PMeshGD LANGUAGES CXX)

# Linked into the extension so only the Discord SDK and libjuice ship as separate binaries.
set(BUILD_SHARED_LIBS OFF)

add_subdirectory(godot-cpp)
add_subdirectory(DiscordP2PMesh)

add_library(discordp2pmesh_gd SHARED
    src/register_types.cpp
    src/discord_p2p_mesh_node.cpp
)

target_compile_features(discordp2pmesh_gd PRIVATE cxx_std_20)
target_include_directories(discordp2pmesh_gd PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/private")
target_link_libraries(discordp2pmesh_gd PRIVATE godot-cpp discordp2pmesh)

# godot-cpp exports the ".<platform>.<target>.<arch>" string Godot expects in the filename.
get_target_property(GODOTCPP_SUFFIX godot-cpp GODOTCPP_SUFFIX)

set(DPMESH_GD_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# $<1:...> stops multi-config generators from appending a per-config subdirectory.
set_target_properties(discordp2pmesh_gd PROPERTIES
    CXX_EXTENSIONS OFF
    CXX_VISIBILITY_PRESET ${GODOTCPP_SYMBOL_VISIBILITY}
    POSITION_INDEPENDENT_CODE ON
    BUILD_RPATH_USE_ORIGIN ON
    INSTALL_RPATH "$ORIGIN"
    PREFIX "lib"
    OUTPUT_NAME "discordp2pmesh${GODOTCPP_SUFFIX}"
    LIBRARY_OUTPUT_DIRECTORY "$<1:${DPMESH_GD_OUTPUT_DIR}>"
    RUNTIME_OUTPUT_DIRECTORY "$<1:${DPMESH_GD_OUTPUT_DIR}>"
    PDB_OUTPUT_DIRECTORY "$<1:${DPMESH_GD_OUTPUT_DIR}>"
)

add_custom_command(TARGET discordp2pmesh_gd POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${DPMESH_OUTPUT_DIR}" "${DPMESH_GD_OUTPUT_DIR}"
    COMMENT "Copying runtime dependencies next to the extension"
)
