// Automatically generated file. Do not edit if you plan to regenerate it.
#include "region.h"
const REGION_T RegionTable[] = {
	{0x000000d5, 0x0000014e, "SystemInit"}, // 0
	{0x0000014f, 0x0000022a, "SystemCoreClockUpdate"}, // 1
	{0x0000025d, 0x00000264, "Reset_Handler"}, // 2
	{0x00000265, 0x00000266, "NMI_Handler"}, // 3
	{0x00000267, 0x00000268, "HardFault_Handler"}, // 4
	{0x00000269, 0x0000026a, "SVC_Handler"}, // 5
	{0x0000026b, 0x0000026c, "PendSV_Handler"}, // 6
	{0x0000026d, 0x0000026e, "SysTick_Handler"}, // 7
	{0x00000279, 0x000002de, "main"}, // 8
	{0x000002e5, 0x00000338, "Init_RGB_LEDs"}, // 9
	{0x00000339, 0x0000036a, "Control_RGB_LEDs"}, // 10
	{0x0000037d, 0x0000038e, "Delay"}, // 11
	{0x00000395, 0x000003c4, "Init_Profiling"}, // 12
	{0x000003c5, 0x000003cc, "Disable_Profiling"}, // 13
	{0x000003cd, 0x000003d4, "Enable_Profiling"}, // 14
	{0x000003e5, 0x000003fe, "__ARM_common_switch8"}, // 15
	{0x000003ff, 0x00000400, "__scatterload_null"}, // 16
	{0x00000411, 0x0000049a, "PIT_IRQHandler"}, // 17
	{0x0000049b, 0x000004e6, "Init_PIT"}, // 18
	{0x000004e7, 0x000004f2, "Start_PIT"}, // 19
	{0x000004f3, 0x000004fe, "Stop_PIT"}, // 20
	{0x000004ff, 0x00000550, "Init_PWM"}, // 21
	{0x00000551, 0x0000056a, "Set_PWM_Values"}, // 22
	{0x000005c1, 0x000005f4, "i2c_init"}, // 23
	{0x000005f5, 0x00000608, "i2c_start"}, // 24
	{0x00000609, 0x0000064e, "i2c_read_setup"}, // 25
	{0x0000064f, 0x00000682, "i2c_repeated_read"}, // 26
	{0x00000683, 0x000006f8, "i2c_read_byte"}, // 27
	{0x000006f9, 0x00000742, "i2c_write_byte"}, // 28
	{0x00000751, 0x00000786, "init_mma"}, // 29
	{0x00000787, 0x000007ea, "read_full_xyz"}, // 30
	{0x000007eb, 0x00000820, "read_xyz"}, // 31
	{0x00000821, 0x000008dc, "convert_xyz_to_roll_pitc"}, // 32
	{0x000008ed, 0x00000918, "__aeabi_uidivmod"}, // 33
	{0x00000919, 0x000009ba, "__aeabi_fadd"}, // 34
	{0x000009bb, 0x000009c2, "__aeabi_fsub"}, // 35
	{0x000009c3, 0x000009ca, "__aeabi_frsub"}, // 36
	{0x000009cb, 0x00000a44, "__aeabi_fmul"}, // 37
	{0x00000a45, 0x00000b8c, "__aeabi_dadd"}, // 38
	{0x00000b8d, 0x00000b98, "__aeabi_dsub"}, // 39
	{0x00000b99, 0x00000ba4, "__aeabi_drsub"}, // 40
	{0x00000ba9, 0x00000c72, "__aeabi_dmul"}, // 41
	{0x00000c79, 0x00000d62, "__aeabi_ddiv"}, // 42
	{0x00000d69, 0x00000d94, "__ARM_scalbn"}, // 43
	{0x00000d95, 0x00000db6, "__aeabi_i2d"}, // 44
	{0x00000dbd, 0x00000de4, "__aeabi_f2d"}, // 45
	{0x00000de5, 0x00000e1c, "__aeabi_d2f"}, // 46
	{0x00000e1d, 0x00000e3c, "__aeabi_llsl"}, // 47
	{0x00000e3d, 0x00000e5e, "__aeabi_llsr"}, // 48
	{0x00000e5f, 0x00000e84, "__aeabi_lasr"}, // 49
	{0x00000e85, 0x00000e94, "_float_round"}, // 50
	{0x00000e95, 0x00000f06, "_float_epilogue"}, // 51
	{0x00000f07, 0x00000f20, "_double_round"}, // 52
	{0x00000f21, 0x00000fc4, "_double_epilogue"}, // 53
	{0x00000fc5, 0x00001066, "_dsqrt"}, // 54
	{0x00001069, 0x00001084, "__scatterload"}, // 55
	{0x0000108d, 0x000010ba, "__ARM_clz"}, // 56
	{0x000010bd, 0x000010e4, "__ARM_fpclassify"}, // 57
	{0x000010e9, 0x00001194, "__kernel_poly"}, // 58
	{0x00001195, 0x0000119e, "__mathlib_dbl_infnan"}, // 59
	{0x0000119f, 0x000011a6, "__mathlib_dbl_infnan2"}, // 60
	{0x000011a9, 0x000011b6, "__mathlib_dbl_underflow"}, // 61
	{0x000011bd, 0x000011ca, "__scatterload_copy"}, // 62
	{0x000011cb, 0x000011d8, "__scatterload_zeroinit"}, // 63
	{0x000011d9, 0x000011de, "__set_errno"}, // 64
	{0x000011e5, 0x000013bc, "atan"}, // 65
	{0x00001401, 0x0000154e, "atan2"}, // 66
	{0x00001579, 0x000015ba, "sqrt"}, // 67
}; 
const unsigned NumProfileRegions=68;
volatile unsigned RegionCount[68];