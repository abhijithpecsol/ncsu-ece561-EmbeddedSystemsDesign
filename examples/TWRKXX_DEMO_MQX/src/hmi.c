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
*   This file contains the source for the HMI KXX example program.
*
*END************************************************************************/
#include <mqx.h>
#include <bsp.h>
#include <shell.h>

#define MAIN_TASK  5
#define SHELL_TASK 6

#if !BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif

/* function prototypes */
void Main_task(uint_32);
void Shell_task(uint_32);
void callback_print_push(pointer);
void callback_toggle_led(pointer);
void callback_rotary_movement(pointer);
void callback_print_release(pointer);
void callback_led_on(pointer);
void callback_led_off(pointer);
int_32 Shell_set_module(int_32, char_ptr []);
int_32 Shell_get_module(int_32, char_ptr []);


TASK_TEMPLATE_STRUCT MQX_template_list[] =
{
/*  Task number, Entry point, Stack, Pri, String, Auto? */
  {MAIN_TASK,   Main_task,   1500,  9,   "main",  MQX_AUTO_START_TASK},
  {SHELL_TASK,  Shell_task,  1500, 12,   "shell", MQX_AUTO_START_TASK},
  {0,           0,           0,     0,   0,      0,                 }
};

const SHELL_COMMAND_STRUCT Shell_commands[] = {
  {"module", Shell_set_module},
  {"get", Shell_get_module},
  {"exit", Shell_exit},
  {"help", Shell_help},
  {NULL, NULL}
};

HMI_CLIENT_STRUCT_PTR bsp_btnled_driver_handle;    /* BTNLED handle pointer */
LWSEM_STRUCT lwsem_tss; /* a semaphore to synchronize TSS Task a TSS Init in bsp_set_twrpi */

/**
 * \brief  This task initializes HMI.
 *
 * It sets LEDs ON and adds callbacks for 12 buttons and
 * a rotary control. Client's poll function is polled.
 *
 * \param  void
 *
 * \return void
 */
void Main_task(uint_32 initial_data)
{
  printf("HMI demo application. \nPress buttons and electrodes available on the board.\n");
  printf("Set TWRPI module with module command.\n");

  bsp_btnled_driver_handle = _bsp_btnled_init(); /* BTNLED initialization */
  if (bsp_btnled_driver_handle == NULL)
  {
    printf("BTNLED initialization failed. \n");
    _task_block();
  }
  /* set LED ON */
  btnled_set_value(bsp_btnled_driver_handle, HMI_LED_1, HMI_VALUE_ON);
  btnled_set_value(bsp_btnled_driver_handle, HMI_LED_2, HMI_VALUE_ON);
  btnled_set_value(bsp_btnled_driver_handle, HMI_LED_3, HMI_VALUE_ON);
  btnled_set_value(bsp_btnled_driver_handle, HMI_LED_4, HMI_VALUE_ON);

  /* add callbacks */
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_1, HMI_VALUE_PUSH, callback_led_off, "1");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_1, HMI_VALUE_RELEASE, callback_led_on, "1");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_2, HMI_VALUE_PUSH, callback_led_off, "2");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_2, HMI_VALUE_RELEASE, callback_led_on, "2");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_3, HMI_VALUE_PUSH, callback_led_off, "3");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_3, HMI_VALUE_RELEASE, callback_led_on, "3");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_4, HMI_VALUE_PUSH, callback_led_off, "4");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_4, HMI_VALUE_RELEASE, callback_led_on, "4");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_5, HMI_VALUE_PUSH, callback_print_push, "button 5");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_6, HMI_VALUE_PUSH, callback_print_push, "button 6");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_7, HMI_VALUE_PUSH, callback_print_push, "button 7");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_8, HMI_VALUE_PUSH, callback_print_push, "button 8");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_9, HMI_VALUE_RELEASE, callback_print_release, "button 9");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_10, HMI_VALUE_PUSH, callback_print_push, "button 10");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_11, HMI_VALUE_PUSH, callback_print_push, "button 11");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_12, HMI_VALUE_PUSH, callback_print_push, "button 12");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_ROTARY_1, HMI_VALUE_MOVEMENT, callback_rotary_movement, "rotary");
  _lwsem_create(&lwsem_tss, 1);
  while (1)
  {
    _lwsem_wait(&lwsem_tss);
    btnled_poll(bsp_btnled_driver_handle);  /* polls TSS and LWGPIO buttons */
    _lwsem_post(&lwsem_tss);
    _time_delay(5); /* time for shell */
  }
}

/**
 * \brief  This task starts shell and polls it regulary.
 *
 * \param  void
 *
 * \return void
 */
void Shell_task(uint_32 initial_data)
{
  while (1)
  {
    Shell(Shell_commands,NULL);    /* start shell */
  }
}

/**
 * \brief  Callback for a pushed button. Function prints the number of the pushed button.
 *
 * \param  void
 *
 * \return void
 */
void callback_print_push(pointer text)
{
  printf("Pushed button: %s\n", text);
}

/**
 * \brief  Callback to turn on the LED.
 *
 * \param  void
 *
 * \return void
 */
