cmake_minimum_required(VERSION 3.30)

project(FlipPan)

gmpi_plugin(
    PROJECT_NAME ${PROJECT_NAME}
    HAS_DSP HAS_GUI USE_STAGING
    FORMATS_LIST GMPI
    SOURCE_FILES
        FlipPan.cpp
        res/FlipPan.svg
)

# shared/src/FlipPan.cpp is NOT listed as a source. FlipPan.cpp #includes it, because
# upstream ships no header and including it is what runs the module's own
# createModel() -- which is what registers the plugin.
foreach(tgt ${PROJECT_NAME})
    if(TARGET ${tgt})
        # HetrickCV shares HetrickCV.hpp, HetrickUtilities.hpp, DSP/ and Gamma
        # across every module, so the upstream tree lives once in shared/
        # rather than being copied per module as Fundamental's is.
        target_include_directories(${tgt} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/../../shared
            ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/src)
        target_link_libraries(${tgt} PRIVATE gmpi_tinyxml2 SynthEditRackAdaptor)
        rack_module_resources(${tgt} ${CMAKE_CURRENT_SOURCE_DIR}/res)
    endif()
endforeach()
