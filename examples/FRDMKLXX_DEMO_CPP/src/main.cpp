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
*   TSS C++ example for Freedom boards.
*
*END************************************************************************/
#include "main.h"
#include "board.h"

using namespace tss;

/* Global variables */
TSSASlider *slider;
TSSSystem *tss_system;

/**
 * \brief TSS onInit function invoked during TSSSystem initialization.
 *
 * \param void
 *
 * \return void
 */
void TSS_fOnInit(void)
{
  InitDevices();
}

/**
 * \brief On board LED initialization.
 *
 * \param void
 *
 * \return void
 */
void led_init(void)
{
  LED0_MUX(1);
  LED0_OFF;
}

/**
 * \brief Movement callback.
 *
 * \param void
 *
 * \return void
 */
void myCallbackMovement(TSSASlider *p_this, TSSASlider::ASliderEvent state, UINT8 elec_num)
{
  /* LED ON only if the position > 20 */
  if (p_this->get(TSS_POSITION_RO) > 20)
  {
    LED0_ON;
  }
  else
  {
    LED0_OFF;
  }
}

/**
 * \brief All electrodes release callback.
 *
 * \param void
 *
 * \return void
 */
void myCallbackAllRelease(TSSASlider *p_this, TSSASlider::ASliderEvent state, UINT8 elec_num)
{
  LED0_OFF;
}

/**
 * \brief Fault callback.
 *
 * \param void
 *
 * \return void
 */
void myFaultCallback(TSSSystem *p_this, TSSSystemEvent state, UINT8 elec_num)
{
  /* an example */
  TSS_SetSystemConfig(System_ElectrodeEnablers_Register, 0x03);
  TSS_SetSystemConfig(System_Faults_Register, 0x00);
}

/**
 * \brief Main function.
 *
 * \param void
 *
 * \return int
 */
int main(void)
{
  slider = static_cast<TSSASlider*>(TSSControlFactory::createTSSControl(0));  /* factory creates control number 0 */
  tss_system = TSSSystem::getTSSSystem(); /* get system pointer for callback registration */

  led_init();
  tss_system->regCallback(&myFaultCallback);
  slider->enable(TSS_ALL_RELEASE);
  slider->regCallback(&myCallbackMovement, TSSASlider::ASLIDER_MOVEMENT);
  slider->regCallback(&myCallbackAllRelease, TSSASlider::ASLIDER_ALL_RELEASE);

  for(;;)
  {
    if (TSSSystem::TSSTask() == TSS_STATUS_OK)
    {
    }
  }

}
