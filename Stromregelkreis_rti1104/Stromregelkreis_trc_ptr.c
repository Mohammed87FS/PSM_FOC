/***************************************************************************

   Source file Stromregelkreis_trc_ptr.c:

   Definition of function that initializes the global TRC pointers

   RTI1104 24.2 (02-Nov-2024)
   Tue Jan 27 16:36:06 2026

   Copyright 2026, dSPACE GmbH. All rights reserved.

 *****************************************************************************/

/* Include header file. */
#include "Stromregelkreis_trc_ptr.h"
#include "Stromregelkreis.h"
#include "Stromregelkreis_private.h"

/* Compiler options to turn off optimization. */
#if !defined(DS_OPTIMIZE_INIT_TRC_POINTERS)
#ifdef _MCCPPC

#pragma options -nOt -nOr -nOi -nOx

#endif

#ifdef __GNUC__

#pragma GCC optimize ("O0")

#endif

#ifdef _MSC_VER

#pragma optimize ("", off)

#endif
#endif

/* Definition of Global pointers to data type transitions (for TRC-file access) */
volatile real_T *p_0_Stromregelkreis_real_T_0 = NULL;
volatile boolean_T *p_0_Stromregelkreis_boolean_T_1 = NULL;
volatile real_T *p_1_Stromregelkreis_real_T_0 = NULL;
volatile boolean_T *p_1_Stromregelkreis_boolean_T_1 = NULL;
volatile int_T *p_2_Stromregelkreis_int_T_0 = NULL;
volatile boolean_T *p_2_Stromregelkreis_boolean_T_1 = NULL;
volatile real_T *p_3_Stromregelkreis_real_T_0 = NULL;

/*
 *  Declare the functions, that initially assign TRC pointers
 */
static void rti_init_trc_pointers_0(void);

/* Global pointers to data type transitions are separated in different functions to avoid overloading */
static void rti_init_trc_pointers_0(void)
{
  p_0_Stromregelkreis_real_T_0 = &Stromregelkreis_B.n_nurmierung1;
  p_0_Stromregelkreis_boolean_T_1 = &Stromregelkreis_B.SFunction_d;
  p_1_Stromregelkreis_real_T_0 = &Stromregelkreis_P.ta;
  p_1_Stromregelkreis_boolean_T_1 = &Stromregelkreis_P.Relay_YOn;
  p_2_Stromregelkreis_int_T_0 = &Stromregelkreis_DW.SFunction_IWORK;
  p_2_Stromregelkreis_boolean_T_1 = &Stromregelkreis_DW.Relay_Mode;
  p_3_Stromregelkreis_real_T_0 = &Stromregelkreis_X.Integrator_CSTATE;
}

void Stromregelkreis_rti_init_trc_pointers(void)
{
  rti_init_trc_pointers_0();
}
