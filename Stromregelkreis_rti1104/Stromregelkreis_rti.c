/*********************** dSPACE target specific file *************************

   Include file Stromregelkreis_rti.c:

   Definition of functions and variables for the system I/O and for
   the hardware and software interrupts used.

   RTI1104 24.2 (02-Nov-2024)
   Tue Jan 27 16:36:06 2026

   Copyright 2026, dSPACE GmbH. All rights reserved.

 *****************************************************************************/

#if !(defined(__RTI_SIMENGINE__) || defined(RTIMP_FRAME))
# error This file may be included only by the RTI(-MP) simulation engine.
#endif

/* Include the model header file. */
#include "Stromregelkreis.h"
#include "Stromregelkreis_private.h"

/* Defines for block output and parameter structure existence */
#define RTI_rtB_STRUCTURE_EXISTS       1
#define RTI_rtP_STRUCTURE_EXISTS       1
#define RTB_STRUCTURE_NAME             Stromregelkreis_B
#define RTP_STRUCTURE_NAME             Stromregelkreis_P

/* dSPACE generated includes for header files */
#include <brtenv.h>
#include <rtkernel.h>
#include <rti_assert.h>
#include <rtidefineddatatypes.h>
#include <def1104.h>
#include <slvdsp1104.h>
#include <rti_slv1104.h>
#ifndef dsRtmGetNumSampleTimes
# define dsRtmGetNumSampleTimes(rtm)   2
#endif

#ifndef dsRtmGetTPtr
# define dsRtmGetTPtr(rtm)             ((rtm)->Timing.t)
#endif

#ifndef dsRtmSetTaskTime
# define dsRtmSetTaskTime(rtm, sti, val) (dsRtmGetTPtr((rtm))[sti] = (val))
#endif

/****** Definitions: task functions for timer tasks *********************/

/* Timer Task 1. (Base rate). */
static void rti_TIMERA(rtk_p_task_control_block task)
{
  /* Task entry code BEGIN */
  /* -- None. -- */
  /* Task entry code END */

  /* Task code. */
  baseRateService(task);

  /* Task exit code BEGIN */
  /* -- None. -- */
  /* Task exit code END */
}

/* ===== Definition of interface functions for simulation engine =========== */
#if GRTINTERFACE == 1
#ifdef MULTITASKING
# define dsIsSampleHit(RTM,sti)        rtmGetSampleHitPtr(RTM)[sti]
#else
# define dsIsSampleHit(RTM,sti)        rtmIsSampleHit(RTM,sti,0)
#endif

#else
#ifndef rtmStepTask
# define rtmStepTask(rtm, idx)         ((rtm)->Timing.TaskCounters.TID[(idx)] == 0)
#endif

# define dsIsSampleHit(RTM,sti)        rtmStepTask(RTM, sti)
#endif

#undef __INLINE
#if defined(_INLINE)
# define __INLINE                      static inline
#else
# define __INLINE                      static
#endif

/*Define additional variables*/
static time_T dsTFinal = -1.0;

#define dsGetTFinal(rtm)               (dsTFinal)

static time_T dsStepSize = 5.0E-5;

# define dsGetStepSize(rtm)            (dsStepSize)

static void rti_mdl_initialize_host_services(void)
{
  ts_timestamp_type ts = { 0, 0 };

  host_service(1, &ts);
}

