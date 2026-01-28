/*********************** dSPACE target specific file *************************

   Header file Stromregelkreis_trc_ptr.h:

   Declaration of function that initializes the global TRC pointers

   RTI1104 24.2 (02-Nov-2024)
   Tue Jan 27 16:36:06 2026

   Copyright 2026, dSPACE GmbH. All rights reserved.

 *****************************************************************************/
#ifndef RTI_HEADER_Stromregelkreis_trc_ptr_h_
#define RTI_HEADER_Stromregelkreis_trc_ptr_h_

/* Include the model header file. */
#include "Stromregelkreis.h"
#include "Stromregelkreis_private.h"
#ifdef EXTERN_C
#undef EXTERN_C
#endif

#ifdef __cplusplus
#define EXTERN_C                       extern "C"
#else
#define EXTERN_C                       extern
#endif

/*
 *  Declare the global TRC pointers
 */
EXTERN_C volatile real_T *p_0_Stromregelkreis_real_T_0;
EXTERN_C volatile boolean_T *p_0_Stromregelkreis_boolean_T_1;
EXTERN_C volatile real_T *p_1_Stromregelkreis_real_T_0;
EXTERN_C volatile boolean_T *p_1_Stromregelkreis_boolean_T_1;
EXTERN_C volatile int_T *p_2_Stromregelkreis_int_T_0;
EXTERN_C volatile boolean_T *p_2_Stromregelkreis_boolean_T_1;
EXTERN_C volatile real_T *p_3_Stromregelkreis_real_T_0;

/*
 *  Declare the general function for TRC pointer initialization
 */
EXTERN_C void Stromregelkreis_rti_init_trc_pointers(void);

#endif                               /* RTI_HEADER_Stromregelkreis_trc_ptr_h_ */
