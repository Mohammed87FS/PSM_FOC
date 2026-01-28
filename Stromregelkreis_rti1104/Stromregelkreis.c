/*
 * Stromregelkreis.c
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

#include "Stromregelkreis_trc_ptr.h"
#include "Stromregelkreis.h"
#include <math.h>
#include "rtwtypes.h"
#include "Stromregelkreis_private.h"
#include "zero_crossing_types.h"
#include <string.h>

/* Block signals (default storage) */
B_Stromregelkreis_T Stromregelkreis_B;

/* Continuous states */
X_Stromregelkreis_T Stromregelkreis_X;

/* Disabled State Vector */
XDis_Stromregelkreis_T Stromregelkreis_XDis;

/* Block states (default storage) */
DW_Stromregelkreis_T Stromregelkreis_DW;

/* Previous zero-crossings (trigger) states */
PrevZCX_Stromregelkreis_T Stromregelkreis_PrevZCX;

/* Real-time model */
static RT_MODEL_Stromregelkreis_T Stromregelkreis_M_;
RT_MODEL_Stromregelkreis_T *const Stromregelkreis_M = &Stromregelkreis_M_;

/*
 * This function updates continuous states using the ODE3 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  /* Solver Matrices */
  static const real_T rt_ODE3_A[3] = {
    1.0/2.0, 3.0/4.0, 1.0
  };

  static const real_T rt_ODE3_B[3][3] = {
    { 1.0/2.0, 0.0, 0.0 },

    { 0.0, 3.0/4.0, 0.0 },

    { 2.0/9.0, 1.0/3.0, 4.0/9.0 }
  };

  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE3_IntgData *id = (ODE3_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T hB[3];
  int_T i;
  int_T nXc = 5;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  Stromregelkreis_derivatives();

  /* f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*)); */
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  Stromregelkreis_output();
  Stromregelkreis_derivatives();

  /* f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*)); */
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  Stromregelkreis_output();
  Stromregelkreis_derivatives();

  /* tnew = t + hA(3);
     ynew = y + f*hB(:,3); */
  for (i = 0; i <= 2; i++) {
    hB[i] = h * rt_ODE3_B[2][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2]);
  }

  rtsiSetT(si, tnew);
  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model output function */
