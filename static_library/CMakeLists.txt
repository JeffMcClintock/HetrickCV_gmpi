# ---------------------------------------------------------------------------
# STATIC entry point: every ported module as an OBJECT library, for a
# SynthEdit-based host that links its module set into its own binary.
#
# TIDE Rack is the consumer this exists for (PLAN constraint 7 is a fixed,
# statically registered module set with no scan, so a .gmpi bundle is invisible
# to it). It fetches this repo with SOURCE_SUBDIR static_library, so the repo
# root never runs; the host already has the SDKs.
#
# OBJECT libraries, not a STATIC archive: registration is a static initializer
# per module TU, and archive members nothing references get dead-stripped.
#
# LICENSING: HetrickCV's own sources are CC0-1.0 (Michael Hetrick) -- code AND
# panel artwork. But shared/Gamma is MIT ((c) 2006 Lance Putnam) and
# HetrickUtilities.hpp includes it, so EVERY module here depends on MIT code
# whose notice must be retained. A host bundling this must ship that notice.
# See UPSTREAM-LICENSE.md. Nothing here is GPL.
#
# THE CONSUMER PROVIDES, before add_subdirectory/FetchContent of this dir:
#   GMPI_SDK, GMPI_UI_SDK, RACK_STATIC_TINYXML2_INCLUDE, and the
#   SynthEditRackAdaptorStatic + RackAdaptorStaticRegistration targets from
#   SynthEdit_Rack_Adaptor configured with RACK_ADAPTOR_STATIC_HOST=ON.
#
# The consumer then links ONE target: HetrickCV_static.
# ---------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.30)

project(HetrickCV_static LANGUAGES CXX)

foreach(_required GMPI_SDK GMPI_UI_SDK RACK_STATIC_TINYXML2_INCLUDE)
    if("${${_required}}" STREQUAL "")
        message(FATAL_ERROR "HetrickCV static build: ${_required} is not set - see static_library/CMakeLists.txt for the consumer contract")
    endif()
endforeach()
if(NOT TARGET SynthEditRackAdaptorStatic)
    message(FATAL_ERROR "HetrickCV static build: SynthEditRackAdaptorStatic target missing - configure SynthEdit_Rack_Adaptor with RACK_ADAPTOR_STATIC_HOST=ON first")
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/RackPanelResources.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/RackModuleMetadata.cmake)

# The modules that COMPILE against the adaptor as of 2026-08-25: 66 of
# upstream's 79 source files. The 13 excluded are listed in README.md with the
# symbol each one still wants -- they are small named gaps in the adaptor's
# mock, not structural problems.
set(_rack_static_modules
    2To4 ASR AmplitudeShaper AnalogToDigital BinaryGate BinaryNoise
    Boolean3 Chaos1Op Chaos2Op Chaos3Op ChaoticAttractors ClockToPhasor
    ClockedNoise Comparator Contrast Crackle DataCompander Delta
    DigitalToAnalog Dust FBSineChaos FlipFlop FlipPan GateDelay
    Gingerbread LogicCombine MidSide MinMax Normals PhaseDrivenSequencer
    PhaseDrivenSequencer32 PhasorAnalyzer PhasorEuclidean PhasorFreezer
    PhasorGates PhasorGates32 PhasorGates64 PhasorGeometry PhasorHumanizer
    PhasorMixer PhasorOctature PhasorProbability PhasorQuadrature
    PhasorRandom PhasorRanger PhasorReset PhasorRhythmGroup PhasorShape
    PhasorShift PhasorStutter PhasorSubstepShape PhasorSwing
    PhasorTimetable PhasorToClock PhasorToLFO PhasorToRandom
    PhasorToWaveforms PolymetricPhasors Probability RandomGates Rotator
    Rungler Scanner TrigShaper VectorMix XYToPolar)

# HETRICKCV_EXCLUDE_MODULES -- drop modules from the set before building.
#
# Exists because a Rack module declares its Model as a GLOBAL symbol
# (`Model* modelFoo`), so two packs shipping a module of the same name collide
# at link with a duplicate-symbol error that names neither pack. Measured
# 2026-08-25: HetrickCV and VCV Fundamental overlap on exactly ONE name,
# MidSide, out of 66 and 38.
#
# The consumer decides which pack wins, because only it knows what else is
# linked. TIDE excludes it here when TIDE_VCV_FUNDAMENTAL is also ON.
set(HETRICKCV_EXCLUDE_MODULES "" CACHE STRING "module names to leave out of HetrickCV_static (e.g. names that collide with another pack)")

if(HETRICKCV_EXCLUDE_MODULES)
    foreach(_x ${HETRICKCV_EXCLUDE_MODULES})
        if(NOT _x IN_LIST _rack_static_modules)
            message(WARNING "HetrickCV: excluded module '${_x}' is not in the set - check the spelling, it is being ignored")
        endif()
        list(REMOVE_ITEM _rack_static_modules ${_x})
    endforeach()
    message(STATUS "HetrickCV: excluding ${HETRICKCV_EXCLUDE_MODULES}")
