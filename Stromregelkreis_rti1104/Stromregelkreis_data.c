/*
 * Stromregelkreis_data.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "Stromregelkreis".
 *
 * Model version              : 1.43
 * Simulink Coder version : 24.2 (R2024b) 21-Jun-2024
 * C source code generated on : Tue Jan 27 16:36:06 2026
 *
 * Target selection: rti1104.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Custom Processor->Custom
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Stromregelkreis.h"

/* Block parameters (default storage) */
P_Stromregelkreis_T Stromregelkreis_P = {
  /* Variable: ta
   * Referenced by: '<Root>/Drehzahl in 1//min'
   */
  5.0E-5,

  /* Mask Parameter: pid_D
   * Referenced by: '<S102>/Derivative Gain'
   */
  0.0,

  /* Mask Parameter: pid_D_k
   * Referenced by: '<S50>/Derivative Gain'
   */
  0.0,

  /* Mask Parameter: pid_I
   * Referenced by: '<S106>/Integral Gain'
   */
  486.88,

  /* Mask Parameter: pid_I_o
   * Referenced by: '<S54>/Integral Gain'
   */
  18.11,

  /* Mask Parameter: pid_InitialConditionForFilter
   * Referenced by: '<S104>/Filter'
   */
  0.0,

  /* Mask Parameter: pid_InitialConditionForFilter_d
   * Referenced by: '<S52>/Filter'
   */
  0.0,

  /* Mask Parameter: pid_InitialConditionForIntegrat
   * Referenced by: '<S109>/Integrator'
   */
  0.0,

  /* Mask Parameter: pid_InitialConditionForIntegr_p
   * Referenced by: '<S57>/Integrator'
   */
  0.0,

  /* Mask Parameter: Ramp1_InitialOutput
   * Referenced by: '<S9>/Constant1'
   */
  0.0,

  /* Mask Parameter: pid_Kb
   * Referenced by: '<S101>/Kb'
   */
  22.06,

  /* Mask Parameter: pid_Kb_c
   * Referenced by: '<S49>/Kb'
   */
  5.319,

  /* Mask Parameter: pid_Kt
   * Referenced by: '<S120>/Kt'
   */
  22.06,

  /* Mask Parameter: pid_Kt_e
   * Referenced by: '<S68>/Kt'
   */
  5.319,

  /* Mask Parameter: pid_LowerIntegratorSaturationLi
   * Referenced by: '<S109>/Integrator'
   */
  -1.0,

  /* Mask Parameter: pid_LowerIntegratorSaturation_b
   * Referenced by: '<S57>/Integrator'
   */
  -1.0,

  /* Mask Parameter: pid_LowerSaturationLimit
   * Referenced by: '<S116>/Saturation'
   */
  -1.0,

  /* Mask Parameter: pid_LowerSaturationLimit_f
   * Referenced by: '<S64>/Saturation'
   */
  -1.0,

  /* Mask Parameter: pid_N
   * Referenced by: '<S112>/Filter Coefficient'
   */
  100.0,

  /* Mask Parameter: pid_N_i
   * Referenced by: '<S60>/Filter Coefficient'
   */
  100.0,

  /* Mask Parameter: pid_P
   * Referenced by: '<S114>/Proportional Gain'
   */
  22.065,

  /* Mask Parameter: pid_P_p
   * Referenced by: '<S62>/Proportional Gain'
   */
  3.404,

  /* Mask Parameter: pid_UpperIntegratorSaturationLi
   * Referenced by: '<S109>/Integrator'
   */
  1.0,

  /* Mask Parameter: pid_UpperIntegratorSaturation_m
   * Referenced by: '<S57>/Integrator'
   */
  1.0,

  /* Mask Parameter: pid_UpperSaturationLimit
   * Referenced by: '<S116>/Saturation'
   */
  1.0,

  /* Mask Parameter: pid_UpperSaturationLimit_a
   * Referenced by: '<S64>/Saturation'
   */
  1.0,

  /* Mask Parameter: Ramp1_slope
   * Referenced by: '<S9>/Step'
   */
  18.849555921538759,

  /* Mask Parameter: Ramp1_start
   * Referenced by:
   *   '<S9>/Constant'
   *   '<S9>/Step'
   */
  0.0,

  /* Expression: -1
   * Referenced by: '<S11>/Gain1'
   */
  -1.0,

  /* Expression: 0
   * Referenced by: '<S11>/iv_sw'
   */
  0.0,

  /* Expression: -1
   * Referenced by: '<S11>/Gain'
   */
  -1.0,

  /* Expression: 0
   * Referenced by: '<S11>/iw_sw'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S11>/iu_sw'
   */
  0.0,

  /* Expression: 1
   * Referenced by: '<S11>/const_iu'
   */
  1.0,

  /* Expression: 0
   * Referenced by: '<S11>/thresh_sw'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<Root>/n_soll'
   */
  0.0,

  /* Expression: 1/3000
   * Referenced by: '<Root>/n_nurmierung1'
   */
  0.00033333333333333332,

  /* Expression: 1/3000
   * Referenced by: '<Root>/n_numierung2'
   */
  0.00033333333333333332,

  /* Expression: 0
   * Referenced by: '<Root>/n_Regelung_Start'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<Root>/iq_soll_safe'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<Root>/n_pi_iq_sw'
   */
  0.0,

  /* Expression: -1
   * Referenced by: '<S12>/Gain'
   */
  -1.0,

  /* Expression: 0.53
   * Referenced by: '<Root>/Offset'
   */
  0.53,

  /* Expression: (2*pi)/4096
   * Referenced by: '<Root>/mechanischer Winkel'
   */
  0.0015339807878856412,

  /* Expression: 3
   * Referenced by: '<Root>/elektrischer Winkel'
   */
  3.0,

  /* Expression: 0
   * Referenced by: '<S9>/Step'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S10>/ON//OFF'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S10>/Startwert'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S10>/Switch'
   */
  0.0,

  /* Expression: 4.46*2
   * Referenced by: '<S10>/Iu_ist'
   */
  8.92,

  /* Expression: 0.1
   * Referenced by: '<S10>/Relay'
   */
  0.1,

  /* Expression: -0.1
   * Referenced by: '<S10>/Relay'
   */
  -0.1,

  /* Expression: -0.5
   * Referenced by: '<S11>/const_iv'
   */
  -0.5,

  /* Expression: 0.5
   * Referenced by: '<S11>/Constant'
   */
  0.5,

  /* Expression: -sqrt(3)
   * Referenced by: '<S11>/Constant1'
   */
  -1.7320508075688772,

  /* Expression: 0
   * Referenced by: '<S10>/Switch1'
   */
  0.0,

  /* Expression: 4.46*2
   * Referenced by: '<S10>/Iv_ist'
   */
  8.92,

  /* Expression: 0.1
   * Referenced by: '<S10>/Relay1'
   */
  0.1,

  /* Expression: -0.1
   * Referenced by: '<S10>/Relay1'
   */
  -0.1,

  /* Expression: -0.5
   * Referenced by: '<S11>/const_iw'
   */
  -0.5,

  /* Expression: 0.5
   * Referenced by: '<S11>/Constant2'
   */
  0.5,

  /* Expression: +sqrt(3)
   * Referenced by: '<S11>/Constant3'
   */
  1.7320508075688772,

  /* Expression: 0
   * Referenced by: '<S10>/Switch2'
   */
  0.0,

  /* Expression: 4.46*2
   * Referenced by: '<S10>/Iw_ist'
   */
  8.92,

  /* Expression: 0.1
   * Referenced by: '<S10>/Relay2'
   */
  0.1,

  /* Expression: -0.1
   * Referenced by: '<S10>/Relay2'
   */
  -0.1,

  /* Expression: 3.5
   * Referenced by: '<S14>/Gain'
   */
  3.5,

  /* Expression: 0
   * Referenced by: '<S14>/Integrator Limited'
   */
  0.0,

  /* Expression: 1
   * Referenced by: '<S14>/Integrator Limited'
   */
  1.0,

  /* Expression: 0
   * Referenced by: '<S14>/Integrator Limited'
   */
  0.0,

  /* Expression: 5.35
   * Referenced by: '<S14>/Gain1'
   */
  5.35,

  /* Expression: 0.5
   * Referenced by: '<S14>/Saturation'
   */
  0.5,

  /* Expression: -0.5
   * Referenced by: '<S14>/Saturation'
   */
  -0.5,

  /* Expression: 1
   * Referenced by: '<S14>/Gain2'
   */
  1.0,

  /* Computed Parameter: Relay_YOn
   * Referenced by: '<S10>/Relay'
   */
  true,

  /* Computed Parameter: Relay_YOff
   * Referenced by: '<S10>/Relay'
   */
  false,

  /* Computed Parameter: Relay1_YOn
   * Referenced by: '<S10>/Relay1'
   */
  true,

  /* Computed Parameter: Relay1_YOff
   * Referenced by: '<S10>/Relay1'
   */
  false,

  /* Computed Parameter: Relay2_YOn
   * Referenced by: '<S10>/Relay2'
   */
  true,

  /* Computed Parameter: Relay2_YOff
   * Referenced by: '<S10>/Relay2'
   */
  false
};
