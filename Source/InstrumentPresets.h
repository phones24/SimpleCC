#pragma once

#include <JuceHeader.h>
#include <vector>

struct CCMapping
{
    int ccNumber;
    juce::String paramName;
};

struct InstrumentPreset
{
    juce::String name;
    juce::String manufacturer;
    std::vector<CCMapping> mappings;
};

inline std::vector<InstrumentPreset> getInstrumentPresets()
{
    return {
        {
            "Access Virus TI",
            "Access",
            {
                {40, "Filter 1 Cutoff"},
                {42, "Filter 1 Reso"},
                {41, "Filter 2 Cutoff"},
                {43, "Filter 2 Reso"},
                {54, "Filter Env Atk"},
                {55, "Filter Env Dec"},
                {56, "Filter Env Sus"},
                {58, "Filter Env Rel"},
                {59, "Amp Env Attack"},
                {60, "Amp Env Decay"},
                {67, "LFO 1 Rate"},
                {79, "LFO 2 Rate"}
            }
        },
        {
            "Arturia MatrixBrute",
            "Arturia",
            {
                {27, "Master Cutoff"},
                {23, "Steiner Cutoff"},
                {83, "Steiner Reso"},
                {24, "Steiner Env Amt"},
                {25, "Ladder Cutoff"},
                {87, "Ladder Reso"},
                {26, "Ladder Env Amt"},
                {102, "VCF Env Attack"},
                {103, "VCF Env Decay"},
                {28, "VCF Env Sustain"},
                {104, "VCF Env Release"},
                {105, "VCA Env Attack"},
                {106, "VCA Env Decay"},
                {29, "VCA Env Sustain"},
                {107, "VCA Env Release"},
                {91, "LFO 1 Rate"}
            }
        },
        {
            "Arturia MicroFreak",
            "Arturia",
            {
                {23, "Filter Cutoff"},
                {83, "Filter Reso"},
                {26, "Filter Env Amt"},
                {105, "Env Attack"},
                {106, "Env Decay"},
                {29, "Env Sustain"},
                {102, "Cyc Env Rise"},
                {103, "Cyc Env Fall"},
                {93, "LFO Rate"}
            }
        },
        {
            "Arturia MiniFreak",
            "Arturia",
            {
                {74, "Filter Cutoff"},
                {71, "Filter Reso"},
                {24, "VCF Env Amount"},
                {80, "Env Attack"},
                {81, "Env Decay"},
                {82, "Env Sustain"},
                {83, "Env Release"},
                {85, "LFO 1 Rate"},
                {87, "LFO 2 Rate"}
            }
        },
        {
            "Arturia PolyBrute",
            "Arturia",
            {
                {27, "Master Cutoff"},
                {25, "Ladder Cutoff"},
                {87, "Ladder Reso"},
                {26, "Ladder Env Amt"},
                {23, "Steiner Cutoff"},
                {83, "Steiner Reso"},
                {24, "Steiner Env Amt"},
                {102, "VCF Env Attack"},
                {103, "VCF Env Decay"},
                {28, "VCF Env Sustain"},
                {104, "VCF Env Release"},
                {105, "VCA Env Attack"},
                {106, "VCA Env Decay"},
                {29, "VCA Env Sustain"},
                {107, "VCA Env Release"},
                {91, "LFO 1 Rate"}
            }
        },
        {
            "ASM Hydrasynth",
            "ASM",
            {
                {74, "Filter 1 Cutoff"},
                {71, "Filter 1 Reso"},
                {3, "Filter 2 Cutoff"},
                {9, "Filter 2 Reso"},
                {73, "Amp Attack"},
                {75, "Amp Decay"},
                {76, "Amp Sustain"},
                {72, "Amp Release"},
                {85, "LFO 1 Rate"},
                {86, "LFO 2 Rate"}
            }
        },
        {
            "Dreadbox Typhon",
            "Dreadbox",
            {
                {74, "Filter Cutoff"},
                {71, "Filter Reso"},
                {73, "Amp Attack"},
                {75, "Amp Decay"},
                {76, "Amp Sustain"},
                {72, "Amp Release"},
                {22, "Filter Attack"},
                {23, "Filter Decay"},
                {24, "Filter Sustain"},
                {25, "Filter Release"},
                {26, "Mod 1 Amount"},
                {27, "Mod 1 Rate"}
            }
        },
        {
            "Elektron Digitakt",
            "Elektron",
            {
                {74, "Filter Freq"},
                {75, "Filter Reso"},
                {77, "Filter Env Depth"},
                {70, "Filter Attack"},
                {71, "Filter Decay"},
                {72, "Filter Sustain"},
                {73, "Filter Release"},
                {78, "Amp Attack"},
                {80, "Amp Decay"},
                {102, "LFO Speed"}
            }
        },
        {
            "Elektron Digitone",
            "Elektron",
            {
                {23, "Filter Freq"},
                {24, "Filter Reso"},
                {25, "Filter Env Depth"},
                {70, "Filter Attack"},
                {71, "Filter Decay"},
                {72, "Filter Sustain"},
                {73, "Filter Release"},
                {104, "Amp Attack"},
                {105, "Amp Decay"},
                {106, "Amp Sustain"},
                {107, "Amp Release"},
                {28, "LFO 1 Speed"},
                {29, "LFO 1 Depth"}
            }
        },
        {
            "Korg Minilogue",
            "Korg",
            {
                {43, "Filter Cutoff"},
                {44, "Filter Reso"},
                {45, "Filter Env Int"},
                {16, "Amp Env Attack"},
                {17, "Amp Env Decay"},
                {18, "Amp Env Sustain"},
                {19, "Amp Env Release"},
                {24, "LFO Rate"},
                {26, "LFO Depth"}
            }
        },
        {
            "Korg Minilogue XD",
            "Korg",
            {
                {43, "Filter Cutoff"},
                {44, "Filter Reso"},
                {16, "Amp Env Attack"},
                {17, "Amp Env Decay"},
                {18, "Amp Env Sustain"},
                {19, "Amp Env Release"},
                {20, "EG Attack"},
                {21, "EG Decay"},
                {24, "LFO Rate"},
                {26, "LFO Intensity"}
            }
        },
        {
            "Modal Argon8",
            "Modal",
            {
                {74, "Filter Cutoff"},
                {71, "Filter Reso"},
                {73, "Amp Attack"},
                {75, "Amp Decay"},
                {76, "Amp Sustain"},
                {72, "Amp Release"},
                {22, "Filter Attack"},
                {23, "Filter Decay"},
                {26, "LFO 1 Rate"},
                {27, "LFO 1 Depth"}
            }
        },
        {
            "Moog Grandmother",
            "Moog",
            {
                {74, "Filter Cutoff"},
                {71, "Filter Reso"},
                {73, "Attack"},
                {75, "Decay"},
                {76, "Sustain"},
                {72, "Release"},
                {16, "LFO Rate"}
            }
        },
        {
            "Moog Matriarch",
            "Moog",
            {
                {74, "Filter Cutoff"},
                {71, "Filter Reso"},
                {73, "Attack"},
                {75, "Decay"},
                {76, "Sustain"},
                {72, "Release"},
                {16, "LFO 1 Rate"},
                {17, "LFO 2 Rate"}
            }
        },
        {
            "Moog Subsequent 37",
            "Moog",
            {
                {19, "Filter Cutoff"},
                {21, "Filter Reso"},
                {27, "Filter Env Amt"},
                {23, "Filter Env Atk"},
                {24, "Filter Env Dec"},
                {25, "Filter Env Sus"},
                {26, "Filter Env Rel"},
                {28, "Amp Env Attack"},
                {29, "Amp Env Decay"},
                {30, "Amp Env Sustain"},
                {31, "Amp Env Release"},
                {3, "LFO 1 Rate"},
                {8, "LFO 2 Rate"}
            }
        },
        {
            "Nord Lead 4",
            "Nord",
            {
                {74, "Filter Cutoff"},
                {71, "Filter Reso"},
                {79, "Filter Env Amt"},
                {73, "Amp Attack"},
                {75, "Amp Decay"},
                {76, "Amp Sustain"},
                {72, "Amp Release"},
                {77, "Filter Attack"},
                {78, "Filter Decay"},
                {8, "LFO Amount"}
            }
        },
        {
            "Novation Bass Station II",
            "Novation",
            {
                {16, "Filter Freq"},
                {82, "Filter Reso"},
                {90, "Amp Env Attack"},
                {91, "Amp Env Decay"},
                {92, "Amp Env Sustain"},
                {93, "Amp Env Release"},
                {102, "Mod Env Attack"},
                {103, "Mod Env Decay"},
                {18, "LFO 1 Speed"},
                {19, "LFO 2 Speed"}
            }
        },
        {
            "Oberheim OB-X8",
            "Oberheim",
            {
                {74, "Filter Cutoff"},
                {71, "Filter Reso"},
                {22, "Filter Env Amt"},
                {73, "Filter Attack"},
                {75, "Filter Decay"},
                {76, "Filter Sustain"},
                {72, "Filter Release"},
                {51, "Amp Attack"},
                {52, "Amp Decay"},
                {53, "Amp Sustain"},
                {54, "Amp Release"}
            }
        },
        {
            "Roland JU-06A",
            "Roland",
            {
                {74, "VCF Cutoff"},
                {71, "VCF Reso"},
                {22, "VCF Env Depth"},
                {23, "VCF LFO Depth"},
                {73, "Env Attack"},
                {75, "Env Decay"},
                {27, "Env Sustain"},
                {72, "Env Release"},
                {3, "LFO Rate"},
                {9, "LFO Delay"}
            }
        },
        {
            "Roland JX-08",
            "Roland",
            {
                {74, "Filter Cutoff"},
                {71, "Filter Reso"},
                {73, "Amp Attack"},
                {75, "Amp Decay"},
                {76, "Amp Sustain"},
                {72, "Amp Release"},
                {22, "VCF Attack"},
                {23, "VCF Decay"},
                {12, "LFO Rate"}
            }
        },
        {
            "Roland S-1",
            "Roland",
            {
                {74, "Filter Cutoff"},
                {71, "Filter Reso"},
                {24, "Filter Env Depth"},
                {73, "Env Attack"},
                {75, "Env Decay"},
                {30, "Env Sustain"},
                {72, "Env Release"},
                {3, "LFO Rate"},
                {25, "Filter LFO Depth"}
            }
        },
        {
            "Sequential OB-6",
            "Sequential",
            {
                {22, "Filter Cutoff"},
                {23, "Filter Reso"},
                {24, "Filter Env Amt"},
                {51, "Filter Attack"},
                {52, "Filter Decay"},
                {53, "Filter Sustain"},
                {54, "Filter Release"},
                {55, "Amp Attack"},
                {56, "Amp Decay"},
                {26, "LFO Rate"},
                {27, "LFO Amount"}
            }
        },
        {
            "Sequential Prophet-6",
            "Sequential",
            {
                {102, "LP Filter Cutoff"},
                {103, "LP Filter Reso"},
                {50, "Filter Env Atk"},
                {51, "Filter Env Dec"},
                {52, "Filter Env Sus"},
                {53, "Filter Env Rel"},
                {43, "VCA Env Attack"},
                {44, "VCA Env Decay"},
                {45, "VCA Env Sustain"},
                {46, "VCA Env Release"}
            }
        },
        {
            "Waldorf Blofeld",
            "Waldorf",
            {
                {69, "Filter 1 Cutoff"},
                {70, "Filter 1 Reso"},
                {80, "Filter 2 Cutoff"},
                {81, "Filter 2 Reso"},
                {95, "Filter Env Atk"},
                {96, "Filter Env Dec"},
                {97, "Filter Env Sus"},
                {100, "Filter Env Rel"},
                {101, "Amp Env Attack"},
                {16, "LFO 1 Speed"},
                {20, "LFO 2 Speed"}
            }
        },
        {
            "Yamaha Reface CS",
            "Yamaha",
            {
                {74, "Filter Cutoff"},
                {71, "Filter Reso"},
                {73, "Attack"},
                {75, "Decay"},
                {76, "Sustain"},
                {72, "Release"},
                {19, "LFO Depth"},
                {20, "LFO Speed"}
            }
        }
    };
}
