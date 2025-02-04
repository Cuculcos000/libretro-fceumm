#ifndef LIBRETRO_CORE_OPTIONS_H__
#define LIBRETRO_CORE_OPTIONS_H__

#include <stdlib.h>
#include <string.h>

#include <libretro.h>
#include <retro_inline.h>

#define HAVE_NO_LANGEXTRA /* its a maintenance hell to keep updated with default options */

#ifndef HAVE_NO_LANGEXTRA
#include "libretro_core_options_intl.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "paldef.h"

/*
 ********************************
 * Core Option Definitions
 ********************************
*/

enum {
    /* core option count + dipswitch option count */
   MAX_CORE_OPTIONS = 64 /* with overflow count */
};

/* RETRO_LANGUAGE_ENGLISH */

/* Default language:
 * - All other languages must include the same keys and values
 * - Will be used as a fallback in the event that frontend language
 *   is not available
 * - Will be used as a fallback for any missing entries in
 *   frontend language definition */

struct retro_core_option_v2_category option_cats_us[] = {
   {
      "system",
      "System",
      "Change region and hardware add-on settings."
   },
   {
      "video",
      "Video",
#ifdef HAVE_NTSC_FILTER
      "Change aspect ratio, display cropping, color palette and video filter options."
#else
      "Change aspect ratio, display cropping and color palette options."
#endif
   },
   {
      "audio",
      "Audio",
      "Change sound quality, volume, channel enable settings."
   },
   {
      "input",
      "Input",
      "Change turbo and light gun settings."
   },
   {
      "hacks",
      "Emulation Hacks",
      "Change processor overclocking and emulation accuracy settings affecting low-level performance and compatibility."
   },
   {
      "dip_switch",
      "DIP Switches",
      "Change arcade game settings."
   },
   { NULL, NULL, NULL },
};

