# Plugin-wide metadata for the adaptor's automatic registration. ONE place,
# included by both entry points - the repo root (.gmpi build) and
# static_library/ (TIDE's static build) - so the two can never drift.
#
# rack_module_resources() bakes these into a generated header rather than
# passing them as -D defines, because values with spaces and parentheses do
# not survive MSBuild's define list.
#
# The category starts "Rack/" ON PURPOSE: SynthEdit's rack browser scope
# (ModuleScope::RackOnly, EditorLib/SynthEditAppBase.cpp) lists prefabs plus
# modules whose category starts "Rack", nothing else. Anywhere else and these
# modules exist but never show up as rack-compatible.
set(RACK_MODULE_ID_PREFIX "HetrickCV: ")
set(RACK_MODULE_CATEGORY  "Rack/HetrickCV")
set(RACK_MODULE_VENDOR    "HetrickCV (ported)")
