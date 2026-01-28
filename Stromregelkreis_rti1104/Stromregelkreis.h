/*
 * Stromregelkreis.h
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

#ifndef Stromregelkreis_h_
#define Stromregelkreis_h_
#ifndef Stromregelkreis_COMMON_INCLUDES_
#define Stromregelkreis_COMMON_INCLUDES_
#include <brtenv.h>
#include <rtkernel.h>
#include <rti_assert.h>
#include <rtidefineddatatypes.h>
#include <def1104.h>
#include <slvdsp1104.h>
#include <rti_slv1104.h>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* Stromregelkreis_COMMON_INCLUDES_ */

#include "Stromregelkreis_types.h"
#include <string.h>
#include "zero_crossing_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetContStateDisabled
#define rtmGetContStateDisabled(rtm)   ((rtm)->contStateDisabled)
#endif

#ifndef rtmSetContStateDisabled
#define rtmSetContStateDisabled(rtm, val) ((rtm)->contStateDisabled = (val))
#endif

#ifndef rtmGetContStates
#define rtmGetContStates(rtm)          ((rtm)->contStates)
#endif

#ifndef rtmSetContStates
#define rtmSetContStates(rtm, val)     ((rtm)->contStates = (val))
#endif

#ifndef rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm) ((rtm)->CTOutputIncnstWithState)
#endif

#ifndef rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm, val) ((rtm)->CTOutputIncnstWithState = (val))
#endif

#ifndef rtmGetDerivCacheNeedsReset
#define rtmGetDerivCacheNeedsReset(rtm) ((rtm)->derivCacheNeedsReset)
#endif

#ifndef rtmSetDerivCacheNeedsReset
#define rtmSetDerivCacheNeedsReset(rtm, val) ((rtm)->derivCacheNeedsReset = (val))
#endif

#ifndef rtmGetIntgData
#define rtmGetIntgData(rtm)            ((rtm)->intgData)
#endif

#ifndef rtmSetIntgData
#define rtmSetIntgData(rtm, val)       ((rtm)->intgData = (val))
#endif

#ifndef rtmGetOdeF
#define rtmGetOdeF(rtm)                ((rtm)->odeF)
#endif

#ifndef rtmSetOdeF
#define rtmSetOdeF(rtm, val)           ((rtm)->odeF = (val))
#endif

#ifndef rtmGetOdeY
#define rtmGetOdeY(rtm)                ((rtm)->odeY)
#endif

#ifndef rtmSetOdeY
#define rtmSetOdeY(rtm, val)           ((rtm)->odeY = (val))
#endif

#ifndef rtmGetPeriodicContStateIndices
#define rtmGetPeriodicContStateIndices(rtm) ((rtm)->periodicContStateIndices)
#endif

#ifndef rtmSetPeriodicContStateIndices
#define rtmSetPeriodicContStateIndices(rtm, val) ((rtm)->periodicContStateIndices = (val))
#endif

#ifndef rtmGetPeriodicContStateRanges
#define rtmGetPeriodicContStateRanges(rtm) ((rtm)->periodicContStateRanges)
#endif

#ifndef rtmSetPeriodicContStateRanges
#define rtmSetPeriodicContStateRanges(rtm, val) ((rtm)->periodicContStateRanges = (val))
#endif

#ifndef rtmGetZCCacheNeedsReset
#define rtmGetZCCacheNeedsReset(rtm)   ((rtm)->zCCacheNeedsReset)
#endif

#ifndef rtmSetZCCacheNeedsReset
#define rtmSetZCCacheNeedsReset(rtm, val) ((rtm)->zCCacheNeedsReset = (val))
#endif

#ifndef rtmGetdX
#define rtmGetdX(rtm)                  ((rtm)->derivs)
#endif

#ifndef rtmSetdX
#define rtmSetdX(rtm, val)             ((rtm)->derivs = (val))
#endif

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

#ifndef rtmGetTStart
#define rtmGetTStart(rtm)              ((rtm)->Timing.tStart)
#endif

