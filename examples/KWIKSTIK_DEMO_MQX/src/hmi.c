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
*   This file contains the source for the KWIKSTICK HMI example program.
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
void callback_print_release(pointer);
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

/**
 * \brief  This task initializes HMI.
 *
 * \param  void
 *
 * \return void
 */
void Main_task(uint_32 initial_data)
{
  printf("HMI demo application. \nPress buttons and electrodes available on the board.\n");
  printf("Set TWRPI module with module command.\n");
  _SLCDModule_Init(); /* SLCD init */
  _SLCDModule_TurnOnFreescaleSign();  /* Freescale sign on display */

  bsp_btnled_driver_handle = _bsp_btnled_init(); /* BTNLED initialization */
  if (bsp_btnled_driver_handle == NULL)
  {
    /* error in initialization */
    printf("BTNLED initialization failed. \n");
    _task_block();
  }

  /* add callbacks */
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_1, HMI_VALUE_PUSH, callback_print_push, "BUT 1");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_1, HMI_VALUE_RELEASE, callback_print_release, "");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_2, HMI_VALUE_PUSH, callback_print_push, "BUT 2");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_2, HMI_VALUE_RELEASE, callback_print_release, "");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_3, HMI_VALUE_PUSH, callback_print_push, "BUT 3");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_3, HMI_VALUE_RELEASE, callback_print_release, "");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_4, HMI_VALUE_PUSH, callback_print_push, "BUT 4");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_4, HMI_VALUE_RELEASE, callback_print_release, "");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_5, HMI_VALUE_PUSH, callback_print_push, "BUT 5");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_5, HMI_VALUE_RELEASE, callback_print_release, "");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_6, HMI_VALUE_PUSH, callback_print_push, "BUT 6");
  btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_6, HMI_VALUE_RELEASE, callback_print_release, "");
  while (1)
  {
    btnled_poll(bsp_btnled_driver_handle);  /* polls TSS and LWGPIO buttons */
    _time_delay(5);                         /* time for shell */
  }
}

/**
 * \brief  Callback for pushed button. Function prints the number of pushed button.
 *
 * \param  void
 *
 * \return void
 */
void callback_print_push(pointer text)
{
  _SLCDModule_PrintString(text, 0);
  printf("Pushed button: %s\n", text);
}

/**
 * \brief  Callback for released button. Function prints the number of released button.
 *
 * \param  void
 *
 * \return void
 */
void callback_print_release(pointer text)
{
  _SLCDModule_ClearLCD(0);
  printf("Released button.\n");
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
          if (_bsp_btnled_set_twrpi(module_number) == MQX_OK)
          {
            printf("Module %d set.\n", module_number);
          }
          else
          {
            printf("Failed to set module %d.\n", module_number);
          }
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