struct retro_core_option_v2_definition option_defs[] = {
   {
      "fceumm_next_region",
      "Region",
      NULL,
      "Force core to use NTSC, PAL or Dendy region timings.",
      NULL,
      "system",
      {
         { "Auto",  NULL },
         { "NTSC",  NULL },
         { "PAL",   NULL },
         { "Dendy", NULL },
         { NULL, NULL },
      },
      "Auto",
   },
   {
      "fceumm_next_game_genie",
      "Game Genie Add-On (Restart Required)",
      NULL,
      "Enable emulation of a Game Genie add-on cartridge, allowing cheat codes to be entered when launching games. The Game Genie ROM file 'gamegenie.nes' must be present in the frontend's system directory. Does not apply to FDS or arcade content.",
      NULL,
      "system",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_next_show_adv_system_options",
      "Show Advanced System Options",
      NULL,
      "Show advanced system options and tweaks.",
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_next_show_adv_sound_options",
      "Show Advanced Sound Options",
      NULL,
      "Show advanced sound options and tweaks.",
      NULL,
      NULL,
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_next_use_newppu",
      "Use New PPU",
      NULL,
      "Choose preferred PPU modes. NOTE: New PPU disables ppu overclocking.",
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_next_aspect",
      "Aspect Ratio",
      NULL,
      "Choose preferred aspect ratio.",
      NULL,
      "video",
      {
         { "8:7 PAR", NULL },
         { "4:3",     NULL },
         { "PP",      "Pixel Perfect" },
         { NULL, NULL },
      },
      "8:7 PAR",
   },
#ifdef PSP /* overscan options */
   {
      "fceumm_next_overscan",
      "Crop Overscan",
      NULL,
      "Removes 8 pixel overscan from all sides of the screen.",
      NULL,
      "video",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
#else
   {
      "fceumm_next_overscan_left",
      "Crop Overscan (Left)",
      NULL,
      "Removes pixels from left side of the screen to simulate overscan seen on standard CRT televisions.",
      NULL,
      "video",
      {
         { "0",  "0px" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { "20", "20px" },
         { "24", "24px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_next_overscan_right",
      "Crop Overscan (Right)",
      NULL,
      "Removes pixels from right side of the screen to simulate overscan seen on standard CRT televisions.",
      NULL,
      "video",
      {
         { "0",  "0px" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { "20", "20px" },
         { "24", "24px" },
         { NULL, NULL },
      },
      "0",
   },
   {
      "fceumm_next_overscan_top",
      "Crop Overscan (Top)",
      NULL,
      "Removes pixels from the top of the screen to simulate overscan seen on standard CRT televisions.",
      NULL,
      "video",
      {
         { "0",  "0px" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { "20", "20px" },
         { "24", "24px" },
         { NULL, NULL },
      },
      "8",
   },
   {
      "fceumm_next_overscan_bottom",
      "Crop Overscan (Bottom)",
      NULL,
      "Removes pixels from the bottom of the screen to simulate overscan seen on standard CRT televisions.",
      NULL,
      "video",
      {
         { "0",  "0px" },
         { "4",  "4px" },
         { "8",  "8px" },
         { "12", "12px" },
         { "16", "16px" },
         { "20", "20px" },
         { "24", "24px" },
         { NULL, NULL },
      },
      "8",
   },
#endif /* overscan options */
   {
      "fceumm_next_palette",
      "Color Palette",
      NULL,
      "Choose from pre-generated palettes, a custom 64x3 palette from file or raw format (needs to use a nes-decoder shader).",
      NULL,
      "video",
      {
         { "default",             "Default" },
         { PAL_ASQREALC,          "AspiringSquire's Real" },
         { PAL_VIRTUALCONSOLE,    "Nintendo Virtual Console" },
         { PAL_NESRGB,            "Nintendo RGB PPU" },
         { PAL_CXA2025AS,         "Sony CXA2025AS US" },
         { PAL_NESPAL,            "PAL" },
         { PAL_BMF_FINAL2,        "BMF's Final 2" },
         { PAL_BMF_FINAL3,        "BMF's Final 3" },
         { PAL_NESCAP,            "RGBSource's NESCAP" },
         { PAL_WAVEBEAM,          "nakedarthur's Wavebeam" },
         { PAL_FBX_DIGITAL_PRIME, "FBX's Digital Prime" },
         { PAL_FBX_MAGNUM,        "FBX's Magnum" },
         { PAL_FBX_SMOOTH_V2,     "FBX's Smooth V2" },
         { PAL_FBX_NES_CLASSIC,   "FBX's NES Classic" },
         { PAL_ROYAL_TEA,         "Royal Tea (PVM-2530)" },
         { "raw",                 "Raw" },
         { "custom",              "Custom" },
         { NULL, NULL }
      },
      "default",
   },
#ifdef HAVE_NTSC_FILTER
   {
      "fceumm_next_ntsc_filter",
      "NTSC Filter",
      NULL,
      "Blargg's NTSC filters are used to replicate RF, Composite, S-Video, and RGB cable signals.",
      NULL,
      "video",
      {
         { "disabled",   NULL },
         { "composite",  "Composite" },
         { "svideo",     "S-Video" },
         { "rgb",        "RGB" },
         { "monochrome", "Monochrome" },
         { NULL, NULL },
      },
      "disabled"
   },
#endif
   {
      "fceumm_next_ppu_disable_emphasis",
      "Disable Color Emphasis",
      NULL,
      "",
      NULL,
      "video",
      {
         { "enabled",   NULL },
         { "disabled",   NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_next_hide_sprites",
      "Hide Sprites",
      NULL,
      "Show or hide sprites.",
      NULL,
      "video",
      {
         { "enabled",   NULL },
         { "disabled",   NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_next_hide_background",
      "Hide Background.",
      NULL,
      "Show or hide background.",
      NULL,
      "video",
      {
         { "enabled",   NULL },
         { "disabled",   NULL },
         { NULL, NULL },
      },
      "disabled"
   },
   {
      "fceumm_next_sound_rate",
      "Sound Sample Rate",
      NULL,
      "Sets sound sample rate. ",
      NULL,
      "audio",
      {
         { "11025", "11 kHz" },
         { "22050", "22 kHz" },
         { "32000", "32 kHz" },
         { "44100", "44 kHz" },
         { "48000", "48 khz" },
         { "96000", "96 kHz" },
         { "192000", "192 kHz (insane)" },
         { "384000", "384 kHz (insane)" },
         { NULL, NULL }
      },
      "44100",
   },
   {
      "fceumm_next_sndquality",
      "Sound Quality",
      NULL,
      "Enable higher quality sound. Increases performance requirements.",
      NULL,
      "audio",
      {
         { "Low",       NULL },
         { "High",      NULL },
         { "Very High", NULL },
         { NULL, NULL },
      },
      "Low",
   },
   {
      "fceumm_next_sndlowpass",
      "Audio Low-pass Filter",
      NULL,
      "Apply a low pass audio filter to simulate the 'muted' sound of the NES when connected to a television via the RF modulator.",
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "3",  NULL },
         { "2",  NULL },
         { "1",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_next_sndstereodelay",
      "Stereo Sound Effect",
      NULL,
      "Enable a fake stereo sound effect by delaying the right audio channel when upmixing the mono signal output from the NES.",
      NULL,
      "audio",
      {
         { "disabled",    NULL },
         { "01_ms_delay", "1ms Delay" },
         { "02_ms_delay", "2ms Delay" },
         { "03_ms_delay", "3ms Delay" },
         { "04_ms_delay", "4ms Delay" },
         { "05_ms_delay", "5ms Delay" },
         { "06_ms_delay", "6ms Delay" },
         { "07_ms_delay", "7ms Delay" },
         { "08_ms_delay", "8ms Delay" },
         { "09_ms_delay", "9ms Delay" },
         { "10_ms_delay", "10ms Delay" },
         { "11_ms_delay", "11ms Delay" },
         { "12_ms_delay", "12ms Delay" },
         { "13_ms_delay", "13ms Delay" },
         { "14_ms_delay", "14ms Delay" },
         { "15_ms_delay", "15ms Delay (Default)" },
         { "16_ms_delay", "16ms Delay" },
         { "17_ms_delay", "17ms Delay" },
         { "18_ms_delay", "18ms Delay" },
         { "19_ms_delay", "19ms Delay" },
         { "20_ms_delay", "20ms Delay" },
         { "21_ms_delay", "21ms Delay" },
         { "22_ms_delay", "22ms Delay" },
         { "23_ms_delay", "23ms Delay" },
         { "24_ms_delay", "24ms Delay" },
         { "25_ms_delay", "25ms Delay" },
         { "26_ms_delay", "26ms Delay" },
         { "27_ms_delay", "27ms Delay" },
         { "28_ms_delay", "28ms Delay" },
         { "29_ms_delay", "29ms Delay" },
         { "30_ms_delay", "30ms Delay" },
         { "31_ms_delay", "31ms Delay" },
         { "32_ms_delay", "32ms Delay" },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_next_swapduty",
      "Swap Audio Duty Cycles",
      "Swap Duty Cycles",
      "Simulates the sound from famiclones that have the pulse wave channels duty cycle bits reversed.",
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_next_reducedmcpopping",
      "Reduce DMC Popping",
      NULL,
      "Reduce DMC audio popping sound.",
      NULL,
      "audio",
      {
         { "disabled", NULL },
         { "enabled", NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_next_opll_tone",
      "Sets tone in YM2413 emulator.",
      NULL,
      "Sets tone in YM2413 emulator.",
      NULL,
      "audio",
      {
         { "Mapper default", NULL },
         { "2413", NULL },
         { "VRC7", NULL },
         { "281B", NULL },
         { NULL, NULL },
      },
      "Mapper default",
   },
   {
      "fceumm_next_sndvolume",
      "Master Volume",
      NULL,
      "Change master volume level %.",
      NULL,
      "audio",
      {
         {   "0", NULL },
         {   "5", NULL },
         {  "10", NULL },
         {  "15", NULL },
         {  "20", NULL },
         {  "25", NULL },
         {  "30", NULL },
         {  "35", NULL },
         {  "40", NULL },
         {  "45", NULL },
         {  "50", NULL },
         {  "55", NULL },
         {  "60", NULL },
         {  "65", NULL },
         {  "70", NULL },
         {  "75", NULL },
         {  "80", NULL },
         {  "85", NULL },
         {  "90", NULL },
         {  "95", NULL },
         { "100", NULL },
         { "100", NULL },
         { "105", NULL },
         { "110", NULL },
         { "115", NULL },
         { "120", NULL },
         { "125", NULL },
         { "130", NULL },
         { "135", NULL },
         { "140", NULL },
         { "145", NULL },
         { "150", NULL },
         { "155", NULL },
         { "160", NULL },
         { "165", NULL },
         { "170", NULL },
         { "175", NULL },
         { "180", NULL },
         { "185", NULL },
         { "190", NULL },
         { "195", NULL },
         { "200", NULL },
         { NULL, NULL },
      },
      "150",
   },
   {
      "fceumm_next_apu_square_1",
      "Channel Volume (Square 1)",
      NULL,
      "Change pulse wave generator 1 audio output.",
      NULL,
      "audio",
      {
         {   "0", NULL },
         {   "5", NULL },
         {  "10", NULL },
         {  "15", NULL },
         {  "20", NULL },
         {  "25", NULL },
         {  "30", NULL },
         {  "35", NULL },
         {  "40", NULL },
         {  "45", NULL },
         {  "50", NULL },
         {  "55", NULL },
         {  "60", NULL },
         {  "65", NULL },
         {  "70", NULL },
         {  "75", NULL },
         {  "80", NULL },
         {  "85", NULL },
         {  "90", NULL },
         {  "95", NULL },
         { "100", NULL },
         { NULL,  NULL  },
      },
      "100",
   },
   {
      "fceumm_next_apu_square_2",
      "Channel Volume (Square 2)",
      NULL,
      "Change pulse wave generator 2 audio output.",
      NULL,
      "audio",
      {
         {   "0", NULL },
         {   "5", NULL },
         {  "10", NULL },
         {  "15", NULL },
         {  "20", NULL },
         {  "25", NULL },
         {  "30", NULL },
         {  "35", NULL },
         {  "40", NULL },
         {  "45", NULL },
         {  "50", NULL },
         {  "55", NULL },
         {  "60", NULL },
         {  "65", NULL },
         {  "70", NULL },
         {  "75", NULL },
         {  "80", NULL },
         {  "85", NULL },
         {  "90", NULL },
         {  "95", NULL },
         { "100", NULL },
         { NULL,  NULL  },
      },
      "100",
   },
   {
      "fceumm_next_apu_triangle",
      "Channel Volume (Triangle)",
      NULL,
      "Modify triangle wave generator audio output.",
      NULL,
      "audio",
      {
         {   "0", NULL },
         {   "5", NULL },
         {  "10", NULL },
         {  "15", NULL },
         {  "20", NULL },
         {  "25", NULL },
         {  "30", NULL },
         {  "35", NULL },
         {  "40", NULL },
         {  "45", NULL },
         {  "50", NULL },
         {  "55", NULL },
         {  "60", NULL },
         {  "65", NULL },
         {  "70", NULL },
         {  "75", NULL },
         {  "80", NULL },
         {  "85", NULL },
         {  "90", NULL },
         {  "95", NULL },
         { "100", NULL },
         { NULL,  NULL  },
      },
      "100",
   },
   {
      "fceumm_next_apu_noise",
      "Channel Volume (Noise)",
      NULL,
      "Change noise generator audio output.",
      NULL,
      "audio",
      {
         {   "0", NULL },
         {   "5", NULL },
         {  "10", NULL },
         {  "15", NULL },
         {  "20", NULL },
         {  "25", NULL },
         {  "30", NULL },
         {  "35", NULL },
         {  "40", NULL },
         {  "45", NULL },
         {  "50", NULL },
         {  "55", NULL },
         {  "60", NULL },
         {  "65", NULL },
         {  "70", NULL },
         {  "75", NULL },
         {  "80", NULL },
         {  "85", NULL },
         {  "90", NULL },
         {  "95", NULL },
         { "100", NULL },
         { NULL,  NULL  },
      },
      "100",
   },
   {
      "fceumm_next_apu_dpcm",
      "Channel Volume (DPCM)",
      NULL,
      "Change delta modulation channel audio output.",
      NULL,
      "audio",
      {
         {   "0", NULL },
         {   "5", NULL },
         {  "10", NULL },
         {  "15", NULL },
         {  "20", NULL },
         {  "25", NULL },
         {  "30", NULL },
         {  "35", NULL },
         {  "40", NULL },
         {  "45", NULL },
         {  "50", NULL },
         {  "55", NULL },
         {  "60", NULL },
         {  "65", NULL },
         {  "70", NULL },
         {  "75", NULL },
         {  "80", NULL },
         {  "85", NULL },
         {  "90", NULL },
         {  "95", NULL },
         { "100", NULL },
         { NULL,  NULL  },
      },
      "100",
   },
   {
      "fceumm_next_apu_fds",
      "Channel Volume (FDS)",
      NULL,
      "Change FDS expansion audio output.",
      NULL,
      "audio",
      {
         {   "0", NULL },
         {   "5", NULL },
         {  "10", NULL },
         {  "15", NULL },
         {  "20", NULL },
         {  "25", NULL },
         {  "30", NULL },
         {  "35", NULL },
         {  "40", NULL },
         {  "45", NULL },
         {  "50", NULL },
         {  "55", NULL },
         {  "60", NULL },
         {  "65", NULL },
         {  "70", NULL },
         {  "75", NULL },
         {  "80", NULL },
         {  "85", NULL },
         {  "90", NULL },
         {  "95", NULL },
         { "100", NULL },
         { NULL,  NULL  },
      },
      "100",
   },
   {
      "fceumm_next_apu_s5b",
      "Channel Volume (S5B)",
      NULL,
      "Change Sunsoft S5B expansion audio output.",
      NULL,
      "audio",
      {
         {   "0", NULL },
         {   "5", NULL },
         {  "10", NULL },
         {  "15", NULL },
         {  "20", NULL },
         {  "25", NULL },
         {  "30", NULL },
         {  "35", NULL },
         {  "40", NULL },
         {  "45", NULL },
         {  "50", NULL },
         {  "55", NULL },
         {  "60", NULL },
         {  "65", NULL },
         {  "70", NULL },
         {  "75", NULL },
         {  "80", NULL },
         {  "85", NULL },
         {  "90", NULL },
         {  "95", NULL },
         { "100", NULL },
         { NULL,  NULL  },
      },
      "100",
   },
   {
      "fceumm_next_apu_n163",
      "Channel Volume (N163)",
      NULL,
      "Chnage Namco 163 expansion audio output.",
      NULL,
      "audio",
      {
         {   "0", NULL },
         {   "5", NULL },
         {  "10", NULL },
         {  "15", NULL },
         {  "20", NULL },
         {  "25", NULL },
         {  "30", NULL },
         {  "35", NULL },
         {  "40", NULL },
         {  "45", NULL },
         {  "50", NULL },
         {  "55", NULL },
         {  "60", NULL },
         {  "65", NULL },
         {  "70", NULL },
         {  "75", NULL },
         {  "80", NULL },
         {  "85", NULL },
         {  "90", NULL },
         {  "95", NULL },
         { "100", NULL },
         { NULL,  NULL  },
      },
      "100",
   },
   {
      "fceumm_next_apu_vrc6",
      "Channel Volume (VRC6)",
      NULL,
      "Change VRC6 expansion audio output.",
      NULL,
      "audio",
      {
         {   "0", NULL },
         {   "5", NULL },
         {  "10", NULL },
         {  "15", NULL },
         {  "20", NULL },
         {  "25", NULL },
         {  "30", NULL },
         {  "35", NULL },
         {  "40", NULL },
         {  "45", NULL },
         {  "50", NULL },
         {  "55", NULL },
         {  "60", NULL },
         {  "65", NULL },
         {  "70", NULL },
         {  "75", NULL },
         {  "80", NULL },
         {  "85", NULL },
         {  "90", NULL },
         {  "95", NULL },
         { "100", NULL },
         { NULL,  NULL  },
      },
      "100",
   },
   {
      "fceumm_next_apu_vrc7",
      "Channel Volume (VRC7)",
      NULL,
      "Change VRC7 expansion audio output.",
      NULL,
      "audio",
      {
         {   "0", NULL },
         {   "5", NULL },
         {  "10", NULL },
         {  "15", NULL },
         {  "20", NULL },
         {  "25", NULL },
         {  "30", NULL },
         {  "35", NULL },
         {  "40", NULL },
         {  "45", NULL },
         {  "50", NULL },
         {  "55", NULL },
         {  "60", NULL },
         {  "65", NULL },
         {  "70", NULL },
         {  "75", NULL },
         {  "80", NULL },
         {  "85", NULL },
         {  "90", NULL },
         {  "95", NULL },
         { "100", NULL },
         { NULL,  NULL  },
      },
      "100",
   },
   {
      "fceumm_next_apu_mmc5",
      "Channel Volume (MMC5)",
      NULL,
      "Change MMC5 expansion audio output.",
      NULL,
      "audio",
      {
         {   "0", NULL },
         {   "5", NULL },
         {  "10", NULL },
         {  "15", NULL },
         {  "20", NULL },
         {  "25", NULL },
         {  "30", NULL },
         {  "35", NULL },
         {  "40", NULL },
         {  "45", NULL },
         {  "50", NULL },
         {  "55", NULL },
         {  "60", NULL },
         {  "65", NULL },
         {  "70", NULL },
         {  "75", NULL },
         {  "80", NULL },
         {  "85", NULL },
         {  "90", NULL },
         {  "95", NULL },
         { "100", NULL },
         { NULL,  NULL  },
      },
      "100",
   },
   {
      "fceumm_next_turbo_enable",
      "Turbo Enable",
      NULL,
      "Enables or disables turbo buttons.",
      NULL,
      "input",
      {
         { "None",     NULL },
         { "Player 1", NULL },
         { "Player 2", NULL },
         { "Players 1 and 2", NULL },
         { NULL, NULL },
      },
      "None",
   },
   {
      "fceumm_next_turbo_rate",
      "Turbo Rate (in frames)",
      NULL,
      "Repeat rate of turbo buttons in frames.",
      NULL,
      "input",
      {
         /*{ "1",  NULL },*/
         { "2",  NULL },
         { "3",  NULL },
         { "4",  NULL },
         { "5",  NULL },
         { "6",  NULL },
         { "7",  NULL },
         { "8",  NULL },
         { "9",  NULL },
         { "10", NULL },
         { "15", NULL },
         { "30", NULL },
         { "60", NULL },
         { NULL, NULL },
      },
      "3",
   },
   {
      "fceumm_next_zapper_mode",
      "Zapper/Mouse Mode",
      NULL,
      "Selects device to use for Zapper or Mouse/Pointer games.",
      NULL,
      "input",
      {
         { "lightgun",    "Crosshair Light gun" },
         { "stlightgun",  "Seqencial Target Light gun" },
         { "touchscreen", "Touchscreen" },
         { "mouse",       "Mouse" },
         { NULL, NULL },
      },
      "lightgun",
   },
   {
      "fceumm_next_show_crosshair",
      "Show Zapper Crosshair",
      NULL,
      "Shows or hides on-screen crosshairs when using a Zapper.",
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_next_zapper_trigger",
      "Invert Zapper Trigger Signal",
      NULL,
      "Inverts trigger logic when using a Zapper. Disabling it resembles original hardware behavior.",
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_next_zapper_sensor",
      "Invert Zapper Sensor Signal",
      NULL,
      "Inverts sensor logic when using a Zapper (Sequential Targets Light gun mode only). Disabling it resembles original NES/FC hardware behavior.",
      NULL,
      "input",
      {
         { "enabled",  NULL },
         { "disabled", NULL },
         { NULL, NULL },
      },
      "enabled",
   },
   {
      "fceumm_next_zapper_tolerance",
      "Zapper Tolerance",
      NULL,
      "Sets how many pixels from target area is on target.",
      NULL,
      "input",
      {
         { "0",  NULL },
         { "1",  NULL },
         { "2",  NULL },
         { "3",  NULL },
         { "4",  NULL },
         { "5",  NULL },
         { "6",  NULL },
         { "7",  NULL },
         { "8",  NULL },
         { "9",  NULL },
         { "10", NULL },
         { "11", NULL },
         { "12", NULL },
         { "13", NULL },
         { "14", NULL },
         { "15", NULL },
         { "16", NULL },
         { "17", NULL },
         { "18", NULL },
         { "19", NULL },
         { "20", NULL },
         { NULL, NULL },
      },
      "6",
   },
   {
      "fceumm_next_arkanoid_mode",
      "Arkanoid Mode",
      NULL,
      "Selects device to use for Arkanoid games.",
      NULL,
      "input",
      {
         { "abs_mouse",      "Absolute mouse" },
         { "mouse",          "Mouse" },
         { "stelladaptor",   "Stelladaptor" },
         { "touchscreen",    "Touchscreen" },
         { NULL, NULL },
      },
      "mouse",
   },
   {
      "fceumm_next_mouse_sensitivity",
      "Mouse Sensitivity",
      NULL,
      "Mouse sensitivity in percent.",
      NULL,
      "input",
      {
         { "20", NULL },
         { "30", NULL },
         { "40", NULL },
         { "50", NULL },
         { "60", NULL },
         { "70", NULL },
         { "80", NULL },
         { "90", NULL },
         { "100", NULL },
         { "110", NULL },
         { "120", NULL },
         { "130", NULL },
         { "140", NULL },
         { "150", NULL },
         { "160", NULL },
         { "170", NULL },
         { "180", NULL },
         { "190", NULL },
         { "200", NULL },
         { NULL, NULL },
      },
      "100",
   },
   {
      "fceumm_next_up_down_allowed",
      "Allow Opposing Directions",
      NULL,
      "Allows simultaneous UP+DOWN or LEFT+RIGHT button combinations, which can create different effects in some games.",
      NULL,
      "input",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_next_nospritelimit",
      "No Sprite Limit",
      NULL,
      "Removes the 8-per-scanline hardware limit. This reduces sprite flickering but can cause some games to glitch since some use this for effects.",
      NULL,
      "hacks",
      {
         { "disabled", NULL },
         { "enabled",  NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_next_overclocking",
      "Overclock",
      NULL,
      "Enables or disables overclocking, which can reduce slowdowns in some games. Postrender method is more compatible with every game, Vblank is more effective for games like Contra Force.",
      NULL,
      "hacks",
      {
         { "disabled",      NULL },
         { "2x-Postrender", NULL },
         { "2x-VBlank",     NULL },
         { NULL, NULL },
      },
      "disabled",
   },
   {
      "fceumm_next_ramstate",
      "RAM Power-On Fill (Restart Required)",
      NULL,
      "RAM values on power up. Some games rely on initial RAM values for random number generation as an example.",
      NULL,
      "hacks",
      {
         { "fill $ff", "$FF" },
         { "fill $00", "$00" },
         { "random",   "Random" },
         { NULL, NULL },
      },
      "fill $ff",
   },
   { NULL, NULL, NULL, NULL, NULL, NULL, {{0}}, NULL },
};

struct retro_core_option_v2_definition option_defs_us[MAX_CORE_OPTIONS];

struct retro_core_options_v2 options_us = {
   option_cats_us,
   option_defs_us
};

/*
 ********************************
 * Language Mapping
 ********************************
*/

#ifndef HAVE_NO_LANGEXTRA
struct retro_core_options_v2 *options_intl[RETRO_LANGUAGE_LAST] = {
   &options_us,    /* RETRO_LANGUAGE_ENGLISH */
   &options_ja,      /* RETRO_LANGUAGE_JAPANESE */
   &options_fr,      /* RETRO_LANGUAGE_FRENCH */
   &options_es,      /* RETRO_LANGUAGE_SPANISH */
   &options_de,      /* RETRO_LANGUAGE_GERMAN */
   &options_it,      /* RETRO_LANGUAGE_ITALIAN */
   &options_nl,      /* RETRO_LANGUAGE_DUTCH */
   &options_pt_br,   /* RETRO_LANGUAGE_PORTUGUESE_BRAZIL */
   &options_pt_pt,   /* RETRO_LANGUAGE_PORTUGUESE_PORTUGAL */
   &options_ru,      /* RETRO_LANGUAGE_RUSSIAN */
   &options_ko,      /* RETRO_LANGUAGE_KOREAN */
   &options_cht,     /* RETRO_LANGUAGE_CHINESE_TRADITIONAL */
   &options_chs,     /* RETRO_LANGUAGE_CHINESE_SIMPLIFIED */
   &options_eo,      /* RETRO_LANGUAGE_ESPERANTO */
   &options_pl,      /* RETRO_LANGUAGE_POLISH */
   &options_vn,      /* RETRO_LANGUAGE_VIETNAMESE */
   &options_ar,      /* RETRO_LANGUAGE_ARABIC */
   &options_el,      /* RETRO_LANGUAGE_GREEK */
   &options_tr,      /* RETRO_LANGUAGE_TURKISH */
   &options_sv,      /* RETRO_LANGUAGE_SLOVAK */
   &options_fa,      /* RETRO_LANGUAGE_PERSIAN */
   &options_he,      /* RETRO_LANGUAGE_HEBREW */
   &options_ast,     /* RETRO_LANGUAGE_ASTURIAN */
   &options_fi,      /* RETRO_LANGUAGE_FINNISH */
};
#endif

/*
 ********************************
 * Functions
 ********************************
*/

/* Handles configuration/setting of core options.
 * Should only be called inside retro_set_environment().
 * > We place the function body in the header to avoid the
 *   necessity of adding more .c files (i.e. want this to
 *   be as painless as possible for core devs)
 */

static INLINE void libretro_set_core_options(retro_environment_t environ_cb,
      bool *categories_supported)
{
   unsigned version  = 0;
#ifndef HAVE_NO_LANGEXTRA
   unsigned language = 0;
#endif

   if (!environ_cb || !categories_supported)
      return;

   *categories_supported = false;

   if (!environ_cb(RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION, &version))
      version = 0;

   if (version >= 2)
   {
#ifndef HAVE_NO_LANGEXTRA
      struct retro_core_options_v2_intl core_options_intl;

      core_options_intl.us    = &options_us;
      core_options_intl.local = NULL;

      if (environ_cb(RETRO_ENVIRONMENT_GET_LANGUAGE, &language) &&
          (language < RETRO_LANGUAGE_LAST) && (language != RETRO_LANGUAGE_ENGLISH))
         core_options_intl.local = options_intl[language];

      *categories_supported = environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_V2_INTL,
            &core_options_intl);
#else
      *categories_supported = environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_V2,
            &options_us);
#endif
   }
   else
   {
      size_t i, j;
      size_t option_index              = 0;
      size_t num_options               = 0;
      struct retro_core_option_definition
            *option_v1_defs_us         = NULL;
#ifndef HAVE_NO_LANGEXTRA
      size_t num_options_intl          = 0;
      struct retro_core_option_v2_definition
            *option_defs_intl          = NULL;
      struct retro_core_option_definition
            *option_v1_defs_intl       = NULL;
      struct retro_core_options_intl
            core_options_v1_intl;
#endif
      struct retro_variable *variables = NULL;
      char **values_buf                = NULL;

      /* Determine total number of options */
      while (true)
      {
         if (option_defs_us[num_options].key)
            num_options++;
         else
            break;
      }

      if (version >= 1)
      {
         /* Allocate US array */
         option_v1_defs_us = (struct retro_core_option_definition *)
               calloc(num_options + 1, sizeof(struct retro_core_option_definition));

         /* Copy parameters from option_defs_us array */
         for (i = 0; i < num_options; i++)
         {
            struct retro_core_option_v2_definition *option_def_us = &option_defs_us[i];
            struct retro_core_option_value *option_values         = option_def_us->values;
            struct retro_core_option_definition *option_v1_def_us = &option_v1_defs_us[i];
            struct retro_core_option_value *option_v1_values      = option_v1_def_us->values;

            option_v1_def_us->key           = option_def_us->key;
            option_v1_def_us->desc          = option_def_us->desc;
            option_v1_def_us->info          = option_def_us->info;
            option_v1_def_us->default_value = option_def_us->default_value;

            /* Values must be copied individually... */
            while (option_values->value)
            {
               option_v1_values->value = option_values->value;
               option_v1_values->label = option_values->label;

               option_values++;
               option_v1_values++;
            }
         }

#ifndef HAVE_NO_LANGEXTRA
         if (environ_cb(RETRO_ENVIRONMENT_GET_LANGUAGE, &language) &&
             (language < RETRO_LANGUAGE_LAST) && (language != RETRO_LANGUAGE_ENGLISH) &&
             options_intl[language])
            option_defs_intl = options_intl[language]->definitions;

         if (option_defs_intl)
         {
            /* Determine number of intl options */
            while (true)
            {
               if (option_defs_intl[num_options_intl].key)
                  num_options_intl++;
               else
                  break;
            }

            /* Allocate intl array */
            option_v1_defs_intl = (struct retro_core_option_definition *)
                  calloc(num_options_intl + 1, sizeof(struct retro_core_option_definition));

            /* Copy parameters from option_defs_intl array */
            for (i = 0; i < num_options_intl; i++)
            {
               struct retro_core_option_v2_definition *option_def_intl = &option_defs_intl[i];
               struct retro_core_option_value *option_values           = option_def_intl->values;
               struct retro_core_option_definition *option_v1_def_intl = &option_v1_defs_intl[i];
               struct retro_core_option_value *option_v1_values        = option_v1_def_intl->values;

               option_v1_def_intl->key           = option_def_intl->key;
               option_v1_def_intl->desc          = option_def_intl->desc;
               option_v1_def_intl->info          = option_def_intl->info;
               option_v1_def_intl->default_value = option_def_intl->default_value;

               /* Values must be copied individually... */
               while (option_values->value)
               {
                  option_v1_values->value = option_values->value;
                  option_v1_values->label = option_values->label;

                  option_values++;
                  option_v1_values++;
               }
            }
         }

         core_options_v1_intl.us    = option_v1_defs_us;
         core_options_v1_intl.local = option_v1_defs_intl;

         environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS_INTL, &core_options_v1_intl);
#else
         environ_cb(RETRO_ENVIRONMENT_SET_CORE_OPTIONS, option_v1_defs_us);
#endif
      }
      else
      {
         /* Allocate arrays */
         variables  = (struct retro_variable *)calloc(num_options + 1,
               sizeof(struct retro_variable));
         values_buf = (char **)calloc(num_options, sizeof(char *));

         if (!variables || !values_buf)
            goto error;

         /* Copy parameters from option_defs_us array */
         for (i = 0; i < num_options; i++)
         {
            const char *key                        = option_defs_us[i].key;
            const char *desc                       = option_defs_us[i].desc;
            const char *default_value              = option_defs_us[i].default_value;
            struct retro_core_option_value *values = option_defs_us[i].values;
            size_t buf_len                         = 3;
            size_t default_index                   = 0;

            values_buf[i] = NULL;

            /* Skip options that are irrelevant when using the
             * old style core options interface */
            if ((strcmp(key, "fceumm_next_show_adv_system_options") == 0) ||
                (strcmp(key, "fceumm_next_advance_sound_options") == 0))
               continue;

            if (desc)
            {
               size_t num_values = 0;

               /* Determine number of values */
               while (true)
               {
                  if (values[num_values].value)
                  {
                     /* Check if this is the default value */
                     if (default_value)
                        if (strcmp(values[num_values].value, default_value) == 0)
                           default_index = num_values;

                     buf_len += strlen(values[num_values].value);
                     num_values++;
                  }
                  else
                     break;
               }

               /* Build values string */
               if (num_values > 0)
               {
                  buf_len += num_values - 1;
                  buf_len += strlen(desc);

                  values_buf[i] = (char *)calloc(buf_len, sizeof(char));
                  if (!values_buf[i])
                     goto error;

                  strcpy(values_buf[i], desc);
                  strcat(values_buf[i], "; ");

                  /* Default value goes first */
                  strcat(values_buf[i], values[default_index].value);

                  /* Add remaining values */
                  for (j = 0; j < num_values; j++)
                  {
                     if (j != default_index)
                     {
                        strcat(values_buf[i], "|");
                        strcat(values_buf[i], values[j].value);
                     }
                  }
               }
            }

            variables[option_index].key   = key;
            variables[option_index].value = values_buf[i];
            option_index++;
         }

         /* Set variables */
         environ_cb(RETRO_ENVIRONMENT_SET_VARIABLES, variables);
      }

error:
      /* Clean up */

      if (option_v1_defs_us)
      {
         free(option_v1_defs_us);
         option_v1_defs_us = NULL;
      }

#ifndef HAVE_NO_LANGEXTRA
      if (option_v1_defs_intl)
      {
         free(option_v1_defs_intl);
         option_v1_defs_intl = NULL;
      }
#endif

      if (values_buf)
      {
         for (i = 0; i < num_options; i++)
         {
            if (values_buf[i])
            {
               free(values_buf[i]);
               values_buf[i] = NULL;
            }
         }

         free(values_buf);
         values_buf = NULL;
      }

      if (variables)
      {
         free(variables);
         variables = NULL;
      }
   }
}

#ifdef __cplusplus
}
#endif

#endif