/* Block signals (default storage) */
typedef struct {
  real_T n_nurmierung1;                /* '<Root>/n_nurmierung1' */
  real_T SFunction2;                   /* '<S2>/S-Function2' */
  real_T Drehzahlin1min;               /* '<Root>/Drehzahl in 1//min' */
  real_T n_numierung2;                 /* '<Root>/n_numierung2' */
  real_T Sum;                          /* '<Root>/Sum' */
  real_T ProportionalGain;             /* '<S114>/Proportional Gain' */
  real_T Integrator;                   /* '<S109>/Integrator' */
  real_T DerivativeGain;               /* '<S102>/Derivative Gain' */
  real_T Filter;                       /* '<S104>/Filter' */
  real_T SumD;                         /* '<S104>/SumD' */
  real_T FilterCoefficient;            /* '<S112>/Filter Coefficient' */
  real_T Sum_j;                        /* '<S118>/Sum' */
  real_T Saturation;                   /* '<S116>/Saturation' */
  real_T n_pi_iq_sw;                   /* '<Root>/n_pi_iq_sw' */
  real_T Gain;                         /* '<S12>/Gain' */
  real_T SFunction1;                   /* '<S2>/S-Function1' */
  real_T mechanischerWinkel;           /* '<Root>/mechanischer Winkel' */
  real_T Add2;                         /* '<Root>/Add2' */
  real_T elektrischerWinkel;           /* '<Root>/elektrischer Winkel' */
  real_T Step;                         /* '<S9>/Step' */
  real_T Clock;                        /* '<S9>/Clock' */
  real_T Switch;                       /* '<Root>/Switch' */
  real_T Sin;                          /* '<S12>/Sin' */
  real_T ia;                           /* '<S12>/ia' */
  real_T Switch_k;                     /* '<S10>/Switch' */
  real_T SFunction;                    /* '<S19>/S-Function' */
  real_T Iu_ist;                       /* '<S10>/Iu_ist' */
  real_T Sum_d;                        /* '<S10>/Sum' */
  real_T Cos;                          /* '<S12>/Cos' */
  real_T ib;                           /* '<S12>/ib' */
  real_T Switch1;                      /* '<S10>/Switch1' */
  real_T SFunction_b;                  /* '<S20>/S-Function' */
  real_T Iv_ist;                       /* '<S10>/Iv_ist' */
  real_T Sum1;                         /* '<S10>/Sum1' */
  real_T Switch2;                      /* '<S10>/Switch2' */
  real_T SFunction_e;                  /* '<S21>/S-Function' */
  real_T Iw_ist;                       /* '<S10>/Iw_ist' */
  real_T Sum2;                         /* '<S10>/Sum2' */
  real_T SumI2;                        /* '<S101>/SumI2' */
  real_T Kb;                           /* '<S101>/Kb' */
  real_T IntegralGain;                 /* '<S106>/Integral Gain' */
  real_T SumI3;                        /* '<S120>/SumI3' */
  real_T Kt;                           /* '<S120>/Kt' */
  real_T SumI1;                        /* '<S121>/SumI1' */
  real_T SumI4;                        /* '<S101>/SumI4' */
  real_T ProportionalGain_k;           /* '<S62>/Proportional Gain' */
  real_T Integrator_l;                 /* '<S57>/Integrator' */
  real_T DerivativeGain_j;             /* '<S50>/Derivative Gain' */
  real_T Filter_h;                     /* '<S52>/Filter' */
  real_T SumD_o;                       /* '<S52>/SumD' */
  real_T FilterCoefficient_l;          /* '<S60>/Filter Coefficient' */
  real_T Sum_de;                       /* '<S66>/Sum' */
  real_T Saturation_a;                 /* '<S64>/Saturation' */
  real_T SumI2_f;                      /* '<S49>/SumI2' */
  real_T Kb_l;                         /* '<S49>/Kb' */
  real_T IntegralGain_o;               /* '<S54>/Integral Gain' */
  real_T SumI3_n;                      /* '<S68>/SumI3' */
  real_T Kt_f;                         /* '<S68>/Kt' */
  real_T SumI1_d;                      /* '<S69>/SumI1' */
  real_T SumI4_g;                      /* '<S49>/SumI4' */
  real_T Gain_b;                       /* '<S14>/Gain' */
  real_T IntegratorLimited;            /* '<S14>/Integrator Limited' */
  real_T Add;                          /* '<S14>/Add' */
  real_T Gain1;                        /* '<S14>/Gain1' */
  real_T Saturation_k;                 /* '<S14>/Saturation' */
  real_T Gain2;                        /* '<S14>/Gain2' */
  real_T iq_soll_final;                /* '<Root>/iq_soll_enable_sw' */
  real_T Sum_n;                        /* '<S9>/Sum' */
  real_T Product;                      /* '<S9>/Product' */
  real_T Output;                       /* '<S9>/Output' */
  real_T iu_sw;                        /* '<S11>/iu_sw' */
  real_T iw_sw;                        /* '<S11>/iw_sw' */
  real_T Product3;                     /* '<S11>/Product3' */
  real_T Gain_i;                       /* '<S11>/Gain' */
  real_T Add1;                         /* '<S11>/Add1' */
  real_T Product2;                     /* '<S11>/Product2' */
  real_T iv_sw;                        /* '<S11>/iv_sw' */
  real_T Product1;                     /* '<S11>/Product1' */
  real_T Gain1_m;                      /* '<S11>/Gain1' */
  real_T Add_j;                        /* '<S11>/Add' */
  real_T Product_a;                    /* '<S11>/Product' */
  boolean_T SFunction_d;               /* '<S1>/S-Function' */
  boolean_T Relay;                     /* '<S10>/Relay' */
  boolean_T Relay1;                    /* '<S10>/Relay1' */
  boolean_T Relay2;                    /* '<S10>/Relay2' */
} B_Stromregelkreis_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  int_T SFunction_IWORK;               /* '<S1>/S-Function' */
  boolean_T Relay_Mode;                /* '<S10>/Relay' */
  boolean_T Relay1_Mode;               /* '<S10>/Relay1' */
  boolean_T Relay2_Mode;               /* '<S10>/Relay2' */
} DW_Stromregelkreis_T;

