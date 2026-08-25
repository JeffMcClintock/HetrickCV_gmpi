# Upstream licences — what a host bundling this must ship

Two licences apply, and **only one of them is attribution-free**.

## HetrickCV — CC0-1.0

`shared/src/` and `shared/res/` are Michael Hetrick's HetrickCV
(<https://github.com/mhetrick/hetrickcv>), byte-for-byte, under **CC0 1.0
Universal** — a public-domain dedication covering **code and panel artwork
alike**. Full text: [shared/HETRICKCV-LICENSE.txt](shared/HETRICKCV-LICENSE.txt).
No attribution is required by the licence. Nothing here is GPL.

## Gamma — MIT, and this one DOES require attribution

`shared/Gamma/` is Gamma (<https://github.com/mhetrick/Gammin>, a fork of
Lance Putnam's Gamma), **MIT, © 2006 Lance Putnam**. Full text:
[shared/GAMMA-LICENSE.txt](shared/GAMMA-LICENSE.txt).

**This is not optional and it is easy to miss.** HetrickCV's own repo is CC0,
so the pack reads as attribution-free — but `shared/src/HetrickUtilities.hpp`
includes Gamma, and every module includes that. **So every module in this repo
depends on MIT code whose copyright and permission notice must be retained in
redistributions**, binaries included.

A host that links `HetrickCV_static` is redistributing Gamma. It must ship the
MIT notice somewhere a user can reach.

## VCV artwork — referenced, never shipped

`HCVThemedRogan` in `shared/src/HetrickUtilities.hpp` names
`res/ComponentLibrary/Rogan1P*.svg`, which is VCV's own artwork and is NOT
CC0. **Nothing loads or ships it here**: in SynthEdit_Rack_Adaptor
`asset::system()` and `Svg::load()` are mock stubs, and `RackEditor` draws its
own knobs from the module's reported geometry. The reference exists in source
only. Recorded so it is not mistaken for a breach later.
