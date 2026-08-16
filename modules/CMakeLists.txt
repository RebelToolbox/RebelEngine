# CMakeLists.txt used for IDE support.

# Changes to project source files need to be applied to the parent directory.
set(parent_source_files ${project_source_files})

# Add modules that build third-party libraries.
add_subdirectory(bullet)
add_subdirectory(cvtt)
add_subdirectory(enet)
add_subdirectory(etc)
add_subdirectory(freetype)
add_subdirectory(jpg)
add_subdirectory(mbedtls)
add_subdirectory(minimp3)
add_subdirectory(noise_texture)
add_subdirectory(ogg)
add_subdirectory(pvr)
add_subdirectory(raycast)
add_subdirectory(recast)
add_subdirectory(regex)
add_subdirectory(squish)
add_subdirectory(stb_vorbis)
add_subdirectory(svg)
add_subdirectory(theora)
add_subdirectory(tinyexr)
add_subdirectory(upnp)
add_subdirectory(vhacd)
add_subdirectory(vorbis)
add_subdirectory(webp)
add_subdirectory(websocket)
add_subdirectory(xatlas_unwrap)

# Apply changes to project source files.
set(project_source_files ${parent_source_files} PARENT_SCOPE)
