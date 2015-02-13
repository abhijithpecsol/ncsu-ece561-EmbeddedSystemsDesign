// Automatically generated file. Do not edit if you plan to regenerate it.
#include "region.h"
const REGION_T RegionTable[] = {
	{0x000000d5, 0x0000014c, "SystemInit"}, // 0
	{0x0000014d, 0x0000022c, "SystemCoreClockUpdate"}, // 1
	{0x00000261, 0x00000268, "Reset_Handler"}, // 2
	{0x00000269, 0x0000026a, "NMI_Handler"}, // 3
	{0x0000026b, 0x0000026c, "HardFault_Handler"}, // 4
	{0x0000026d, 0x0000026e, "SVC_Handler"}, // 5
	{0x0000026f, 0x00000270, "PendSV_Handler"}, // 6
	{0x00000271, 0x00000272, "SysTick_Handler"}, // 7
	{0x0000027d, 0x000002e2, "main"}, // 8
	{0x000002e9, 0x0000033a, "Init_RGB_LEDs"}, // 9
	{0x0000033b, 0x0000036e, "Control_RGB_LEDs"}, // 10
	{0x00000385, 0x00000398, "Delay"}, // 11
	{0x0000039d, 0x000003c8, "__aeabi_uidivmod"}, // 12
	{0x000003c9, 0x000003e0, "__ARM_scalbnf"}, // 13
	{0x000003e1, 0x000003f6, "__aeabi_i2f"}, // 14
	{0x000003f7, 0x000003f8, "__scatterload_null"}, // 15
	{0x00000411, 0x0000049a, "PIT_IRQHandler"}, // 16
	{0x0000049b, 0x000004e6, "Init_PIT"}, // 17
	{0x000004e7, 0x000004f2, "Start_PIT"}, // 18
	{0x000004f3, 0x000004fe, "Stop_PIT"}, // 19
	{0x000004ff, 0x00000550, "Init_PWM"}, // 20
	{0x00000551, 0x0000056a, "Set_PWM_Values"}, // 21
	{0x000005c1, 0x0000060e, "i2c_init"}, // 22
	{0x0000060f, 0x00000618, "I2C0_IRQHandler"}, // 23
	{0x00000619, 0x0000062c, "i2c_start"}, // 24
	{0x0000062d, 0x00000674, "i2c_read_setup"}, // 25
	{0x00000675, 0x000006a8, "i2c_repeated_read"}, // 26
	{0x000006a9, 0x00000720, "i2c_read_byte"}, // 27
	{0x00000721, 0x0000076c, "i2c_write_byte"}, // 28
	{0x00000785, 0x000007ba, "init_mma"}, // 29
	{0x000007bb, 0x0000081a, "read_full_xyz"}, // 30
	{0x0000081b, 0x00000850, "read_xyz"}, // 31
	{0x00000851, 0x000008c6, "convert_xyz_to_roll_pitc"}, // 32
	{0x000008d1, 0x00000918, "Init_Profiling"}, // 33
	{0x00000919, 0x00000920, "Disable_Profiling"}, // 34
	{0x00000921, 0x00000928, "Enable_Profiling"}, // 35
	{0x00000939, 0x00000954, "sqrt_approx"}, // 36
	{0x00000955, 0x000009d8, "atan2_approx"}, // 37
	{0x000009ed, 0x00000a8e, "__aeabi_fadd"}, // 38
	{0x00000a8f, 0x00000a96, "__aeabi_fsub"}, // 39
	{0x00000a97, 0x00000a9e, "__aeabi_frsub"}, // 40
	{0x00000a9f, 0x00000b18, "__aeabi_fmul"}, // 41
	{0x00000b19, 0x00000b94, "__aeabi_fdiv"}, // 42
	{0x00000b95, 0x00000ba4, "_float_round"}, // 43
	{0x00000ba5, 0x00000c16, "_float_epilogue"}, // 44
	{0x00000c19, 0x00000c34, "__scatterload"}, // 45
	{0x00000c3d, 0x00000c4a, "__scatterload_copy"}, // 46
	{0x00000c4b, 0x00000c58, "__scatterload_zeroinit"}, // 47
}; 
const unsigned NumProfileRegions=48;
volatile unsigned RegionCount[48];