void callback_led_on(pointer text)
{
  printf("LED ON. \n");
  printf("Released button %s.\n",text);
  switch(*(char*)text)
  {
    case '1':
      btnled_set_value(bsp_btnled_driver_handle, HMI_LED_1, HMI_VALUE_ON);
      break;
    case '2':
      btnled_set_value(bsp_btnled_driver_handle, HMI_LED_2, HMI_VALUE_ON);
      break;
    case '3':
      btnled_set_value(bsp_btnled_driver_handle, HMI_LED_3, HMI_VALUE_ON);
      break;
    case '4':
      btnled_set_value(bsp_btnled_driver_handle, HMI_LED_4, HMI_VALUE_ON);
      break;
    default:
      break;
  }
}

/**
 * \brief  Callback to turn off the LED.
 *
 * \param  void
 *
 * \return void
 */
void callback_led_off(pointer text)
{
  printf("LED OFF. \n");
  printf("Touched button %s.\n",text);
  switch(*(char*)text)
  {
    case '1':
      btnled_set_value(bsp_btnled_driver_handle, HMI_LED_1, HMI_VALUE_OFF);
      break;
    case '2':
      btnled_set_value(bsp_btnled_driver_handle, HMI_LED_2, HMI_VALUE_OFF);
      break;
    case '3':
      btnled_set_value(bsp_btnled_driver_handle, HMI_LED_3, HMI_VALUE_OFF);
      break;
    case '4':
      btnled_set_value(bsp_btnled_driver_handle, HMI_LED_4, HMI_VALUE_OFF);
      break;
    default:
      break;
  }
}

/**
 * \brief  Callback for a rotary movement.
 *
 * \param  void
 *
 * \return void
 */
void callback_rotary_movement(pointer text)
{
  printf("%s movement detected.\n", text);
}

/**
 * \brief  Callback for the released button.
 *
 * \param  void
 *
 * \return void
 */
void callback_print_release(pointer text)
{
  printf("Released button: %s\n", text);
}

/**
 * \brief  This function sets modules with a command in the shell.
 *
 * \param  void
 *
 * \return SHELL_EXIT_SUCCESS
 *         SHELL_EXIT_ERROR
 */
int_32 Shell_set_module(int_32 argc, char_ptr argv[] )
{
  boolean print_usage, shorthelp = FALSE;
  int_32  return_code = SHELL_EXIT_SUCCESS;
  uint_32 module_number;

  print_usage = Shell_check_help_request(argc, argv, &shorthelp);

  if (!print_usage)
  {
    if (argc > 2)
    {
      printf("Error, invalid number of parameters\n");
      return_code = SHELL_EXIT_ERROR;
      print_usage = TRUE;
    }
    else
    {
      if (argc == 2)
      {
        sscanf(argv[1],"%d", &module_number);
        if (module_number != _bsp_btnled_get_twrpi())
        {
          _lwsem_wait(&lwsem_tss);
          if (_bsp_btnled_set_twrpi(module_number) == MQX_OK)
          {
            printf("Module %d set.\n", module_number);
          }
          else
          {
            printf("Failed to set module %d.\n", module_number);
          }
          _lwsem_post(&lwsem_tss);
        }
        else
        {
          printf("Module %d already activated.\n", module_number);
        }
      }
    }
  }

  if (print_usage)
  {
    if (shorthelp)
    {
      printf("%s\n", argv[0]); /* shows in help command */
    }
    else
    {
      printf("Usage: %s number\n", argv[0]); /* shows available options in help */
      printf("Available TWRPI modules:\n");
#if defined(BSP_TWRPI_VOID)
      printf("%d BSP_TWRPI_VOID\n", BSP_TWRPI_VOID);
#endif
#if defined(BSP_TWRPI_ROTARY)
      printf("%d BSP_TWRPI_ROTARY\n", BSP_TWRPI_ROTARY);
#endif
#if defined(BSP_TWRPI_KEYPAD)
      printf("%d BSP_TWRPI_KEYPAD\n", BSP_TWRPI_KEYPAD);
#endif
    }
  }
  return return_code;
}

/**
 * \brief  This function prints actual module.
 *
 * \param  void
 *
 * \return SHELL_EXIT_SUCCESS
 *         SHELL_EXIT_ERROR
 */
int_32 Shell_get_module(int_32 argc, char_ptr argv[] )
{
  boolean print_usage, shorthelp = FALSE;
  int_32  return_code = SHELL_EXIT_SUCCESS;

  print_usage = Shell_check_help_request(argc, argv, &shorthelp );

  if (!print_usage)
  {
    if (argc > 2)
    {
      printf("Error, invalid number of parameters\n");
      return_code = SHELL_EXIT_ERROR;
      print_usage = TRUE;
    }
    else
    {
      if (argc == 1)
      {
        printf("Module %d active.\n", _bsp_btnled_get_twrpi());
      }
    }
  }

  if (print_usage)
  {
    printf("Usage: %s \n", argv[0]); /* show in help command */
  }
  return return_code;
}