endif()

add_library(HetrickCV_static INTERFACE)

set(_modules_dir ${CMAKE_CURRENT_SOURCE_DIR}/../modules)
set(_shared_dir  ${CMAKE_CURRENT_SOURCE_DIR}/../shared)

# SHARED IMPLEMENTATION UNITS -- not modules, and easy to miss because nothing
# references them until the LINK. HetrickCV keeps several class bodies in their
# own .cpp (HCVCuspMap::generate() is the one that surfaced this), so compiling
# only the module wrappers leaves them undefined. Syntax-checking every module
# passes and the link then fails, which is exactly what happened.
# The Gamma set is upstream HetrickCV's own -- its Makefile compiles exactly
# these three (arr, Domain, scl) for the Rack plugin. Gamma's CMakeLists names
# four more (DFT, FFT_fftpack, fftpack++1, fftpack++2) that the plugin build
# does not use; left out until something needs them, rather than carrying an
# FFT nobody calls.
add_library(hcv_shared OBJECT
    ${_shared_dir}/src/HetrickUtilities.cpp
    ${_shared_dir}/src/DSP/HCVChaos.cpp
    ${_shared_dir}/src/DSP/Phasors/HCVPhasorAnalyzers.cpp
    ${_shared_dir}/src/DSP/Phasors/HCVPhasorEffects.cpp
    ${_shared_dir}/Gamma_src/arr.cpp
    ${_shared_dir}/Gamma_src/Domain.cpp
    ${_shared_dir}/Gamma_src/scl.cpp
)
target_include_directories(hcv_shared PRIVATE
    ${_shared_dir}
    ${_shared_dir}/src
    ${GMPI_SDK}
    ${GMPI_SDK}/Core
    ${GMPI_UI_SDK}
    ${RACK_STATIC_TINYXML2_INCLUDE}
)
target_link_libraries(hcv_shared PRIVATE SynthEditRackAdaptorStatic)
set_target_properties(hcv_shared PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON)
target_sources(HetrickCV_static INTERFACE $<TARGET_OBJECTS:hcv_shared>)

foreach(_m ${_rack_static_modules})
    set(_tgt hcvmod_${_m})

    add_library(${_tgt} OBJECT ${_modules_dir}/${_m}/${_m}.cpp)

    # shared/ once, not per module: HetrickCV's modules all include
    # HetrickCV.hpp, HetrickUtilities.hpp, DSP/ and Gamma.
    target_include_directories(${_tgt} PRIVATE
        ${_shared_dir}
        ${_shared_dir}/src
        ${GMPI_SDK}
        ${GMPI_SDK}/Core
        ${GMPI_UI_SDK}
        ${RACK_STATIC_TINYXML2_INCLUDE}
    )
    target_link_libraries(${_tgt} PRIVATE SynthEditRackAdaptorStatic)

    if(RACK_ADAPTOR_TRACE)
        target_compile_definitions(${_tgt} PRIVATE RACK_ADAPTOR_TRACE=1)
    endif()

    set_target_properties(${_tgt} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        POSITION_INDEPENDENT_CODE ON)

    rack_module_resources(${_tgt} ${_modules_dir}/${_m}/res)

    # INTERFACE sources, not target_link_libraries: an OBJECT library's objects
    # do not propagate through an INTERFACE library to its consumers, whereas
    # INTERFACE sources land on each consuming target's link line -- which is
    # the no-dead-stripping guarantee this file exists to give.
    target_sources(HetrickCV_static INTERFACE $<TARGET_OBJECTS:${_tgt}>)
endforeach()

# Exactly one copy of the deferred-registration queue per binary. If the host
# also links VCV_Fundamental_static, only ONE aggregate may contribute it or
# the objects are duplicated on the link line.
#
# A GLOBAL PROPERTY, not a cache variable, and that distinction cost a build:
# a CACHE INTERNAL guard survives between configures, so the SECOND configure
# of the same tree skipped the registration object entirely and every module's
# autoRegisterModel() came out undefined. A global property is per-configure,
# which is the scope this guard actually wants.
get_property(_rack_reg_linked GLOBAL PROPERTY RACK_STATIC_REGISTRATION_LINKED)
if(NOT _rack_reg_linked)
    target_sources(HetrickCV_static INTERFACE $<TARGET_OBJECTS:RackAdaptorStaticRegistration>)
    set_property(GLOBAL PROPERTY RACK_STATIC_REGISTRATION_LINKED ON)
endif()

target_link_libraries(HetrickCV_static INTERFACE SynthEditRackAdaptorStatic)

list(LENGTH _rack_static_modules _n)
message(STATUS "HetrickCV_static: ${CMAKE_CURRENT_LIST_DIR} - ${_n} module object libraries")
