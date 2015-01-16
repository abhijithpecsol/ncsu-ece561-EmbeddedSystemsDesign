/*HEADER********************************************************************
*
* Copyright (c) 2013 Freescale Semiconductor;
* All Rights Reserved
*
***************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* Comments:
*
*   Modules for TWRPI code file.
*
*END************************************************************************/
#include "module_id.h"

//  All tower board modules
TWRPI_DEVICES module_id_twrpi_modules[MODULE_SLOT_CNT] = {TWRPI_NUMBER_MODULES};
//  Modules ID's for detection on AD converter
const TWRPI_MODULES_IDS twrpi_modules_ids[TWRPI_NUMBER_MODULES] = {{0xff,0xff},{8,8},{5,8},{5,7},{5,4},{5,6},{5,5},{5,3},{5,2},{5,1},{5,0}};
const TWRPI_MODULES_IDS twrpi_modules_adc_channels[MODULE_SLOT_CNT] = { {TWR_ADC_CHANN_A, TWR_ADC_CHANN_B }};
//  Ranges for conversion from AD converter
const Word twrpi_modules_adc_values[8] = {
 TWRPI_ADC_TRESHOLD_1,
 TWRPI_ADC_TRESHOLD_2,
 TWRPI_ADC_TRESHOLD_3,
 TWRPI_ADC_TRESHOLD_4,
 TWRPI_ADC_TRESHOLD_5,
 TWRPI_ADC_TRESHOLD_6,
 TWRPI_ADC_TRESHOLD_7,
 TWRPI_ADC_TRESHOLD_8
};

static TWRPI_MODULES_EVENTS twrpi_events[TWRPI_BUFFER_EV_LEN];

static Byte twrpi_ev_r;
static Byte twrpi_ev_w;

/**
 * \brief Function receives actual event
 *
 * \param pEvent Pointer to structures of devices
 *
 * \return true  if success
 * \return false otherwise
 */
Byte MODULE_GetEvent(TWRPI_MODULES_EVENTS* pEvent)
{
  if(twrpi_ev_r == twrpi_ev_w)
    return FALSE;
  *pEvent = twrpi_events[twrpi_ev_r++];

  if(twrpi_ev_r >= TWRPI_BUFFER_EV_LEN)
    twrpi_ev_r = 0;

    return TRUE;
}

/**
 * \brief  Function selects correct values of ID's
 *
 * \param value_Adc Exponent
 *
 * \return i index of tower module
 * \return 0 unsuccesfull selecting
 */
static byte MODULE_ID_Get_device_ID(word value_Adc)
{
  byte i;

  for(i=0; i < 8 ; i++)
  {
    if(value_Adc <= twrpi_modules_adc_values[i])
     break;
  }
  return i;
}

/**
 * \brief Function read value from specific channel on ADC
 *
 * \param AdcChannel  ADC channel
 *
 * \param result      Pointer to result value (value from ADC data register)
 *
 * \return true  if success
 * \return false otherwise
 */
static byte MODULE_ID_Read_ADC(unsigned char AdcChannel, word* result)
{
      short cnt = 0;
      byte i;
      word res = 0;

      /* ADC module decoding */
      if ((AdcChannel & TWR_ADC_MASK) == TWR_ADC0)
      {
        /* Modules Clock enabling */
        SCG_C4_ADC = 1u; /* ADC0 clock enabling */
      #if USE_INTERNAL_PULL_UP
        PORT_PTAPE_PTAPE1 = 1;
        PORT_PTFPE_PTFPE7 = 1;
      #endif
      } else
      {
        /* Unknown ADC */
        return FALSE;
      }

      // Check if ADC measurement is active
      if(ADC_SC2 & ADC_SC2_ADACT_MASK)
      {
        return FALSE;
      }

      ADC_SC3 = 0x08; // set the ADC to 12 bit mode and short time measuring

      for(i=0; i<8; i++) {
        ADC_SC1_ADCH = AdcChannel;
        // Wait for ADC conversion to complete
        cnt = 0;

        while ((!(ADC_SC1 & ADC_SC1_COCO_MASK)) && (++cnt)){};

        ADC_SC1_ADCH = 0x1F; //Turn off ADC

        if(!cnt)
          return FALSE;

        res += ADC_R;
      }
      // if the measurement ends by timeout return 0 in other case result of ADC conversion
      *result = res >> 3;

      return TRUE;
}

