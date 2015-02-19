/*----------------------------------------------------------------------------
 *
 * v2xe.h
 * Michael Meli
 *
 * Header file for v2xe.c
 *
 *----------------------------------------------------------------------------*/

#include <stdint.h>
 
#define kSyncChar   0xAA
#define kTerminator 0x00

enum
{
    // commands/frame types
    kGetModInfo = 1,    // 0x01
    kModInfoResp,       // 0x02 
    kSetDataComponents, // 0x03 
    kGetData,           // 0x04
    kDataResp,          // 0x05
    kSetConfig,         // 0x06
    kGetConfig,         // 0x07
    kConfigResp,        // 0x08
    kSaveConfig,        // 0x09
    kStartCal,          // 0x0A
    kStopCal,           // 0x0B
    kGetCalData,        // 0x0C
    kCalDataResp,       // 0x0D
    kSetCalData,        // 0x0E

    // data types
    kRawX = 1,          // 0x01
    kRawY,              // 0x02
    kCalibratedX,       // 0x03
    kCalibratedY,       // 0x04
    kHeading,           // 0x05
    kMagnitude,         // 0x06
    kTemperature,       // 0x07
    kDistortion,        // 0x08
    kCalStatus,         // 0x09

    // config types
    kDeclination = 1,   // 0x01
    kTrueNorth,         // 0x02
    kCalSampleFreq,     // 0x03
    kSampleFreq,        // 0x04
    kPeriod,            // 0x05
    kBigEndian,         // 0x06
    kDampingSize,       // 0x07

    // cal data types
    kXOffset = 1,       // 0x01
    kYOffset,           // 0x02
    kXGain,             // 0x03
    kYGain,             // 0x04
    kPhi,               // 0x05
    kCalMagnitude       // 0x06
};

typedef struct
{
    int32_t x, y;
    float xe, ye;
    float heading;
} V2XEData;

void get_v2xe_data(V2XEData * data);
void calibrate_v2xe(void);
void calculate_tilt_compensated_heading(void);