static void rti_mdl_initialize_io_boards(void)
{
  /* Registering of RTI products and modules at VCM */
  {
    vcm_module_register(VCM_MID_RTI1104, (void *) 0,
                        VCM_TXT_RTI1104, 24, 2, 0,
                        VCM_VERSION_RELEASE, 0, 0, 0, VCM_CTRL_NO_ST);

    {
      vcm_module_descriptor_type* msg_mod_ptr;
      msg_mod_ptr = vcm_module_register(VCM_MID_MATLAB, (void *) 0,
        VCM_TXT_MATLAB, 24, 2, 0,
        VCM_VERSION_RELEASE, 0, 0, 0, VCM_CTRL_NO_ST);
      vcm_module_register(VCM_MID_SIMULINK, msg_mod_ptr,
                          VCM_TXT_SIMULINK, 24, 2, 0,
                          VCM_VERSION_RELEASE, 0, 0, 0, VCM_CTRL_NO_ST);
      vcm_module_register(VCM_MID_RTW, msg_mod_ptr,
                          VCM_TXT_RTW, 24, 2, 0,
                          VCM_VERSION_RELEASE, 0, 0, 0, VCM_CTRL_NO_ST);
      vcm_module_register(VCM_MID_STATEFLOW, msg_mod_ptr,
                          VCM_TXT_STATEFLOW, 24, 2, 0,
                          VCM_VERSION_RELEASE, 0, 0, 0, VCM_CTRL_NO_ST);
      vcm_module_register(VCM_MID_STATEFLOW_CODER, msg_mod_ptr,
                          VCM_TXT_STATEFLOW_CODER, 24, 2, 0,
                          VCM_VERSION_RELEASE, 0, 0, 0, VCM_CTRL_NO_ST);
    }
  }

  /* dSPACE I/O Board DS1104SLAVE #1 */
  /* Initialize master - slave DSP communication */
  ds1104_slave_dsp_communication_init();

  /* Initialize automatic index generation */
  rti_slv1104_init_fcn_index();

  /* Initialize slave DSP error level */
  rti_slv1104_error_level = 0;

  /* dSPACE I/O Board DS1104SLAVE #1 Unit:BIT_P2 */
  ds1104_slave_dsp_bit_io_init(1, 2, SLVDSP1104_BIT_IO_BIT3_MSK|
    SLVDSP1104_BIT_IO_BIT4_MSK|SLVDSP1104_BIT_IO_BIT5_MSK,
    SLVDSP1104_BIT_IO_BIT3_OUT|SLVDSP1104_BIT_IO_BIT4_OUT|
    SLVDSP1104_BIT_IO_BIT5_OUT);

  /* dSPACE I/O Board DS1104SLAVE #1 Unit:BIT_P2 Group:BIT_OUT_P2 */
  ds1104_slave_dsp_bit_io_set_register(1, &rti_slv1104_fcn_index[10], 2);
  ds1104_slave_dsp_bit_io_clear_register(1, &rti_slv1104_fcn_index[13], 2);

  /* dSPACE I/O Board DS1104 #1 Unit:ENC_SETUP */
  ds1104_inc_init(1, DS1104_INC_MODE_TTL);
  ds1104_inc_init(2, DS1104_INC_MODE_TTL);

  /* Initialization of Master ADC units */
  ds1104_adc_trigger_setup(2, DS1104_TRIGGER_DISABLE);
  ds1104_adc_trigger_setup(3, DS1104_TRIGGER_DISABLE);
  ds1104_adc_trigger_setup(4, DS1104_TRIGGER_DISABLE);

  /* Initialization of Master EncSetup units  */
  ds1104_inc_trigger_setup(1,DS1104_TRIGGER_DISABLE);
  ds1104_inc_trigger_setup(2,DS1104_TRIGGER_DISABLE);
}

/* Function rti_mdl_slave_load() is empty */
#define rti_mdl_slave_load()

/* Function rti_mdl_rtk_initialize() is empty */
#define rti_mdl_rtk_initialize()

static void rti_mdl_initialize_io_units(void)
{
  /* dSPACE I/O Board DS1104 #1 Unit:ENC_HW_INDEX */
  ds1104_inc_set_idxmode(1, DS1104_INC_IDXMODE_ON);

  /* dSPACE I/O Board DS1104 #1 Unit:ENC_POS */
  ds1104_inc_position_write(1, 0, DS1104_INC_LINE_SUBDIV_4);

  /* dSPACE I/O Board DS1104SLAVE #1 Unit:SLV_BIT_IO Group:BIT_OUT_CLEAR_P2 */
  rti_slv1104_taskqueue_error[1] = ds1104_slave_dsp_bit_io_clear(1,
    rti_slv1104_fcn_index[13], SLVDSP1104_BIT_IO_BIT3_MSK|
    SLVDSP1104_BIT_IO_BIT4_MSK|SLVDSP1104_BIT_IO_BIT5_MSK);
  rti_slv1104_taskqueue_error_check(1);
}

/* Function rti_mdl_acknowledge_interrupts() is empty */
#define rti_mdl_acknowledge_interrupts()

/* Function rti_mdl_timetables_register() is empty */
#define rti_mdl_timetables_register()

static void rti_mdl_background(void)
{
  /* dSPACE I/O Board DS1104SLAVE #1 */
  /* Check master - slave dsp communication */
  rti_slv1104_taskqueue_error_all_check();
}

__INLINE void rti_mdl_sample_input(void)
{
  /* Calls for base sample time: [5.0E-5, 0] */
  /* dSPACE I/O Board DS1104 #1 Unit:ENC_HW_INDEX Group:ENC_HW_INDEX */
  if (!(Stromregelkreis_DW.SFunction_IWORK)) {
    if (Stromregelkreis_B.SFunction_d = (boolean_T) ds1104_inc_index_read(1,
         DS1104_INC_IDXMODE_OFF))
      Stromregelkreis_DW.SFunction_IWORK = 1;
  } else
    Stromregelkreis_B.SFunction_d = 1;

  /* dSPACE I/O Board DS1104 #1 Unit:ADC */
  ds1104_adc_start(DS1104_ADC2|DS1104_ADC3|DS1104_ADC4);

  /* dSPACE I/O Board DS1104 #1 Unit:ENC_POS Group:POS */
  Stromregelkreis_B.SFunction1 = (real_T) ds1104_inc_position_read(1,
    DS1104_INC_LINE_SUBDIV_4);

  /* dSPACE I/O Board DS1104 #1 Unit:ENC_POS Group:DPOS */
  Stromregelkreis_B.SFunction2 = (real_T) ds1104_inc_delta_position_read(1,
    DS1104_INC_LINE_SUBDIV_4);
}

static void rti_mdl_daq_service()
{
  /* dSPACE Host Service */
  host_service(1, rtk_current_task_absolute_time_ptr_get());
}

#undef __INLINE

/****** [EOF] ****************************************************************/
