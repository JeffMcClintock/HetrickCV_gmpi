// Dust -- the entire port.
//
// Upstream's own registration line puts the GMPI plugin in the factory, so
// there is nothing to write here. See SynthEdit_Rack_Adaptor.
//
// ../../shared/src/Dust.cpp is HetrickCV's file byte-for-byte (CC0-1.0, Michael
// Hetrick). It is #included rather than linked because upstream ships no
// header, and because including it is what runs its createModel() in a
// translation unit where the adaptor is already declared.
#include "RackModule.h"
#include "src/Dust.cpp"