/* Continuous states (default storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<S109>/Integrator' */
  real_T Filter_CSTATE;                /* '<S104>/Filter' */
  real_T Integrator_CSTATE_d;          /* '<S57>/Integrator' */
  real_T Filter_CSTATE_k;              /* '<S52>/Filter' */
  real_T IntegratorLimited_CSTATE;     /* '<S14>/Integrator Limited' */
} X_Stromregelkreis_T;

/* State derivatives (default storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<S109>/Integrator' */
  real_T Filter_CSTATE;                /* '<S104>/Filter' */
  real_T Integrator_CSTATE_d;          /* '<S57>/Integrator' */
  real_T Filter_CSTATE_k;              /* '<S52>/Filter' */
  real_T IntegratorLimited_CSTATE;     /* '<S14>/Integrator Limited' */
} XDot_Stromregelkreis_T;

/* State disabled  */
typedef struct {
  boolean_T Integrator_CSTATE;         /* '<S109>/Integrator' */
  boolean_T Filter_CSTATE;             /* '<S104>/Filter' */
  boolean_T Integrator_CSTATE_d;       /* '<S57>/Integrator' */
  boolean_T Filter_CSTATE_k;           /* '<S52>/Filter' */
  boolean_T IntegratorLimited_CSTATE;  /* '<S14>/Integrator Limited' */
} XDis_Stromregelkreis_T;

/* Zero-crossing (trigger) state */
typedef struct {
  ZCSigState DS1104ENC_SET_POS_C1_Trig_ZCE;/* '<Root>/DS1104ENC_SET_POS_C1' */
} PrevZCX_Stromregelkreis_T;

#ifndef ODE3_INTG
#define ODE3_INTG

/* ODE3 Integration Data */
typedef struct {
  real_T *y;                           /* output */
  real_T *f[3];                        /* derivatives */
} ODE3_IntgData;

#endif

