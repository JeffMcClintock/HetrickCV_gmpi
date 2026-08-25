// Rack's conventional plugin header name, which HetrickCV does not use.
//
// rack_module_resources() generates RackPanelResources.h with
// `#include "plugin.hpp"` -- upstream Rack's convention, and what
// VCV_Fundamental_gmpi's per-module vcv/plugin.hpp satisfies. HetrickCV's
// equivalent is HetrickCV.hpp: same role, different name (using namespace
// rack, plus the extern Model* declarations).
//
// So this forwards. Adding it here rather than teaching the generator a second
// filename keeps the generated header identical for every ported pack.
#pragma once
#include "HetrickCV.hpp"