/**
 * \brief Function identifies connected devices on TWR module
 *
 * \param module  Current TWR module
 *
 * \return true  if success
 * \return false otherwise
 */
Byte MODULE_ID_Identify_Device(MODULES module)
{
      TWRPI_DEVICES tmp_device;
      Word resultAdc;
      Byte j;
      TWRPI_MODULES_ADC_CHANNELS id_detected;

      if(MODULE_ID_Read_ADC(twrpi_modules_adc_channels[module].id0, &resultAdc) == TRUE)
        id_detected.id0 = MODULE_ID_Get_device_ID(resultAdc);
      else
        return FALSE;

      if(MODULE_ID_Read_ADC(twrpi_modules_adc_channels[module].id1, &resultAdc) == TRUE)
        id_detected.id1 = MODULE_ID_Get_device_ID(resultAdc);
      else
        return FALSE;

      tmp_device = TWRPI_UNKNOWN;
      for(j=1; j < TWRPI_NUMBER_MODULES; j++) {
        if(id_detected.id0 ==  twrpi_modules_ids[j].id0 && id_detected.id1 == twrpi_modules_ids[j].id1 ) {
          tmp_device = (TWRPI_DEVICES) j;
          break;
        }
      }

      if(module_id_twrpi_modules[module] != tmp_device)
      {
        TWRPI_MODULES_EVENTS tmp_ev;
        tmp_ev.module = module;
        tmp_ev.prev_dev = module_id_twrpi_modules[module];
        tmp_ev.new_dev = tmp_device;

        twrpi_events[twrpi_ev_w++] = tmp_ev;
        if(twrpi_ev_w >= TWRPI_BUFFER_EV_LEN)
          twrpi_ev_w = 0;

        module_id_twrpi_modules[module] = tmp_device;

        #ifdef CALLBACK
          CALLBACK(module_id_twrpi_modules, module);
        #endif
      }
   return TRUE;
 }

/**
 * \brief This function checks all devices on all modules
 *
 * \param void
 *
 * \return true  if success
 * \return false otherwise
 */
Byte MODULE_ID_Check_Modules(void)
{
   byte i;

   for(i = 0; i < MODULE_SLOT_CNT; i++)
   {
      if(MODULE_ID_Identify_Device( (MODULES) i) == FALSE)
        return FALSE;
   }
   return TRUE;
}

/**
 * \brief This function checks all devices on current module
 *
 * \param module  Current TWR module
 *
 * \param dev     Pointer to the current device
 *
 * \return true  if success
 * \return false otherwise
 */
Byte  MODULE_ID_Check_Module(MODULES module, TWRPI_DEVICES* dev)
{
  Byte ret = 0;

  ret = MODULE_ID_Identify_Device(module);
  *dev = module_id_twrpi_modules[module];

  return ret;
}

/**
 * \brief This function get device on current module
 *
 * \param module  Current TWR module
 *
 * \param dev     Pointer to the current device
 *
 * \return device
 */
TWRPI_DEVICES  MODULE_ID_Get_Device(MODULES module)
{
  return module_id_twrpi_modules[module];
}

/**
 * \brief This function init all modules
 *
 * \param void
 *
 * \return void
 */
void MODULE_ID_Init(void)
{
  twrpi_ev_r = 0;
  twrpi_ev_w = 0;

  module_id_twrpi_modules[MODULE_A] = TWRPI_NUMBER_MODULES;
}