/* Parameters (default storage) */
struct P_Stromregelkreis_T_ {
  real_T ta;                           /* Variable: ta
                                        * Referenced by: '<Root>/Drehzahl in 1//min'
                                        */
  real_T pid_D;                        /* Mask Parameter: pid_D
                                        * Referenced by: '<S102>/Derivative Gain'
                                        */
  real_T pid_D_k;                      /* Mask Parameter: pid_D_k
                                        * Referenced by: '<S50>/Derivative Gain'
                                        */
  real_T pid_I;                        /* Mask Parameter: pid_I
                                        * Referenced by: '<S106>/Integral Gain'
                                        */
  real_T pid_I_o;                      /* Mask Parameter: pid_I_o
                                        * Referenced by: '<S54>/Integral Gain'
                                        */
  real_T pid_InitialConditionForFilter;
                                /* Mask Parameter: pid_InitialConditionForFilter
                                 * Referenced by: '<S104>/Filter'
                                 */
  real_T pid_InitialConditionForFilter_d;
                              /* Mask Parameter: pid_InitialConditionForFilter_d
                               * Referenced by: '<S52>/Filter'
                               */
  real_T pid_InitialConditionForIntegrat;
                              /* Mask Parameter: pid_InitialConditionForIntegrat
                               * Referenced by: '<S109>/Integrator'
                               */
  real_T pid_InitialConditionForIntegr_p;
                              /* Mask Parameter: pid_InitialConditionForIntegr_p
                               * Referenced by: '<S57>/Integrator'
                               */
  real_T Ramp1_InitialOutput;          /* Mask Parameter: Ramp1_InitialOutput
                                        * Referenced by: '<S9>/Constant1'
                                        */
  real_T pid_Kb;                       /* Mask Parameter: pid_Kb
                                        * Referenced by: '<S101>/Kb'
                                        */
  real_T pid_Kb_c;                     /* Mask Parameter: pid_Kb_c
                                        * Referenced by: '<S49>/Kb'
                                        */
  real_T pid_Kt;                       /* Mask Parameter: pid_Kt
                                        * Referenced by: '<S120>/Kt'
                                        */
  real_T pid_Kt_e;                     /* Mask Parameter: pid_Kt_e
                                        * Referenced by: '<S68>/Kt'
                                        */
  real_T pid_LowerIntegratorSaturationLi;
                              /* Mask Parameter: pid_LowerIntegratorSaturationLi
                               * Referenced by: '<S109>/Integrator'
                               */
  real_T pid_LowerIntegratorSaturation_b;
                              /* Mask Parameter: pid_LowerIntegratorSaturation_b
                               * Referenced by: '<S57>/Integrator'
                               */
  real_T pid_LowerSaturationLimit;   /* Mask Parameter: pid_LowerSaturationLimit
                                      * Referenced by: '<S116>/Saturation'
                                      */
  real_T pid_LowerSaturationLimit_f;
                                   /* Mask Parameter: pid_LowerSaturationLimit_f
                                    * Referenced by: '<S64>/Saturation'
                                    */
  real_T pid_N;                        /* Mask Parameter: pid_N
                                        * Referenced by: '<S112>/Filter Coefficient'
                                        */
  real_T pid_N_i;                      /* Mask Parameter: pid_N_i
                                        * Referenced by: '<S60>/Filter Coefficient'
                                        */
  real_T pid_P;                        /* Mask Parameter: pid_P
                                        * Referenced by: '<S114>/Proportional Gain'
                                        */
  real_T pid_P_p;                      /* Mask Parameter: pid_P_p
                                        * Referenced by: '<S62>/Proportional Gain'
                                        */
  real_T pid_UpperIntegratorSaturationLi;
                              /* Mask Parameter: pid_UpperIntegratorSaturationLi
                               * Referenced by: '<S109>/Integrator'
                               */
  real_T pid_UpperIntegratorSaturation_m;
                              /* Mask Parameter: pid_UpperIntegratorSaturation_m
                               * Referenced by: '<S57>/Integrator'
                               */
  real_T pid_UpperSaturationLimit;   /* Mask Parameter: pid_UpperSaturationLimit
                                      * Referenced by: '<S116>/Saturation'
                                      */
  real_T pid_UpperSaturationLimit_a;
                                   /* Mask Parameter: pid_UpperSaturationLimit_a
                                    * Referenced by: '<S64>/Saturation'
                                    */
  real_T Ramp1_slope;                  /* Mask Parameter: Ramp1_slope
                                        * Referenced by: '<S9>/Step'
                                        */
  real_T Ramp1_start;                  /* Mask Parameter: Ramp1_start
                                        * Referenced by:
                                        *   '<S9>/Constant'
                                        *   '<S9>/Step'
                                        */
  real_T Gain1_Gain;                   /* Expression: -1
                                        * Referenced by: '<S11>/Gain1'
                                        */
  real_T iv_sw_Threshold;              /* Expression: 0
                                        * Referenced by: '<S11>/iv_sw'
                                        */
  real_T Gain_Gain;                    /* Expression: -1
                                        * Referenced by: '<S11>/Gain'
                                        */
  real_T iw_sw_Threshold;              /* Expression: 0
                                        * Referenced by: '<S11>/iw_sw'
                                        */
  real_T iu_sw_Threshold;              /* Expression: 0
                                        * Referenced by: '<S11>/iu_sw'
                                        */
  real_T const_iu_Value;               /* Expression: 1
                                        * Referenced by: '<S11>/const_iu'
                                        */
  real_T thresh_sw_Value;              /* Expression: 0
                                        * Referenced by: '<S11>/thresh_sw'
                                        */
  real_T n_soll_Value;                 /* Expression: 0
                                        * Referenced by: '<Root>/n_soll'
                                        */
  real_T n_nurmierung1_Gain;           /* Expression: 1/3000
                                        * Referenced by: '<Root>/n_nurmierung1'
                                        */
  real_T n_numierung2_Gain;            /* Expression: 1/3000
                                        * Referenced by: '<Root>/n_numierung2'
                                        */
  real_T n_Regelung_Start_Value;       /* Expression: 0
                                        * Referenced by: '<Root>/n_Regelung_Start'
                                        */
  real_T iq_soll_safe_Value;           /* Expression: 0
                                        * Referenced by: '<Root>/iq_soll_safe'
                                        */
  real_T n_pi_iq_sw_Threshold;         /* Expression: 0
                                        * Referenced by: '<Root>/n_pi_iq_sw'
                                        */
  real_T Gain_Gain_d;                  /* Expression: -1
                                        * Referenced by: '<S12>/Gain'
                                        */
  real_T Offset_Value;                 /* Expression: 0.53
                                        * Referenced by: '<Root>/Offset'
                                        */
  real_T mechanischerWinkel_Gain;      /* Expression: (2*pi)/4096
                                        * Referenced by: '<Root>/mechanischer Winkel'
                                        */
  real_T elektrischerWinkel_Gain;      /* Expression: 3
                                        * Referenced by: '<Root>/elektrischer Winkel'
                                        */
  real_T Step_Y0;                      /* Expression: 0
                                        * Referenced by: '<S9>/Step'
                                        */
  real_T ONOFF_Value;                  /* Expression: 0
                                        * Referenced by: '<S10>/ON//OFF'
                                        */
  real_T Startwert_Value;              /* Expression: 0
                                        * Referenced by: '<S10>/Startwert'
                                        */
  real_T Switch_Threshold;             /* Expression: 0
                                        * Referenced by: '<S10>/Switch'
                                        */
  real_T Iu_ist_Gain;                  /* Expression: 4.46*2
                                        * Referenced by: '<S10>/Iu_ist'
                                        */
  real_T Relay_OnVal;                  /* Expression: 0.1
                                        * Referenced by: '<S10>/Relay'
                                        */
  real_T Relay_OffVal;                 /* Expression: -0.1
                                        * Referenced by: '<S10>/Relay'
                                        */
  real_T const_iv_Value;               /* Expression: -0.5
                                        * Referenced by: '<S11>/const_iv'
                                        */
  real_T Constant_Value;               /* Expression: 0.5
                                        * Referenced by: '<S11>/Constant'
                                        */
  real_T Constant1_Value;              /* Expression: -sqrt(3)
                                        * Referenced by: '<S11>/Constant1'
                                        */
  real_T Switch1_Threshold;            /* Expression: 0
                                        * Referenced by: '<S10>/Switch1'
                                        */
  real_T Iv_ist_Gain;                  /* Expression: 4.46*2
                                        * Referenced by: '<S10>/Iv_ist'
                                        */
  real_T Relay1_OnVal;                 /* Expression: 0.1
                                        * Referenced by: '<S10>/Relay1'
                                        */
  real_T Relay1_OffVal;                /* Expression: -0.1
                                        * Referenced by: '<S10>/Relay1'
                                        */
  real_T const_iw_Value;               /* Expression: -0.5
                                        * Referenced by: '<S11>/const_iw'
                                        */
  real_T Constant2_Value;              /* Expression: 0.5
                                        * Referenced by: '<S11>/Constant2'
                                        */
  real_T Constant3_Value;              /* Expression: +sqrt(3)
                                        * Referenced by: '<S11>/Constant3'
                                        */
  real_T Switch2_Threshold;            /* Expression: 0
                                        * Referenced by: '<S10>/Switch2'
                                        */
  real_T Iw_ist_Gain;                  /* Expression: 4.46*2
                                        * Referenced by: '<S10>/Iw_ist'
                                        */
  real_T Relay2_OnVal;                 /* Expression: 0.1
                                        * Referenced by: '<S10>/Relay2'
                                        */
  real_T Relay2_OffVal;                /* Expression: -0.1
                                        * Referenced by: '<S10>/Relay2'
                                        */
  real_T Gain_Gain_o;                  /* Expression: 3.5
                                        * Referenced by: '<S14>/Gain'
                                        */
  real_T IntegratorLimited_IC;         /* Expression: 0
                                        * Referenced by: '<S14>/Integrator Limited'
                                        */
  real_T IntegratorLimited_UpperSat;   /* Expression: 1
                                        * Referenced by: '<S14>/Integrator Limited'
                                        */
  real_T IntegratorLimited_LowerSat;   /* Expression: 0
                                        * Referenced by: '<S14>/Integrator Limited'
                                        */
  real_T Gain1_Gain_d;                 /* Expression: 5.35
                                        * Referenced by: '<S14>/Gain1'
                                        */
  real_T Saturation_UpperSat;          /* Expression: 0.5
                                        * Referenced by: '<S14>/Saturation'
                                        */
  real_T Saturation_LowerSat;          /* Expression: -0.5
                                        * Referenced by: '<S14>/Saturation'
                                        */
  real_T Gain2_Gain;                   /* Expression: 1
                                        * Referenced by: '<S14>/Gain2'
                                        */
  boolean_T Relay_YOn;                 /* Computed Parameter: Relay_YOn
                                        * Referenced by: '<S10>/Relay'
                                        */
  boolean_T Relay_YOff;                /* Computed Parameter: Relay_YOff
                                        * Referenced by: '<S10>/Relay'
                                        */
  boolean_T Relay1_YOn;                /* Computed Parameter: Relay1_YOn
                                        * Referenced by: '<S10>/Relay1'
                                        */
  boolean_T Relay1_YOff;               /* Computed Parameter: Relay1_YOff
                                        * Referenced by: '<S10>/Relay1'
                                        */
  boolean_T Relay2_YOn;                /* Computed Parameter: Relay2_YOn
                                        * Referenced by: '<S10>/Relay2'
                                        */
  boolean_T Relay2_YOff;               /* Computed Parameter: Relay2_YOff
                                        * Referenced by: '<S10>/Relay2'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_Stromregelkreis_T {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;
  X_Stromregelkreis_T *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  XDis_Stromregelkreis_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  real_T odeY[5];
  real_T odeF[3][5];
  ODE3_IntgData intgData;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numSampTimes;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    uint32_T clockTick1;
    uint32_T clockTickH1;
    time_T tStart;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Block parameters (default storage) */
extern P_Stromregelkreis_T Stromregelkreis_P;

/* Block signals (default storage) */
extern B_Stromregelkreis_T Stromregelkreis_B;

/* Continuous states (default storage) */
extern X_Stromregelkreis_T Stromregelkreis_X;

/* Disabled states (default storage) */
extern XDis_Stromregelkreis_T Stromregelkreis_XDis;

/* Block states (default storage) */
extern DW_Stromregelkreis_T Stromregelkreis_DW;

/* Zero-crossing (trigger) state */
extern PrevZCX_Stromregelkreis_T Stromregelkreis_PrevZCX;

/* Model entry point functions */
extern void Stromregelkreis_initialize(void);
extern void Stromregelkreis_output(void);
extern void Stromregelkreis_update(void);
extern void Stromregelkreis_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Stromregelkreis_T *const Stromregelkreis_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'Stromregelkreis'
 * '<S1>'   : 'Stromregelkreis/DS1104ENC_HW_INDEX_C1'
 * '<S2>'   : 'Stromregelkreis/DS1104ENC_POS_C1'
 * '<S3>'   : 'Stromregelkreis/DS1104ENC_SETUP'
 * '<S4>'   : 'Stromregelkreis/DS1104ENC_SET_POS_C1'
 * '<S5>'   : 'Stromregelkreis/DS1104SL_DSP_BIT_OUT_C3'
 * '<S6>'   : 'Stromregelkreis/DS1104SL_DSP_BIT_OUT_C4'
 * '<S7>'   : 'Stromregelkreis/DS1104SL_DSP_BIT_OUT_C5'
 * '<S8>'   : 'Stromregelkreis/RTI Data'
 * '<S9>'   : 'Stromregelkreis/Ramp1'
 * '<S10>'  : 'Stromregelkreis/Stromregelung'
 * '<S11>'  : 'Stromregelkreis/ab > 1_2_3'
 * '<S12>'  : 'Stromregelkreis/dq>ab'
 * '<S13>'  : 'Stromregelkreis/drehzahlregler'
 * '<S14>'  : 'Stromregelkreis/n_Regler'
 * '<S15>'  : 'Stromregelkreis/pid'
 * '<S16>'  : 'Stromregelkreis/RTI Data/RTI Data Store'
 * '<S17>'  : 'Stromregelkreis/RTI Data/RTI Data Store/RTI Data Store'
 * '<S18>'  : 'Stromregelkreis/RTI Data/RTI Data Store/RTI Data Store/RTI Data Store'
 * '<S19>'  : 'Stromregelkreis/Stromregelung/DS1104ADC_C5'
 * '<S20>'  : 'Stromregelkreis/Stromregelung/DS1104ADC_C6'
 * '<S21>'  : 'Stromregelkreis/Stromregelung/DS1104ADC_C7'
 * '<S22>'  : 'Stromregelkreis/drehzahlregler/pid'
 * '<S23>'  : 'Stromregelkreis/drehzahlregler/pid/Anti-windup'
 * '<S24>'  : 'Stromregelkreis/drehzahlregler/pid/D Gain'
 * '<S25>'  : 'Stromregelkreis/drehzahlregler/pid/External Derivative'
 * '<S26>'  : 'Stromregelkreis/drehzahlregler/pid/Filter'
 * '<S27>'  : 'Stromregelkreis/drehzahlregler/pid/Filter ICs'
 * '<S28>'  : 'Stromregelkreis/drehzahlregler/pid/I Gain'
 * '<S29>'  : 'Stromregelkreis/drehzahlregler/pid/Ideal P Gain'
 * '<S30>'  : 'Stromregelkreis/drehzahlregler/pid/Ideal P Gain Fdbk'
 * '<S31>'  : 'Stromregelkreis/drehzahlregler/pid/Integrator'
 * '<S32>'  : 'Stromregelkreis/drehzahlregler/pid/Integrator ICs'
 * '<S33>'  : 'Stromregelkreis/drehzahlregler/pid/N Copy'
 * '<S34>'  : 'Stromregelkreis/drehzahlregler/pid/N Gain'
 * '<S35>'  : 'Stromregelkreis/drehzahlregler/pid/P Copy'
 * '<S36>'  : 'Stromregelkreis/drehzahlregler/pid/Parallel P Gain'
 * '<S37>'  : 'Stromregelkreis/drehzahlregler/pid/Reset Signal'
 * '<S38>'  : 'Stromregelkreis/drehzahlregler/pid/Saturation'
 * '<S39>'  : 'Stromregelkreis/drehzahlregler/pid/Saturation Fdbk'
 * '<S40>'  : 'Stromregelkreis/drehzahlregler/pid/Sum'
 * '<S41>'  : 'Stromregelkreis/drehzahlregler/pid/Sum Fdbk'
 * '<S42>'  : 'Stromregelkreis/drehzahlregler/pid/Tracking Mode'
 * '<S43>'  : 'Stromregelkreis/drehzahlregler/pid/Tracking Mode Sum'
 * '<S44>'  : 'Stromregelkreis/drehzahlregler/pid/Tsamp - Integral'
 * '<S45>'  : 'Stromregelkreis/drehzahlregler/pid/Tsamp - Ngain'
 * '<S46>'  : 'Stromregelkreis/drehzahlregler/pid/postSat Signal'
 * '<S47>'  : 'Stromregelkreis/drehzahlregler/pid/preInt Signal'
 * '<S48>'  : 'Stromregelkreis/drehzahlregler/pid/preSat Signal'
 * '<S49>'  : 'Stromregelkreis/drehzahlregler/pid/Anti-windup/Back Calculation'
 * '<S50>'  : 'Stromregelkreis/drehzahlregler/pid/D Gain/Internal Parameters'
 * '<S51>'  : 'Stromregelkreis/drehzahlregler/pid/External Derivative/Error'
 * '<S52>'  : 'Stromregelkreis/drehzahlregler/pid/Filter/Cont. Filter'
 * '<S53>'  : 'Stromregelkreis/drehzahlregler/pid/Filter ICs/Internal IC - Filter'
 * '<S54>'  : 'Stromregelkreis/drehzahlregler/pid/I Gain/Internal Parameters'
 * '<S55>'  : 'Stromregelkreis/drehzahlregler/pid/Ideal P Gain/Passthrough'
 * '<S56>'  : 'Stromregelkreis/drehzahlregler/pid/Ideal P Gain Fdbk/Disabled'
 * '<S57>'  : 'Stromregelkreis/drehzahlregler/pid/Integrator/Continuous'
 * '<S58>'  : 'Stromregelkreis/drehzahlregler/pid/Integrator ICs/Internal IC'
 * '<S59>'  : 'Stromregelkreis/drehzahlregler/pid/N Copy/Disabled'
 * '<S60>'  : 'Stromregelkreis/drehzahlregler/pid/N Gain/Internal Parameters'
 * '<S61>'  : 'Stromregelkreis/drehzahlregler/pid/P Copy/Disabled'
 * '<S62>'  : 'Stromregelkreis/drehzahlregler/pid/Parallel P Gain/Internal Parameters'
 * '<S63>'  : 'Stromregelkreis/drehzahlregler/pid/Reset Signal/Disabled'
 * '<S64>'  : 'Stromregelkreis/drehzahlregler/pid/Saturation/Enabled'
 * '<S65>'  : 'Stromregelkreis/drehzahlregler/pid/Saturation Fdbk/Disabled'
 * '<S66>'  : 'Stromregelkreis/drehzahlregler/pid/Sum/Sum_PID'
 * '<S67>'  : 'Stromregelkreis/drehzahlregler/pid/Sum Fdbk/Disabled'
 * '<S68>'  : 'Stromregelkreis/drehzahlregler/pid/Tracking Mode/Enabled'
 * '<S69>'  : 'Stromregelkreis/drehzahlregler/pid/Tracking Mode Sum/Tracking Mode'
 * '<S70>'  : 'Stromregelkreis/drehzahlregler/pid/Tsamp - Integral/TsSignalSpecification'
 * '<S71>'  : 'Stromregelkreis/drehzahlregler/pid/Tsamp - Ngain/Passthrough'
 * '<S72>'  : 'Stromregelkreis/drehzahlregler/pid/postSat Signal/Forward_Path'
 * '<S73>'  : 'Stromregelkreis/drehzahlregler/pid/preInt Signal/Internal PreInt'
 * '<S74>'  : 'Stromregelkreis/drehzahlregler/pid/preSat Signal/Forward_Path'
 * '<S75>'  : 'Stromregelkreis/pid/Anti-windup'
 * '<S76>'  : 'Stromregelkreis/pid/D Gain'
 * '<S77>'  : 'Stromregelkreis/pid/External Derivative'
 * '<S78>'  : 'Stromregelkreis/pid/Filter'
 * '<S79>'  : 'Stromregelkreis/pid/Filter ICs'
 * '<S80>'  : 'Stromregelkreis/pid/I Gain'
 * '<S81>'  : 'Stromregelkreis/pid/Ideal P Gain'
 * '<S82>'  : 'Stromregelkreis/pid/Ideal P Gain Fdbk'
 * '<S83>'  : 'Stromregelkreis/pid/Integrator'
 * '<S84>'  : 'Stromregelkreis/pid/Integrator ICs'
 * '<S85>'  : 'Stromregelkreis/pid/N Copy'
 * '<S86>'  : 'Stromregelkreis/pid/N Gain'
 * '<S87>'  : 'Stromregelkreis/pid/P Copy'
 * '<S88>'  : 'Stromregelkreis/pid/Parallel P Gain'
 * '<S89>'  : 'Stromregelkreis/pid/Reset Signal'
 * '<S90>'  : 'Stromregelkreis/pid/Saturation'
 * '<S91>'  : 'Stromregelkreis/pid/Saturation Fdbk'
 * '<S92>'  : 'Stromregelkreis/pid/Sum'
 * '<S93>'  : 'Stromregelkreis/pid/Sum Fdbk'
 * '<S94>'  : 'Stromregelkreis/pid/Tracking Mode'
 * '<S95>'  : 'Stromregelkreis/pid/Tracking Mode Sum'
 * '<S96>'  : 'Stromregelkreis/pid/Tsamp - Integral'
 * '<S97>'  : 'Stromregelkreis/pid/Tsamp - Ngain'
 * '<S98>'  : 'Stromregelkreis/pid/postSat Signal'
 * '<S99>'  : 'Stromregelkreis/pid/preInt Signal'
 * '<S100>' : 'Stromregelkreis/pid/preSat Signal'
 * '<S101>' : 'Stromregelkreis/pid/Anti-windup/Back Calculation'
 * '<S102>' : 'Stromregelkreis/pid/D Gain/Internal Parameters'
 * '<S103>' : 'Stromregelkreis/pid/External Derivative/Error'
 * '<S104>' : 'Stromregelkreis/pid/Filter/Cont. Filter'
 * '<S105>' : 'Stromregelkreis/pid/Filter ICs/Internal IC - Filter'
 * '<S106>' : 'Stromregelkreis/pid/I Gain/Internal Parameters'
 * '<S107>' : 'Stromregelkreis/pid/Ideal P Gain/Passthrough'
 * '<S108>' : 'Stromregelkreis/pid/Ideal P Gain Fdbk/Disabled'
 * '<S109>' : 'Stromregelkreis/pid/Integrator/Continuous'
 * '<S110>' : 'Stromregelkreis/pid/Integrator ICs/Internal IC'
 * '<S111>' : 'Stromregelkreis/pid/N Copy/Disabled'
 * '<S112>' : 'Stromregelkreis/pid/N Gain/Internal Parameters'
 * '<S113>' : 'Stromregelkreis/pid/P Copy/Disabled'
 * '<S114>' : 'Stromregelkreis/pid/Parallel P Gain/Internal Parameters'
 * '<S115>' : 'Stromregelkreis/pid/Reset Signal/Disabled'
 * '<S116>' : 'Stromregelkreis/pid/Saturation/Enabled'
 * '<S117>' : 'Stromregelkreis/pid/Saturation Fdbk/Disabled'
 * '<S118>' : 'Stromregelkreis/pid/Sum/Sum_PID'
 * '<S119>' : 'Stromregelkreis/pid/Sum Fdbk/Disabled'
 * '<S120>' : 'Stromregelkreis/pid/Tracking Mode/Enabled'
 * '<S121>' : 'Stromregelkreis/pid/Tracking Mode Sum/Tracking Mode'
 * '<S122>' : 'Stromregelkreis/pid/Tsamp - Integral/TsSignalSpecification'
 * '<S123>' : 'Stromregelkreis/pid/Tsamp - Ngain/Passthrough'
 * '<S124>' : 'Stromregelkreis/pid/postSat Signal/Forward_Path'
 * '<S125>' : 'Stromregelkreis/pid/preInt Signal/Internal PreInt'
 * '<S126>' : 'Stromregelkreis/pid/preSat Signal/Forward_Path'
 */
#endif                                 /* Stromregelkreis_h_ */
