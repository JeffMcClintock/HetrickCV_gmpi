# HetrickCV_gmpi

Michael Hetrick's **HetrickCV** modules, ported to run as SynthEdit / GMPI
plugins via [SynthEdit_Rack_Adaptor](https://github.com/JeffMcClintock/SynthEdit_Rack_Adaptor).
The DSP is upstream's, unmodified.

**Licensing in one line: HetrickCV is CC0 (code and artwork), but it depends on
Gamma, which is MIT and requires its notice be retained.** Read
[UPSTREAM-LICENSE.md](UPSTREAM-LICENSE.md) before shipping anything.

## Layout

    shared/src/     HetrickCV's source, byte-for-byte (CC0)
    shared/Gamma/   Gamma (MIT) -- upstream keeps it as a submodule; vendored here
    shared/res/     panel art (CC0)
    modules/<Name>/ two-line wrapper + that module's res/, one dir per module
    static_library/ OBJECT-library aggregate for a statically-linking host (TIDE)
    cmake/          shared panel-resource and metadata helpers

`shared/` is one tree rather than copied per module, which is the one place
this repo's layout differs from `VCV_Fundamental_gmpi`: HetrickCV's modules all
include `HetrickCV.hpp`, `HetrickUtilities.hpp`, `DSP/` and Gamma.

## What is here: 66 of upstream's 79 source files

Measured 2026-08-25 by compiling every one against the adaptor (syntax-only,
C++23). The rest are excluded because they do not compile yet:

    BinaryCounter Bitshift BlankPanel Exponent GateJunction GateJunctionExp HetrickCV HetrickUtilities PhasorBurstGen PhasorDivMult PhasorGen PhasorSplitter Waveshaper 

Two of those are not modules at all — `HetrickCV.cpp` is the plugin entry point
and `HetrickUtilities.cpp` a shared implementation unit. The others want small
named additions to the adaptor's mock: `dsp::approxExp2_taylor5`, `LEDBezel`,
simd `int32_4` and an `abs` overload, `getOutput`,
`NVG_ONE_MINUS_DST_COLOR`, `SvgPanel::setBackground`.

## NOT VERIFIED

**No module here has been run.** 66 translation units compile; they have not
been linked, registered, drawn or heard. Registration, panel rendering and
audio are all unmeasured — see TideSynth **E24**.