void Stromregelkreis_output(void)
{
  real_T currentTime;
  real_T u1;
  real_T u2;
  boolean_T tmp;
  boolean_T zcEvent;
  if (rtmIsMajorTimeStep(Stromregelkreis_M)) {
    /* set solver stop time */
    if (!(Stromregelkreis_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&Stromregelkreis_M->solverInfo,
                            ((Stromregelkreis_M->Timing.clockTickH0 + 1) *
        Stromregelkreis_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&Stromregelkreis_M->solverInfo,
                            ((Stromregelkreis_M->Timing.clockTick0 + 1) *
        Stromregelkreis_M->Timing.stepSize0 +
        Stromregelkreis_M->Timing.clockTickH0 *
        Stromregelkreis_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(Stromregelkreis_M)) {
    Stromregelkreis_M->Timing.t[0] = rtsiGetT(&Stromregelkreis_M->solverInfo);
  }

  tmp = rtmIsMajorTimeStep(Stromregelkreis_M);
  if (tmp) {
    /* S-Function (rti_commonblock): '<S1>/S-Function' */
    /* This comment workarounds a code generation problem */
    ;

    /* Outputs for Triggered SubSystem: '<Root>/DS1104ENC_SET_POS_C1' incorporates:
     *  TriggerPort: '<S4>/Trigger'
     */
    if (rtsiIsModeUpdateTimeStep(&Stromregelkreis_M->solverInfo)) {
      zcEvent = (Stromregelkreis_B.SFunction_d &&
                 (Stromregelkreis_PrevZCX.DS1104ENC_SET_POS_C1_Trig_ZCE !=
                  POS_ZCSIG));
      if (zcEvent) {
        /* S-Function (rti_commonblock): '<S4>/S-Function1' */
        /* This comment workarounds a code generation problem */
        ;

        /* dSPACE I/O Board DS1104 Unit:ENC_SET */
        ds1104_inc_position_write(1, 0, DS1104_INC_LINE_SUBDIV_4);
      }

      Stromregelkreis_PrevZCX.DS1104ENC_SET_POS_C1_Trig_ZCE =
        Stromregelkreis_B.SFunction_d;
    }

    /* End of Outputs for SubSystem: '<Root>/DS1104ENC_SET_POS_C1' */

    /* Gain: '<Root>/n_nurmierung1' incorporates:
     *  Constant: '<Root>/n_soll'
     */
    Stromregelkreis_B.n_nurmierung1 = Stromregelkreis_P.n_nurmierung1_Gain *
      Stromregelkreis_P.n_soll_Value;

    /* S-Function (rti_commonblock): '<S2>/S-Function2' */
    /* This comment workarounds a code generation problem */
    ;

    /* Gain: '<Root>/Drehzahl in 1//min' */
    currentTime = 60.0 / (Stromregelkreis_P.ta * 4096.0);

    /* Gain: '<Root>/Drehzahl in 1//min' */
    Stromregelkreis_B.Drehzahlin1min = currentTime *
      Stromregelkreis_B.SFunction2;

    /* Gain: '<Root>/n_numierung2' */
    Stromregelkreis_B.n_numierung2 = Stromregelkreis_P.n_numierung2_Gain *
      Stromregelkreis_B.Drehzahlin1min;

    /* Sum: '<Root>/Sum' */
    Stromregelkreis_B.Sum = Stromregelkreis_B.n_nurmierung1 -
      Stromregelkreis_B.n_numierung2;

    /* Gain: '<S114>/Proportional Gain' */
    Stromregelkreis_B.ProportionalGain = Stromregelkreis_P.pid_P *
      Stromregelkreis_B.Sum;
  }

  /* Integrator: '<S109>/Integrator' */
  /* Limited  Integrator  */
  if (Stromregelkreis_X.Integrator_CSTATE >=
      Stromregelkreis_P.pid_UpperIntegratorSaturationLi) {
    Stromregelkreis_X.Integrator_CSTATE =
      Stromregelkreis_P.pid_UpperIntegratorSaturationLi;
  } else if (Stromregelkreis_X.Integrator_CSTATE <=
             Stromregelkreis_P.pid_LowerIntegratorSaturationLi) {
    Stromregelkreis_X.Integrator_CSTATE =
      Stromregelkreis_P.pid_LowerIntegratorSaturationLi;
  }

  /* Integrator: '<S109>/Integrator' */
  Stromregelkreis_B.Integrator = Stromregelkreis_X.Integrator_CSTATE;
  if (tmp) {
    /* Gain: '<S102>/Derivative Gain' */
    Stromregelkreis_B.DerivativeGain = Stromregelkreis_P.pid_D *
      Stromregelkreis_B.Sum;
  }

  /* Integrator: '<S104>/Filter' */
  Stromregelkreis_B.Filter = Stromregelkreis_X.Filter_CSTATE;

  /* Sum: '<S104>/SumD' */
  Stromregelkreis_B.SumD = Stromregelkreis_B.DerivativeGain -
    Stromregelkreis_B.Filter;

  /* Gain: '<S112>/Filter Coefficient' */
  Stromregelkreis_B.FilterCoefficient = Stromregelkreis_P.pid_N *
    Stromregelkreis_B.SumD;

  /* Sum: '<S118>/Sum' */
  Stromregelkreis_B.Sum_j = (Stromregelkreis_B.ProportionalGain +
    Stromregelkreis_B.Integrator) + Stromregelkreis_B.FilterCoefficient;

  /* Saturate: '<S116>/Saturation' */
  currentTime = Stromregelkreis_B.Sum_j;
  u1 = Stromregelkreis_P.pid_LowerSaturationLimit;
  u2 = Stromregelkreis_P.pid_UpperSaturationLimit;
  if (currentTime > u2) {
    /* Saturate: '<S116>/Saturation' */
    Stromregelkreis_B.Saturation = u2;
  } else if (currentTime < u1) {
    /* Saturate: '<S116>/Saturation' */
    Stromregelkreis_B.Saturation = u1;
  } else {
    /* Saturate: '<S116>/Saturation' */
    Stromregelkreis_B.Saturation = currentTime;
  }

  /* End of Saturate: '<S116>/Saturation' */

  /* Switch: '<Root>/n_pi_iq_sw' incorporates:
   *  Constant: '<Root>/n_Regelung_Start'
   *  Switch: '<Root>/iq_soll_enable_sw'
   */
  if (Stromregelkreis_P.n_Regelung_Start_Value >
      Stromregelkreis_P.n_pi_iq_sw_Threshold) {
    /* Switch: '<Root>/n_pi_iq_sw' */
    Stromregelkreis_B.n_pi_iq_sw = Stromregelkreis_B.Saturation;
  } else {
    if (Stromregelkreis_B.SFunction_d) {
      /* Switch: '<Root>/iq_soll_enable_sw' incorporates:
       *  Constant: '<Root>/iq_soll_safe'
       */
      Stromregelkreis_B.iq_soll_final = Stromregelkreis_P.iq_soll_safe_Value;
    } else {
      /* Switch: '<Root>/iq_soll_enable_sw' */
      Stromregelkreis_B.iq_soll_final = Stromregelkreis_B.Saturation;
    }

    /* Switch: '<Root>/n_pi_iq_sw' */
    Stromregelkreis_B.n_pi_iq_sw = Stromregelkreis_B.iq_soll_final;
  }

  /* End of Switch: '<Root>/n_pi_iq_sw' */

  /* Gain: '<S12>/Gain' */
  Stromregelkreis_B.Gain = Stromregelkreis_P.Gain_Gain_d *
    Stromregelkreis_B.n_pi_iq_sw;
  if (tmp) {
    /* S-Function (rti_commonblock): '<S2>/S-Function1' */
    /* This comment workarounds a code generation problem */
    ;

    /* Gain: '<Root>/mechanischer Winkel' */
    Stromregelkreis_B.mechanischerWinkel =
      Stromregelkreis_P.mechanischerWinkel_Gain * Stromregelkreis_B.SFunction1;

    /* Sum: '<Root>/Add2' incorporates:
     *  Constant: '<Root>/Offset'
     */
    Stromregelkreis_B.Add2 = Stromregelkreis_P.Offset_Value +
      Stromregelkreis_B.mechanischerWinkel;

    /* Gain: '<Root>/elektrischer Winkel' */
    Stromregelkreis_B.elektrischerWinkel =
      Stromregelkreis_P.elektrischerWinkel_Gain * Stromregelkreis_B.Add2;
  }

  /* Step: '<S9>/Step' */
  currentTime = Stromregelkreis_M->Timing.t[0];
  if (currentTime < Stromregelkreis_P.Ramp1_start) {
    /* Step: '<S9>/Step' */
    Stromregelkreis_B.Step = Stromregelkreis_P.Step_Y0;
  } else {
    /* Step: '<S9>/Step' */
    Stromregelkreis_B.Step = Stromregelkreis_P.Ramp1_slope;
  }

  /* End of Step: '<S9>/Step' */

  /* Clock: '<S9>/Clock' */
  Stromregelkreis_B.Clock = Stromregelkreis_M->Timing.t[0];

  /* Switch: '<Root>/Switch' */
  if (Stromregelkreis_B.SFunction_d) {
    /* Switch: '<Root>/Switch' */
    Stromregelkreis_B.Switch = Stromregelkreis_B.elektrischerWinkel;
  } else {
    /* Sum: '<S9>/Sum' incorporates:
     *  Constant: '<S9>/Constant'
     */
    Stromregelkreis_B.Sum_n = Stromregelkreis_B.Clock -
      Stromregelkreis_P.Ramp1_start;

    /* Product: '<S9>/Product' */
    Stromregelkreis_B.Product = Stromregelkreis_B.Step * Stromregelkreis_B.Sum_n;

    /* Sum: '<S9>/Output' incorporates:
     *  Constant: '<S9>/Constant1'
     */
    Stromregelkreis_B.Output = Stromregelkreis_B.Product +
      Stromregelkreis_P.Ramp1_InitialOutput;

    /* Switch: '<Root>/Switch' */
    Stromregelkreis_B.Switch = Stromregelkreis_B.Output;
  }

  /* End of Switch: '<Root>/Switch' */

  /* Trigonometry: '<S12>/Sin' */
  Stromregelkreis_B.Sin = sin(Stromregelkreis_B.Switch);

  /* Product: '<S12>/ia' */
  Stromregelkreis_B.ia = Stromregelkreis_B.Gain * Stromregelkreis_B.Sin;

  /* Switch: '<S10>/Switch' incorporates:
   *  Constant: '<S10>/ON//OFF'
   */
  if (Stromregelkreis_P.ONOFF_Value > Stromregelkreis_P.Switch_Threshold) {
    /* Switch: '<S11>/iu_sw' incorporates:
     *  Constant: '<S11>/thresh_sw'
     */
    if (Stromregelkreis_P.thresh_sw_Value > Stromregelkreis_P.iu_sw_Threshold) {
      /* Switch: '<S11>/iu_sw' incorporates:
       *  Constant: '<S11>/const_iu'
       */
      Stromregelkreis_B.iu_sw = Stromregelkreis_P.const_iu_Value;
    } else {
      /* Switch: '<S11>/iu_sw' */
      Stromregelkreis_B.iu_sw = Stromregelkreis_B.ia;
    }

    /* End of Switch: '<S11>/iu_sw' */

    /* Switch: '<S10>/Switch' */
    Stromregelkreis_B.Switch_k = Stromregelkreis_B.iu_sw;
  } else {
    /* Switch: '<S10>/Switch' incorporates:
     *  Constant: '<S10>/Startwert'
     */
    Stromregelkreis_B.Switch_k = Stromregelkreis_P.Startwert_Value;
  }

  /* End of Switch: '<S10>/Switch' */
  if (tmp) {
    /* S-Function (rti_commonblock): '<S19>/S-Function' */
    /* This comment workarounds a code generation problem */
    ;

    /* dSPACE I/O Board DS1104 #1 Unit:ADC Group:NON_MUX */
    Stromregelkreis_B.SFunction = (real_T) ds1104_adc_read_conv(2);

    /* Gain: '<S10>/Iu_ist' */
    Stromregelkreis_B.Iu_ist = Stromregelkreis_P.Iu_ist_Gain *
      Stromregelkreis_B.SFunction;
  }

  /* Sum: '<S10>/Sum' */
  Stromregelkreis_B.Sum_d = Stromregelkreis_B.Switch_k -
    Stromregelkreis_B.Iu_ist;

  /* Relay: '<S10>/Relay' incorporates:
   *  Relay: '<S10>/Relay1'
   *  Relay: '<S10>/Relay2'
   */
  zcEvent = rtsiIsModeUpdateTimeStep(&Stromregelkreis_M->solverInfo);
  if (zcEvent) {
    Stromregelkreis_DW.Relay_Mode = ((Stromregelkreis_B.Sum_d >=
      Stromregelkreis_P.Relay_OnVal) || ((!(Stromregelkreis_B.Sum_d <=
      Stromregelkreis_P.Relay_OffVal)) && Stromregelkreis_DW.Relay_Mode));
  }

  if (Stromregelkreis_DW.Relay_Mode) {
    /* Relay: '<S10>/Relay' */
    Stromregelkreis_B.Relay = Stromregelkreis_P.Relay_YOn;
  } else {
    /* Relay: '<S10>/Relay' */
    Stromregelkreis_B.Relay = Stromregelkreis_P.Relay_YOff;
  }

  /* End of Relay: '<S10>/Relay' */
  if (tmp) {
    /* S-Function (rti_commonblock): '<S5>/S-Function1' */
    /* This comment workarounds a code generation problem */
    ;

    /* dSPACE I/O Board DS1104 #1 Unit:SLV_BIT_IO Group:BIT_OUT */
    if (Stromregelkreis_B.Relay > 0) {
      ds1104_slave_dsp_bit_io_set(1, rti_slv1104_fcn_index[10],
        SLVDSP1104_BIT_IO_BIT3_MSK);
    } else {
      ds1104_slave_dsp_bit_io_clear(1, rti_slv1104_fcn_index[13],
        SLVDSP1104_BIT_IO_BIT3_MSK);
    }
  }

  /* Trigonometry: '<S12>/Cos' */
  Stromregelkreis_B.Cos = cos(Stromregelkreis_B.Switch);

  /* Product: '<S12>/ib' */
  Stromregelkreis_B.ib = Stromregelkreis_B.n_pi_iq_sw * Stromregelkreis_B.Cos;

  /* Switch: '<S10>/Switch1' incorporates:
   *  Constant: '<S10>/ON//OFF'
   */
  if (Stromregelkreis_P.ONOFF_Value > Stromregelkreis_P.Switch1_Threshold) {
    /* Switch: '<S11>/iv_sw' incorporates:
     *  Constant: '<S11>/thresh_sw'
     */
    if (Stromregelkreis_P.thresh_sw_Value > Stromregelkreis_P.iv_sw_Threshold) {
      /* Switch: '<S11>/iv_sw' incorporates:
       *  Constant: '<S11>/const_iv'
       */
      Stromregelkreis_B.iv_sw = Stromregelkreis_P.const_iv_Value;
    } else {
      /* Product: '<S11>/Product1' incorporates:
       *  Constant: '<S11>/Constant1'
       */
      Stromregelkreis_B.Product1 = Stromregelkreis_B.ib *
        Stromregelkreis_P.Constant1_Value;

      /* Gain: '<S11>/Gain1' */
      Stromregelkreis_B.Gain1_m = Stromregelkreis_P.Gain1_Gain *
        Stromregelkreis_B.ia;

      /* Sum: '<S11>/Add' */
      Stromregelkreis_B.Add_j = Stromregelkreis_B.Gain1_m +
        Stromregelkreis_B.Product1;

      /* Product: '<S11>/Product' incorporates:
       *  Constant: '<S11>/Constant'
       */
      Stromregelkreis_B.Product_a = Stromregelkreis_P.Constant_Value *
        Stromregelkreis_B.Add_j;

      /* Switch: '<S11>/iv_sw' */
      Stromregelkreis_B.iv_sw = Stromregelkreis_B.Product_a;
    }

    /* End of Switch: '<S11>/iv_sw' */

    /* Switch: '<S10>/Switch1' */
    Stromregelkreis_B.Switch1 = Stromregelkreis_B.iv_sw;
  } else {
    /* Switch: '<S10>/Switch1' incorporates:
     *  Constant: '<S10>/Startwert'
     */
    Stromregelkreis_B.Switch1 = Stromregelkreis_P.Startwert_Value;
  }

  /* End of Switch: '<S10>/Switch1' */
  if (tmp) {
    /* S-Function (rti_commonblock): '<S20>/S-Function' */
    /* This comment workarounds a code generation problem */
    ;

    /* dSPACE I/O Board DS1104 #1 Unit:ADC Group:NON_MUX */
    Stromregelkreis_B.SFunction_b = (real_T) ds1104_adc_read_conv(3);

    /* Gain: '<S10>/Iv_ist' */
    Stromregelkreis_B.Iv_ist = Stromregelkreis_P.Iv_ist_Gain *
      Stromregelkreis_B.SFunction_b;
  }

  /* Sum: '<S10>/Sum1' */
  Stromregelkreis_B.Sum1 = Stromregelkreis_B.Switch1 - Stromregelkreis_B.Iv_ist;

  /* Relay: '<S10>/Relay1' */
  if (zcEvent) {
    Stromregelkreis_DW.Relay1_Mode = ((Stromregelkreis_B.Sum1 >=
      Stromregelkreis_P.Relay1_OnVal) || ((!(Stromregelkreis_B.Sum1 <=
      Stromregelkreis_P.Relay1_OffVal)) && Stromregelkreis_DW.Relay1_Mode));
  }

  if (Stromregelkreis_DW.Relay1_Mode) {
    /* Relay: '<S10>/Relay1' */
    Stromregelkreis_B.Relay1 = Stromregelkreis_P.Relay1_YOn;
  } else {
    /* Relay: '<S10>/Relay1' */
    Stromregelkreis_B.Relay1 = Stromregelkreis_P.Relay1_YOff;
  }

  if (tmp) {
    /* S-Function (rti_commonblock): '<S6>/S-Function1' */
    /* This comment workarounds a code generation problem */
    ;

    /* dSPACE I/O Board DS1104 #1 Unit:SLV_BIT_IO Group:BIT_OUT */
    if (Stromregelkreis_B.Relay1 > 0) {
      ds1104_slave_dsp_bit_io_set(1, rti_slv1104_fcn_index[10],
        SLVDSP1104_BIT_IO_BIT4_MSK);
    } else {
      ds1104_slave_dsp_bit_io_clear(1, rti_slv1104_fcn_index[13],
        SLVDSP1104_BIT_IO_BIT4_MSK);
    }
  }

  /* Switch: '<S10>/Switch2' incorporates:
   *  Constant: '<S10>/ON//OFF'
   */
  if (Stromregelkreis_P.ONOFF_Value > Stromregelkreis_P.Switch2_Threshold) {
    /* Switch: '<S11>/iw_sw' incorporates:
     *  Constant: '<S11>/thresh_sw'
     */
    if (Stromregelkreis_P.thresh_sw_Value > Stromregelkreis_P.iw_sw_Threshold) {
      /* Switch: '<S11>/iw_sw' incorporates:
       *  Constant: '<S11>/const_iw'
       */
      Stromregelkreis_B.iw_sw = Stromregelkreis_P.const_iw_Value;
    } else {
      /* Product: '<S11>/Product3' incorporates:
       *  Constant: '<S11>/Constant3'
       */
      Stromregelkreis_B.Product3 = Stromregelkreis_B.ib *
        Stromregelkreis_P.Constant3_Value;

      /* Gain: '<S11>/Gain' */
      Stromregelkreis_B.Gain_i = Stromregelkreis_P.Gain_Gain *
        Stromregelkreis_B.ia;

      /* Sum: '<S11>/Add1' */
      Stromregelkreis_B.Add1 = Stromregelkreis_B.Gain_i +
        Stromregelkreis_B.Product3;

      /* Product: '<S11>/Product2' incorporates:
       *  Constant: '<S11>/Constant2'
       */
      Stromregelkreis_B.Product2 = Stromregelkreis_P.Constant2_Value *
        Stromregelkreis_B.Add1;

      /* Switch: '<S11>/iw_sw' */
      Stromregelkreis_B.iw_sw = Stromregelkreis_B.Product2;
    }

    /* End of Switch: '<S11>/iw_sw' */

    /* Switch: '<S10>/Switch2' */
    Stromregelkreis_B.Switch2 = Stromregelkreis_B.iw_sw;
  } else {
    /* Switch: '<S10>/Switch2' incorporates:
     *  Constant: '<S10>/Startwert'
     */
    Stromregelkreis_B.Switch2 = Stromregelkreis_P.Startwert_Value;
  }

  /* End of Switch: '<S10>/Switch2' */
  if (tmp) {
    /* S-Function (rti_commonblock): '<S21>/S-Function' */
    /* This comment workarounds a code generation problem */
    ;

    /* dSPACE I/O Board DS1104 #1 Unit:ADC Group:NON_MUX */
    Stromregelkreis_B.SFunction_e = (real_T) ds1104_adc_read_conv(4);

    /* Gain: '<S10>/Iw_ist' */
    Stromregelkreis_B.Iw_ist = Stromregelkreis_P.Iw_ist_Gain *
      Stromregelkreis_B.SFunction_e;
  }

  /* Sum: '<S10>/Sum2' */
  Stromregelkreis_B.Sum2 = Stromregelkreis_B.Switch2 - Stromregelkreis_B.Iw_ist;

  /* Relay: '<S10>/Relay2' */
  if (zcEvent) {
    Stromregelkreis_DW.Relay2_Mode = ((Stromregelkreis_B.Sum2 >=
      Stromregelkreis_P.Relay2_OnVal) || ((!(Stromregelkreis_B.Sum2 <=
      Stromregelkreis_P.Relay2_OffVal)) && Stromregelkreis_DW.Relay2_Mode));
  }

  if (Stromregelkreis_DW.Relay2_Mode) {
    /* Relay: '<S10>/Relay2' */
    Stromregelkreis_B.Relay2 = Stromregelkreis_P.Relay2_YOn;
  } else {
    /* Relay: '<S10>/Relay2' */
    Stromregelkreis_B.Relay2 = Stromregelkreis_P.Relay2_YOff;
  }

  if (tmp) {
    /* S-Function (rti_commonblock): '<S7>/S-Function1' */
    /* This comment workarounds a code generation problem */
    ;

    /* dSPACE I/O Board DS1104 #1 Unit:SLV_BIT_IO Group:BIT_OUT */
    if (Stromregelkreis_B.Relay2 > 0) {
      ds1104_slave_dsp_bit_io_set(1, rti_slv1104_fcn_index[10],
        SLVDSP1104_BIT_IO_BIT5_MSK);
    } else {
      ds1104_slave_dsp_bit_io_clear(1, rti_slv1104_fcn_index[13],
        SLVDSP1104_BIT_IO_BIT5_MSK);
    }

    /* Gain: '<S106>/Integral Gain' */
    Stromregelkreis_B.IntegralGain = Stromregelkreis_P.pid_I *
      Stromregelkreis_B.Sum;

    /* Gain: '<S62>/Proportional Gain' */
    Stromregelkreis_B.ProportionalGain_k = Stromregelkreis_P.pid_P_p * 0.0;
  }

  /* Sum: '<S101>/SumI2' */
  Stromregelkreis_B.SumI2 = Stromregelkreis_B.Saturation -
    Stromregelkreis_B.Sum_j;

  /* Gain: '<S101>/Kb' */
  Stromregelkreis_B.Kb = Stromregelkreis_P.pid_Kb * Stromregelkreis_B.SumI2;

  /* Sum: '<S120>/SumI3' */
  Stromregelkreis_B.SumI3 = 0.0 - Stromregelkreis_B.Saturation;

  /* Gain: '<S120>/Kt' */
  Stromregelkreis_B.Kt = Stromregelkreis_P.pid_Kt * Stromregelkreis_B.SumI3;

  /* Sum: '<S121>/SumI1' */
  Stromregelkreis_B.SumI1 = Stromregelkreis_B.IntegralGain +
    Stromregelkreis_B.Kt;

  /* Sum: '<S101>/SumI4' */
  Stromregelkreis_B.SumI4 = Stromregelkreis_B.Kb + Stromregelkreis_B.SumI1;

  /* Integrator: '<S57>/Integrator' */
  /* Limited  Integrator  */
  if (Stromregelkreis_X.Integrator_CSTATE_d >=
      Stromregelkreis_P.pid_UpperIntegratorSaturation_m) {
    Stromregelkreis_X.Integrator_CSTATE_d =
      Stromregelkreis_P.pid_UpperIntegratorSaturation_m;
  } else if (Stromregelkreis_X.Integrator_CSTATE_d <=
             Stromregelkreis_P.pid_LowerIntegratorSaturation_b) {
    Stromregelkreis_X.Integrator_CSTATE_d =
      Stromregelkreis_P.pid_LowerIntegratorSaturation_b;
  }

  /* Integrator: '<S57>/Integrator' */
  Stromregelkreis_B.Integrator_l = Stromregelkreis_X.Integrator_CSTATE_d;
  if (tmp) {
    /* Gain: '<S50>/Derivative Gain' */
    Stromregelkreis_B.DerivativeGain_j = Stromregelkreis_P.pid_D_k * 0.0;
  }

  /* Integrator: '<S52>/Filter' */
  Stromregelkreis_B.Filter_h = Stromregelkreis_X.Filter_CSTATE_k;

  /* Sum: '<S52>/SumD' */
  Stromregelkreis_B.SumD_o = Stromregelkreis_B.DerivativeGain_j -
    Stromregelkreis_B.Filter_h;

  /* Gain: '<S60>/Filter Coefficient' */
  Stromregelkreis_B.FilterCoefficient_l = Stromregelkreis_P.pid_N_i *
    Stromregelkreis_B.SumD_o;

  /* Sum: '<S66>/Sum' */
  Stromregelkreis_B.Sum_de = (Stromregelkreis_B.ProportionalGain_k +
    Stromregelkreis_B.Integrator_l) + Stromregelkreis_B.FilterCoefficient_l;

  /* Saturate: '<S64>/Saturation' */
  currentTime = Stromregelkreis_B.Sum_de;
  u1 = Stromregelkreis_P.pid_LowerSaturationLimit_f;
  u2 = Stromregelkreis_P.pid_UpperSaturationLimit_a;
  if (currentTime > u2) {
    /* Saturate: '<S64>/Saturation' */
    Stromregelkreis_B.Saturation_a = u2;
  } else if (currentTime < u1) {
    /* Saturate: '<S64>/Saturation' */
    Stromregelkreis_B.Saturation_a = u1;
  } else {
    /* Saturate: '<S64>/Saturation' */
    Stromregelkreis_B.Saturation_a = currentTime;
  }

  /* End of Saturate: '<S64>/Saturation' */

  /* Sum: '<S49>/SumI2' */
  Stromregelkreis_B.SumI2_f = Stromregelkreis_B.Saturation_a -
    Stromregelkreis_B.Sum_de;

  /* Gain: '<S49>/Kb' */
  Stromregelkreis_B.Kb_l = Stromregelkreis_P.pid_Kb_c *
    Stromregelkreis_B.SumI2_f;
  if (tmp) {
    /* Gain: '<S54>/Integral Gain' */
    Stromregelkreis_B.IntegralGain_o = Stromregelkreis_P.pid_I_o * 0.0;

    /* Gain: '<S14>/Gain' */
    Stromregelkreis_B.Gain_b = Stromregelkreis_P.Gain_Gain_o * 0.0;
  }

  /* Sum: '<S68>/SumI3' */
  Stromregelkreis_B.SumI3_n = 0.0 - Stromregelkreis_B.Saturation_a;

  /* Gain: '<S68>/Kt' */
  Stromregelkreis_B.Kt_f = Stromregelkreis_P.pid_Kt_e *
    Stromregelkreis_B.SumI3_n;

  /* Sum: '<S69>/SumI1' */
  Stromregelkreis_B.SumI1_d = Stromregelkreis_B.IntegralGain_o +
    Stromregelkreis_B.Kt_f;

  /* Sum: '<S49>/SumI4' */
  Stromregelkreis_B.SumI4_g = Stromregelkreis_B.Kb_l + Stromregelkreis_B.SumI1_d;

  /* Integrator: '<S14>/Integrator Limited' */
  /* Limited  Integrator  */
  if (Stromregelkreis_X.IntegratorLimited_CSTATE >=
      Stromregelkreis_P.IntegratorLimited_UpperSat) {
    Stromregelkreis_X.IntegratorLimited_CSTATE =
      Stromregelkreis_P.IntegratorLimited_UpperSat;
  } else if (Stromregelkreis_X.IntegratorLimited_CSTATE <=
             Stromregelkreis_P.IntegratorLimited_LowerSat) {
    Stromregelkreis_X.IntegratorLimited_CSTATE =
      Stromregelkreis_P.IntegratorLimited_LowerSat;
  }

  /* Integrator: '<S14>/Integrator Limited' */
  Stromregelkreis_B.IntegratorLimited =
    Stromregelkreis_X.IntegratorLimited_CSTATE;

  /* Sum: '<S14>/Add' */
  Stromregelkreis_B.Add = Stromregelkreis_B.Gain_b +
    Stromregelkreis_B.IntegratorLimited;
  if (tmp) {
    /* Gain: '<S14>/Gain1' */
    Stromregelkreis_B.Gain1 = Stromregelkreis_P.Gain1_Gain_d *
      Stromregelkreis_B.Gain_b;
  }

  /* Saturate: '<S14>/Saturation' */
  currentTime = Stromregelkreis_B.Add;
  u1 = Stromregelkreis_P.Saturation_LowerSat;
  u2 = Stromregelkreis_P.Saturation_UpperSat;
  if (currentTime > u2) {
    /* Saturate: '<S14>/Saturation' */
    Stromregelkreis_B.Saturation_k = u2;
  } else if (currentTime < u1) {
    /* Saturate: '<S14>/Saturation' */
    Stromregelkreis_B.Saturation_k = u1;
  } else {
    /* Saturate: '<S14>/Saturation' */
    Stromregelkreis_B.Saturation_k = currentTime;
  }

  /* End of Saturate: '<S14>/Saturation' */

  /* Gain: '<S14>/Gain2' */
  Stromregelkreis_B.Gain2 = Stromregelkreis_P.Gain2_Gain *
    Stromregelkreis_B.Saturation_k;
  if (tmp) {
    /* S-Function (rti_commonblock): '<S3>/S-Function1' */
    /* This comment workarounds a code generation problem */
    ;

    /* S-Function (rti_commonblock): '<S3>/S-Function2' */
    /* This comment workarounds a code generation problem */
    ;
  }
}

/* Model update function */
void Stromregelkreis_update(void)
{
  if (rtmIsMajorTimeStep(Stromregelkreis_M)) {
    rt_ertODEUpdateContinuousStates(&Stromregelkreis_M->solverInfo);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++Stromregelkreis_M->Timing.clockTick0)) {
    ++Stromregelkreis_M->Timing.clockTickH0;
  }

  Stromregelkreis_M->Timing.t[0] = rtsiGetSolverStopTime
    (&Stromregelkreis_M->solverInfo);

  {
    /* Update absolute timer for sample time: [5.0E-5s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The resolution of this integer timer is 5.0E-5, which is the step size
     * of the task. Size of "clockTick1" ensures timer will not overflow during the
     * application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    Stromregelkreis_M->Timing.clockTick1++;
    if (!Stromregelkreis_M->Timing.clockTick1) {
      Stromregelkreis_M->Timing.clockTickH1++;
    }
  }
}

/* Derivatives for root system: '<Root>' */
void Stromregelkreis_derivatives(void)
{
  XDot_Stromregelkreis_T *_rtXdot;
  boolean_T lsat;
  boolean_T usat;
  _rtXdot = ((XDot_Stromregelkreis_T *) Stromregelkreis_M->derivs);

  /* Derivatives for Integrator: '<S109>/Integrator' */
  lsat = (Stromregelkreis_X.Integrator_CSTATE <=
          Stromregelkreis_P.pid_LowerIntegratorSaturationLi);
  usat = (Stromregelkreis_X.Integrator_CSTATE >=
          Stromregelkreis_P.pid_UpperIntegratorSaturationLi);
  if (((!lsat) && (!usat)) || (lsat && (Stromregelkreis_B.SumI4 > 0.0)) || (usat
       && (Stromregelkreis_B.SumI4 < 0.0))) {
    _rtXdot->Integrator_CSTATE = Stromregelkreis_B.SumI4;
  } else {
    /* in saturation */
    _rtXdot->Integrator_CSTATE = 0.0;
  }

  /* End of Derivatives for Integrator: '<S109>/Integrator' */

  /* Derivatives for Integrator: '<S104>/Filter' */
  _rtXdot->Filter_CSTATE = Stromregelkreis_B.FilterCoefficient;

  /* Derivatives for Integrator: '<S57>/Integrator' */
  lsat = (Stromregelkreis_X.Integrator_CSTATE_d <=
          Stromregelkreis_P.pid_LowerIntegratorSaturation_b);
  usat = (Stromregelkreis_X.Integrator_CSTATE_d >=
          Stromregelkreis_P.pid_UpperIntegratorSaturation_m);
  if (((!lsat) && (!usat)) || (lsat && (Stromregelkreis_B.SumI4_g > 0.0)) ||
      (usat && (Stromregelkreis_B.SumI4_g < 0.0))) {
    _rtXdot->Integrator_CSTATE_d = Stromregelkreis_B.SumI4_g;
  } else {
    /* in saturation */
    _rtXdot->Integrator_CSTATE_d = 0.0;
  }

  /* End of Derivatives for Integrator: '<S57>/Integrator' */

  /* Derivatives for Integrator: '<S52>/Filter' */
  _rtXdot->Filter_CSTATE_k = Stromregelkreis_B.FilterCoefficient_l;

  /* Derivatives for Integrator: '<S14>/Integrator Limited' */
  lsat = (Stromregelkreis_X.IntegratorLimited_CSTATE <=
          Stromregelkreis_P.IntegratorLimited_LowerSat);
  usat = (Stromregelkreis_X.IntegratorLimited_CSTATE >=
          Stromregelkreis_P.IntegratorLimited_UpperSat);
  if (((!lsat) && (!usat)) || (lsat && (Stromregelkreis_B.Gain1 > 0.0)) || (usat
       && (Stromregelkreis_B.Gain1 < 0.0))) {
    _rtXdot->IntegratorLimited_CSTATE = Stromregelkreis_B.Gain1;
  } else {
    /* in saturation */
    _rtXdot->IntegratorLimited_CSTATE = 0.0;
  }

  /* End of Derivatives for Integrator: '<S14>/Integrator Limited' */
}

/* Model initialize function */
void Stromregelkreis_initialize(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)Stromregelkreis_M, 0,
                sizeof(RT_MODEL_Stromregelkreis_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&Stromregelkreis_M->solverInfo,
                          &Stromregelkreis_M->Timing.simTimeStep);
    rtsiSetTPtr(&Stromregelkreis_M->solverInfo, &rtmGetTPtr(Stromregelkreis_M));
    rtsiSetStepSizePtr(&Stromregelkreis_M->solverInfo,
                       &Stromregelkreis_M->Timing.stepSize0);
    rtsiSetdXPtr(&Stromregelkreis_M->solverInfo, &Stromregelkreis_M->derivs);
    rtsiSetContStatesPtr(&Stromregelkreis_M->solverInfo, (real_T **)
                         &Stromregelkreis_M->contStates);
    rtsiSetNumContStatesPtr(&Stromregelkreis_M->solverInfo,
      &Stromregelkreis_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&Stromregelkreis_M->solverInfo,
      &Stromregelkreis_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&Stromregelkreis_M->solverInfo,
      &Stromregelkreis_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&Stromregelkreis_M->solverInfo,
      &Stromregelkreis_M->periodicContStateRanges);
    rtsiSetContStateDisabledPtr(&Stromregelkreis_M->solverInfo, (boolean_T**)
      &Stromregelkreis_M->contStateDisabled);
    rtsiSetErrorStatusPtr(&Stromregelkreis_M->solverInfo, (&rtmGetErrorStatus
      (Stromregelkreis_M)));
    rtsiSetRTModelPtr(&Stromregelkreis_M->solverInfo, Stromregelkreis_M);
  }

  rtsiSetSimTimeStep(&Stromregelkreis_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetIsMinorTimeStepWithModeChange(&Stromregelkreis_M->solverInfo, false);
  rtsiSetIsContModeFrozen(&Stromregelkreis_M->solverInfo, false);
  Stromregelkreis_M->intgData.y = Stromregelkreis_M->odeY;
  Stromregelkreis_M->intgData.f[0] = Stromregelkreis_M->odeF[0];
  Stromregelkreis_M->intgData.f[1] = Stromregelkreis_M->odeF[1];
  Stromregelkreis_M->intgData.f[2] = Stromregelkreis_M->odeF[2];
  Stromregelkreis_M->contStates = ((X_Stromregelkreis_T *) &Stromregelkreis_X);
  Stromregelkreis_M->contStateDisabled = ((XDis_Stromregelkreis_T *)
    &Stromregelkreis_XDis);
  Stromregelkreis_M->Timing.tStart = (0.0);
  rtsiSetSolverData(&Stromregelkreis_M->solverInfo, (void *)
                    &Stromregelkreis_M->intgData);
  rtsiSetSolverName(&Stromregelkreis_M->solverInfo,"ode3");
  rtmSetTPtr(Stromregelkreis_M, &Stromregelkreis_M->Timing.tArray[0]);
  Stromregelkreis_M->Timing.stepSize0 = 5.0E-5;

  /* block I/O */
  (void) memset(((void *) &Stromregelkreis_B), 0,
                sizeof(B_Stromregelkreis_T));

  /* states (continuous) */
  {
    (void) memset((void *)&Stromregelkreis_X, 0,
                  sizeof(X_Stromregelkreis_T));
  }

  /* disabled states */
  {
    (void) memset((void *)&Stromregelkreis_XDis, 0,
                  sizeof(XDis_Stromregelkreis_T));
  }

  /* states (dwork) */
  (void) memset((void *)&Stromregelkreis_DW, 0,
                sizeof(DW_Stromregelkreis_T));

  {
    /* user code (registration function declaration) */
    /*Initialize global TRC pointers. */
    Stromregelkreis_rti_init_trc_pointers();
  }

  /* Start for S-Function (rti_commonblock): '<S1>/S-Function' */

  /* dSPACE I/O Board DS1104 Unit:ENC_HW_INDEX */
  Stromregelkreis_DW.SFunction_IWORK = 0;
  Stromregelkreis_PrevZCX.DS1104ENC_SET_POS_C1_Trig_ZCE = POS_ZCSIG;

  /* InitializeConditions for Integrator: '<S109>/Integrator' */
  Stromregelkreis_X.Integrator_CSTATE =
    Stromregelkreis_P.pid_InitialConditionForIntegrat;

  /* InitializeConditions for Integrator: '<S104>/Filter' */
  Stromregelkreis_X.Filter_CSTATE =
    Stromregelkreis_P.pid_InitialConditionForFilter;

  /* InitializeConditions for Integrator: '<S57>/Integrator' */
  Stromregelkreis_X.Integrator_CSTATE_d =
    Stromregelkreis_P.pid_InitialConditionForIntegr_p;

  /* InitializeConditions for Integrator: '<S52>/Filter' */
  Stromregelkreis_X.Filter_CSTATE_k =
    Stromregelkreis_P.pid_InitialConditionForFilter_d;

  /* InitializeConditions for Integrator: '<S14>/Integrator Limited' */
  Stromregelkreis_X.IntegratorLimited_CSTATE =
    Stromregelkreis_P.IntegratorLimited_IC;
}

/* Model terminate function */
void Stromregelkreis_terminate(void)
{
  /* (no terminate code required) */
}
