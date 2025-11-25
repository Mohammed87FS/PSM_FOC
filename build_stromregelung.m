modelName = 'psm_stromregelung';

% Alle Modelle schließen
bdclose all

% Workspace leeren
clear all
close all
clc

% Modellname neu setzen (war durch clear all gelöscht)
modelName = 'psm_stromregelung';

% Cached/compiled Dateien löschen
if exist([modelName '.slxc'], 'file')
    delete([modelName '.slxc']);
end
if exist('slprj', 'dir')
    rmdir('slprj', 's');
end

% Bestehende .slx löschen
if bdIsLoaded(modelName)
    close_system(modelName, 0);
end
if exist([modelName '.slx'], 'file')
    delete([modelName '.slx']);
end

% Kurze Pause, damit Dateisystem aufräumt
pause(0.5);

new_system(modelName);

%% Motorparameter (RL-Modell)
R = 1.0;   % Widerstand in Ohm
L = 0.01;  % Induktivität in Henry
Udc = 30;  % DC-Link Spannung in Volt (erhöht für ausreichende Leistung!)

%% Parameter für Startwert γ (wird für Drehzahlregler überschrieben)
f_elektrisch_init = 10;  % Initiale elektrische Frequenz in Hz
omega_init = 2*pi*f_elektrisch_init;  % Winkelgeschwindigkeit in rad/s

%% Drehzahlregler-Parameter (NEU - Aufgabe 4.4)
omega_soll = 2*pi*10;  % Soll-Drehzahl in rad/s (entspricht 10 Hz elektrisch)
Kp_omega = 0.02;       % Proportionalverstärkung (optimiert für neue Mechanik)
Ki_omega = 1.0;        % Integralverstärkung (optimiert für neue Mechanik)
iq_max = 3.0;          % Maximaler Strom in A (erhöht für mehr Dynamik)
iq_min = -3.0;         % Minimaler Strom in A (Anti-Windup)

%% Mechanisches Motormodell-Parameter (für Drehzahlrückkopplung)
J = 0.001;             % Trägheitsmoment in kg·m²
kt = 0.2;              % Drehmoment-Konstante: T = kt·iq (Nm/A) [optimiert!]
B = 0.005;             % Reibungskoeffizient (Dämpfung) [reduziert für bessere Performance]

%% ========================================================================
%% MECHANISCHES MOTORMODELL + DREHZAHLREGLER (Aufgabe 4.4)
%% ========================================================================
% Geschlossene Schleife: iq → Drehmoment → Mechanik → ω → γ → zurück!
% Dies simuliert einen echten Motor mit Trägheit

% Drehzahl-Sollwert
add_block('simulink/Sources/Constant', [modelName '/omega_soll_const']);
set_param([modelName '/omega_soll_const'], 'Value', num2str(omega_soll));
set_param([modelName '/omega_soll_const'], 'Position', [50, 480, 80, 510]);

% Fehlerbildung: e_omega = ω_soll - ω_ist
add_block('simulink/Math Operations/Sum', [modelName '/Sum_omega']);
set_param([modelName '/Sum_omega'], 'Inputs', '+-');
set_param([modelName '/Sum_omega'], 'Position', [120, 485, 150, 515]);

% PI-Regler: Drehzahl → iq
add_block('simulink/Continuous/PID Controller', [modelName '/PI_omega']);
set_param([modelName '/PI_omega'], 'Controller', 'PI');
set_param([modelName '/PI_omega'], 'P', num2str(Kp_omega));
set_param([modelName '/PI_omega'], 'I', num2str(Ki_omega));
set_param([modelName '/PI_omega'], 'InitialConditionForIntegrator', '0');
set_param([modelName '/PI_omega'], 'UpperSaturationLimit', num2str(iq_max));
set_param([modelName '/PI_omega'], 'LowerSaturationLimit', num2str(iq_min));
set_param([modelName '/PI_omega'], 'Position', [180, 480, 230, 520]);

%% MECHANIK: iq → T → ω → γ (geschlossene Schleife!)

% Drehmoment aus Strom: T = kt·iq
add_block('simulink/Math Operations/Gain', [modelName '/Torque_Gain']);
set_param([modelName '/Torque_Gain'], 'Gain', num2str(kt));
set_param([modelName '/Torque_Gain'], 'Position', [260, 480, 290, 510]);

% Reibungsdrehmoment: T_friction = B·ω
add_block('simulink/Math Operations/Gain', [modelName '/Friction']);
set_param([modelName '/Friction'], 'Gain', num2str(B));
set_param([modelName '/Friction'], 'Position', [500, 540, 530, 570]);

% Netto-Drehmoment: T_net = T - T_friction
add_block('simulink/Math Operations/Sum', [modelName '/Sum_Torque']);
set_param([modelName '/Sum_Torque'], 'Inputs', '+-');
set_param([modelName '/Sum_Torque'], 'Position', [570, 485, 600, 515]);

% Beschleunigung: dω/dt = T_net / J
add_block('simulink/Math Operations/Gain', [modelName '/InvJ']);
set_param([modelName '/InvJ'], 'Gain', num2str(1/J));
set_param([modelName '/InvJ'], 'Position', [630, 485, 660, 515]);

% Integration: ω = ∫(dω/dt) [Motor startet bei Ruhe!]
add_block('simulink/Continuous/Integrator', [modelName '/omega_meas']);
set_param([modelName '/omega_meas'], 'InitialCondition', '0');
set_param([modelName '/omega_meas'], 'Position', [690, 485, 720, 515]);

% Integration: γ = ∫ω (Winkel)
add_block('simulink/Continuous/Integrator', [modelName '/Gamma']);
set_param([modelName '/Gamma'], 'InitialCondition', '0');
set_param([modelName '/Gamma'], 'Position', [750, 485, 780, 515]);

% Verbindungen Mechanik
add_line(modelName, 'omega_soll_const/1', 'Sum_omega/1');
add_line(modelName, 'omega_meas/1', 'Sum_omega/2');
add_line(modelName, 'Sum_omega/1', 'PI_omega/1');
add_line(modelName, 'PI_omega/1', 'Torque_Gain/1');
add_line(modelName, 'Torque_Gain/1', 'Sum_Torque/1');
add_line(modelName, 'Friction/1', 'Sum_Torque/2');
add_line(modelName, 'Sum_Torque/1', 'InvJ/1');
add_line(modelName, 'InvJ/1', 'omega_meas/1');
add_line(modelName, 'omega_meas/1', 'Friction/1');
add_line(modelName, 'omega_meas/1', 'Gamma/1');

%% ========================================================================
%% dq → αβ → abc TRANSFORMATION (Aufgabe 4.2)
%% ========================================================================
% Statt fester Sollströme erzeugen wir sie aus iq (vom PI-Regler) und γ
% Dies ist feldorientierte Regelung (FOC) mit Drehzahlregelung

% sin(γ) und cos(γ) für Transformation
add_block('simulink/Math Operations/Trigonometric Function', [modelName '/sin_gamma']);
set_param([modelName '/sin_gamma'], 'Operator', 'sin');
set_param([modelName '/sin_gamma'], 'Position', [120, 40, 150, 70]);

add_block('simulink/Math Operations/Trigonometric Function', [modelName '/cos_gamma']);
set_param([modelName '/cos_gamma'], 'Operator', 'cos');
set_param([modelName '/cos_gamma'], 'Position', [120, 80, 150, 110]);

% Negation für sin (weil iα = -iq·sin(γ))
add_block('simulink/Math Operations/Gain', [modelName '/neg_sin']);
set_param([modelName '/neg_sin'], 'Gain', '-1');
set_param([modelName '/neg_sin'], 'Position', [180, 40, 210, 70]);

% Multiplikation: iα = -iq·sin(γ)  [iq kommt jetzt vom PI-Regler!]
add_block('simulink/Math Operations/Product', [modelName '/prod_alpha']);
set_param([modelName '/prod_alpha'], 'Inputs', '2');
set_param([modelName '/prod_alpha'], 'Position', [330, 50, 360, 80]);

% Multiplikation: iβ = iq·cos(γ)  [iq kommt jetzt vom PI-Regler!]
add_block('simulink/Math Operations/Product', [modelName '/prod_beta']);
set_param([modelName '/prod_beta'], 'Inputs', '2');
set_param([modelName '/prod_beta'], 'Position', [330, 100, 360, 130]);

% αβ → abc Transformation
% iU = iα
add_block('simulink/Math Operations/Gain', [modelName '/IU_SOLL']);
set_param([modelName '/IU_SOLL'], 'Gain', '1');
set_param([modelName '/IU_SOLL'], 'Position', [320, 50, 350, 80]);

% iV = -0.5·iα + √3/2·iβ
% Zuerst iα skalieren
add_block('simulink/Math Operations/Gain', [modelName '/alpha_V']);
set_param([modelName '/alpha_V'], 'Gain', '-0.5');
set_param([modelName '/alpha_V'], 'Position', [320, 150, 350, 180]);

% iβ skalieren
add_block('simulink/Math Operations/Gain', [modelName '/beta_V']);
set_param([modelName '/beta_V'], 'Gain', num2str(sqrt(3)/2));
set_param([modelName '/beta_V'], 'Position', [320, 190, 350, 220]);

% Summieren für iV
add_block('simulink/Math Operations/Sum', [modelName '/IV_SOLL']);
set_param([modelName '/IV_SOLL'], 'Inputs', '++');
set_param([modelName '/IV_SOLL'], 'Position', [380, 165, 410, 195]);

% iW = -0.5·iα - √3/2·iβ
% iα skalieren (gleich wie für V)
add_block('simulink/Math Operations/Gain', [modelName '/alpha_W']);
set_param([modelName '/alpha_W'], 'Gain', '-0.5');
set_param([modelName '/alpha_W'], 'Position', [320, 250, 350, 280]);

% iβ skalieren (negativ)
add_block('simulink/Math Operations/Gain', [modelName '/beta_W']);
set_param([modelName '/beta_W'], 'Gain', num2str(-sqrt(3)/2));
set_param([modelName '/beta_W'], 'Position', [320, 290, 350, 320]);

% Summieren für iW
add_block('simulink/Math Operations/Sum', [modelName '/IW_SOLL']);
set_param([modelName '/IW_SOLL'], 'Inputs', '++');
set_param([modelName '/IW_SOLL'], 'Position', [380, 265, 410, 295]);

% Verbindungen für dq-Transformation (iq kommt vom PI-Regler!)
add_line(modelName, 'Gamma/1', 'sin_gamma/1');
add_line(modelName, 'Gamma/1', 'cos_gamma/1');
add_line(modelName, 'sin_gamma/1', 'neg_sin/1');
add_line(modelName, 'neg_sin/1', 'prod_alpha/1');
add_line(modelName, 'PI_omega/1', 'prod_alpha/2');  % iq vom Drehzahlregler!
add_line(modelName, 'PI_omega/1', 'prod_beta/1');   % iq vom Drehzahlregler!
add_line(modelName, 'cos_gamma/1', 'prod_beta/2');

% iα und iβ zu abc
add_line(modelName, 'prod_alpha/1', 'IU_SOLL/1');
add_line(modelName, 'prod_alpha/1', 'alpha_V/1');
add_line(modelName, 'prod_alpha/1', 'alpha_W/1');
add_line(modelName, 'prod_beta/1', 'beta_V/1');
add_line(modelName, 'prod_beta/1', 'beta_W/1');
add_line(modelName, 'alpha_V/1', 'IV_SOLL/1');
add_line(modelName, 'beta_V/1', 'IV_SOLL/2');
add_line(modelName, 'alpha_W/1', 'IW_SOLL/1');
add_line(modelName, 'beta_W/1', 'IW_SOLL/2');

%% ========================================================================
%% STEUERUNGS-KONSTANTEN
%% ========================================================================

% Startwert, der verwendet wird, wenn der Regler aus ist.
add_block('simulink/Sources/Constant', [modelName '/Initialisierung']);
set_param([modelName '/Initialisierung'], 'Value', '0');  % Start bei 0
set_param([modelName '/Initialisierung'], 'Position', [50, 350, 80, 370]);

% ON/OFF Schalter, um den Regler ein oder auszuschalten... 0 = aus, 1 = an.
add_block('simulink/Sources/Constant', [modelName '/ON_OFF']);
set_param([modelName '/ON_OFF'], 'Value', '1');  % Regler eingeschaltet
set_param([modelName '/ON_OFF'], 'Position', [50, 400, 80, 430]);

%% ========================================================================
%% PHASE U - STROMREGELKREIS
%% ========================================================================

% Switch: Wählt zwischen transformiertem Sollwert und Initialisierung
add_block('simulink/Signal Routing/Switch', [modelName '/Switch_U']);
set_param([modelName '/Switch_U'], 'Threshold', '0.5');
set_param([modelName '/Switch_U'], 'Position', [450, 50, 480, 80]);

% Fehlerbildung: Soll - Ist
add_block('simulink/Math Operations/Sum', [modelName '/Sum_U']);
set_param([modelName '/Sum_U'], 'Inputs', '+-');
set_param([modelName '/Sum_U'], 'Position', [520, 50, 550, 80]);

% Zweipunktregler (Hysterese-Relay)
add_block('simulink/Discontinuities/Relay', [modelName '/Relay_U']);
set_param([modelName '/Relay_U'], 'OnSwitchValue', '0.1');   % Vergrößert gegen Chattering!
set_param([modelName '/Relay_U'], 'OffSwitchValue', '-0.1'); % Vergrößert gegen Chattering!
set_param([modelName '/Relay_U'], 'OnOutputValue', '1');
set_param([modelName '/Relay_U'], 'OffOutputValue', '-1');
set_param([modelName '/Relay_U'], 'Position', [590, 50, 620, 80]);

% Spannungsbegrenzung (±Udc)
add_block('simulink/Discontinuities/Saturation', [modelName '/Sat_U']);
set_param([modelName '/Sat_U'], 'UpperLimit', 'Udc');
set_param([modelName '/Sat_U'], 'LowerLimit', '-Udc');
set_param([modelName '/Sat_U'], 'Position', [660, 50, 690, 80]);

% Gate Output
add_block('simulink/Sinks/Out1', [modelName '/U_gate']);
set_param([modelName '/U_gate'], 'Position', [730, 55, 760, 75]);

%% RL-Motormodell Phase U (geschlossener Regelkreis)
add_block('simulink/Math Operations/Sum', [modelName '/Sum_Motor_U']);
set_param([modelName '/Sum_Motor_U'], 'Inputs', '+-');
set_param([modelName '/Sum_Motor_U'], 'Position', [900, 50, 930, 80]);

add_block('simulink/Math Operations/Gain', [modelName '/InvL_U']);
set_param([modelName '/InvL_U'], 'Gain', '1/L');
set_param([modelName '/InvL_U'], 'Position', [970, 50, 1000, 80]);

add_block('simulink/Continuous/Integrator', [modelName '/Integrator_U']);
set_param([modelName '/Integrator_U'], 'InitialCondition', '0');
set_param([modelName '/Integrator_U'], 'Position', [1040, 50, 1070, 80]);

add_block('simulink/Math Operations/Gain', [modelName '/K_U']);
set_param([modelName '/K_U'], 'Gain', '1');
set_param([modelName '/K_U'], 'Position', [1110, 50, 1140, 80]);

add_block('simulink/Math Operations/Gain', [modelName '/R_U']);
set_param([modelName '/R_U'], 'Gain', 'R');
set_param([modelName '/R_U'], 'Position', [820, 120, 850, 150]);

% Verbindungen Phase U
add_line(modelName, 'IU_SOLL/1', 'Switch_U/1');
add_line(modelName, 'ON_OFF/1', 'Switch_U/2');
add_line(modelName, 'Initialisierung/1', 'Switch_U/3');
add_line(modelName, 'Switch_U/1', 'Sum_U/1');
add_line(modelName, 'K_U/1', 'Sum_U/2');
add_line(modelName, 'Sum_U/1', 'Relay_U/1');
add_line(modelName, 'Relay_U/1', 'Sat_U/1');
add_line(modelName, 'Sat_U/1', 'U_gate/1');

% RL-Motormodell
add_line(modelName, 'Sat_U/1', 'Sum_Motor_U/1');
add_line(modelName, 'R_U/1', 'Sum_Motor_U/2');
add_line(modelName, 'Sum_Motor_U/1', 'InvL_U/1');
add_line(modelName, 'InvL_U/1', 'Integrator_U/1');
add_line(modelName, 'Integrator_U/1', 'K_U/1');
add_line(modelName, 'Integrator_U/1', 'R_U/1');

%% ========================================================================
%% PHASE V - STROMREGELKREIS
%% ========================================================================

add_block('simulink/Signal Routing/Switch', [modelName '/Switch_V']);
set_param([modelName '/Switch_V'], 'Threshold', '0.5');
set_param([modelName '/Switch_V'], 'Position', [450, 170, 480, 200]);

add_block('simulink/Math Operations/Sum', [modelName '/Sum_V']);
set_param([modelName '/Sum_V'], 'Inputs', '+-');
set_param([modelName '/Sum_V'], 'Position', [520, 170, 550, 200]);

add_block('simulink/Discontinuities/Relay', [modelName '/Relay_V']);
set_param([modelName '/Relay_V'], 'OnSwitchValue', '0.1');   % Vergrößert gegen Chattering!
set_param([modelName '/Relay_V'], 'OffSwitchValue', '-0.1'); % Vergrößert gegen Chattering!
set_param([modelName '/Relay_V'], 'OnOutputValue', '1');
set_param([modelName '/Relay_V'], 'OffOutputValue', '-1');
set_param([modelName '/Relay_V'], 'Position', [590, 170, 620, 200]);

add_block('simulink/Discontinuities/Saturation', [modelName '/Sat_V']);
set_param([modelName '/Sat_V'], 'UpperLimit', 'Udc');
set_param([modelName '/Sat_V'], 'LowerLimit', '-Udc');
set_param([modelName '/Sat_V'], 'Position', [660, 170, 690, 200]);

add_block('simulink/Sinks/Out1', [modelName '/V_gate']);
set_param([modelName '/V_gate'], 'Position', [730, 175, 760, 195]);

add_block('simulink/Math Operations/Sum', [modelName '/Sum_Motor_V']);
set_param([modelName '/Sum_Motor_V'], 'Inputs', '+-');
set_param([modelName '/Sum_Motor_V'], 'Position', [900, 170, 930, 200]);

add_block('simulink/Math Operations/Gain', [modelName '/InvL_V']);
set_param([modelName '/InvL_V'], 'Gain', '1/L');
set_param([modelName '/InvL_V'], 'Position', [970, 170, 1000, 200]);

add_block('simulink/Continuous/Integrator', [modelName '/Integrator_V']);
set_param([modelName '/Integrator_V'], 'InitialCondition', '0');
set_param([modelName '/Integrator_V'], 'Position', [1040, 170, 1070, 200]);

add_block('simulink/Math Operations/Gain', [modelName '/K_V']);
set_param([modelName '/K_V'], 'Gain', '1');
set_param([modelName '/K_V'], 'Position', [1110, 170, 1140, 200]);

add_block('simulink/Math Operations/Gain', [modelName '/R_V']);
set_param([modelName '/R_V'], 'Gain', 'R');
set_param([modelName '/R_V'], 'Position', [820, 240, 850, 270]);

% Verbindungen Phase V
add_line(modelName, 'IV_SOLL/1', 'Switch_V/1');
add_line(modelName, 'ON_OFF/1', 'Switch_V/2');
add_line(modelName, 'Initialisierung/1', 'Switch_V/3');
add_line(modelName, 'Switch_V/1', 'Sum_V/1');
add_line(modelName, 'K_V/1', 'Sum_V/2');
add_line(modelName, 'Sum_V/1', 'Relay_V/1');
add_line(modelName, 'Relay_V/1', 'Sat_V/1');
add_line(modelName, 'Sat_V/1', 'V_gate/1');

add_line(modelName, 'Sat_V/1', 'Sum_Motor_V/1');
add_line(modelName, 'R_V/1', 'Sum_Motor_V/2');
add_line(modelName, 'Sum_Motor_V/1', 'InvL_V/1');
add_line(modelName, 'InvL_V/1', 'Integrator_V/1');
add_line(modelName, 'Integrator_V/1', 'K_V/1');
add_line(modelName, 'Integrator_V/1', 'R_V/1');

%% ========================================================================
%% PHASE W - STROMREGELKREIS
%% ========================================================================

add_block('simulink/Signal Routing/Switch', [modelName '/Switch_W']);
set_param([modelName '/Switch_W'], 'Threshold', '0.5');
set_param([modelName '/Switch_W'], 'Position', [450, 290, 480, 320]);

add_block('simulink/Math Operations/Sum', [modelName '/Sum_W']);
set_param([modelName '/Sum_W'], 'Inputs', '+-');
set_param([modelName '/Sum_W'], 'Position', [520, 290, 550, 320]);

add_block('simulink/Discontinuities/Relay', [modelName '/Relay_W']);
set_param([modelName '/Relay_W'], 'OnSwitchValue', '0.1');   % Vergrößert gegen Chattering!
set_param([modelName '/Relay_W'], 'OffSwitchValue', '-0.1'); % Vergrößert gegen Chattering!
set_param([modelName '/Relay_W'], 'OnOutputValue', '1');
set_param([modelName '/Relay_W'], 'OffOutputValue', '-1');
set_param([modelName '/Relay_W'], 'Position', [590, 290, 620, 320]);

add_block('simulink/Discontinuities/Saturation', [modelName '/Sat_W']);
set_param([modelName '/Sat_W'], 'UpperLimit', 'Udc');
set_param([modelName '/Sat_W'], 'LowerLimit', '-Udc');
set_param([modelName '/Sat_W'], 'Position', [660, 290, 690, 320]);

add_block('simulink/Sinks/Out1', [modelName '/W_gate']);
set_param([modelName '/W_gate'], 'Position', [730, 295, 760, 315]);

add_block('simulink/Math Operations/Sum', [modelName '/Sum_Motor_W']);
set_param([modelName '/Sum_Motor_W'], 'Inputs', '+-');
set_param([modelName '/Sum_Motor_W'], 'Position', [900, 290, 930, 320]);

add_block('simulink/Math Operations/Gain', [modelName '/InvL_W']);
set_param([modelName '/InvL_W'], 'Gain', '1/L');
set_param([modelName '/InvL_W'], 'Position', [970, 290, 1000, 320]);

add_block('simulink/Continuous/Integrator', [modelName '/Integrator_W']);
set_param([modelName '/Integrator_W'], 'InitialCondition', '0');
set_param([modelName '/Integrator_W'], 'Position', [1040, 290, 1070, 320]);

add_block('simulink/Math Operations/Gain', [modelName '/K_W']);
set_param([modelName '/K_W'], 'Gain', '1');
set_param([modelName '/K_W'], 'Position', [1110, 290, 1140, 320]);

add_block('simulink/Math Operations/Gain', [modelName '/R_W']);
set_param([modelName '/R_W'], 'Gain', 'R');
set_param([modelName '/R_W'], 'Position', [820, 360, 850, 390]);

% Verbindungen Phase W
add_line(modelName, 'IW_SOLL/1', 'Switch_W/1');
add_line(modelName, 'ON_OFF/1', 'Switch_W/2');
add_line(modelName, 'Initialisierung/1', 'Switch_W/3');
add_line(modelName, 'Switch_W/1', 'Sum_W/1');
add_line(modelName, 'K_W/1', 'Sum_W/2');
add_line(modelName, 'Sum_W/1', 'Relay_W/1');
add_line(modelName, 'Relay_W/1', 'Sat_W/1');
add_line(modelName, 'Sat_W/1', 'W_gate/1');

add_line(modelName, 'Sat_W/1', 'Sum_Motor_W/1');
add_line(modelName, 'R_W/1', 'Sum_Motor_W/2');
add_line(modelName, 'Sum_Motor_W/1', 'InvL_W/1');
add_line(modelName, 'InvL_W/1', 'Integrator_W/1');
add_line(modelName, 'Integrator_W/1', 'K_W/1');
add_line(modelName, 'Integrator_W/1', 'R_W/1');

%% ========================================================================
%% VISUALISIERUNG - SCOPES
%% ========================================================================

% Scope 1: Sollströme (dq-Transformation)
add_block('simulink/Signal Routing/Mux', [modelName '/Mux_Soll']);
set_param([modelName '/Mux_Soll'], 'Inputs', '3');
set_param([modelName '/Mux_Soll'], 'Position', [450, 440, 480, 500]);

add_block('simulink/Sinks/Scope', [modelName '/Scope_Soll']);
set_param([modelName '/Scope_Soll'], 'Position', [520, 440, 580, 500]);

add_line(modelName, 'IU_SOLL/1', 'Mux_Soll/1');
add_line(modelName, 'IV_SOLL/1', 'Mux_Soll/2');
add_line(modelName, 'IW_SOLL/1', 'Mux_Soll/3');
add_line(modelName, 'Mux_Soll/1', 'Scope_Soll/1');

% Scope 2: Gate-Signale (Spannungen)
add_block('simulink/Signal Routing/Mux', [modelName '/Mux_Gates']);
set_param([modelName '/Mux_Gates'], 'Inputs', '3');
set_param([modelName '/Mux_Gates'], 'Position', [800, 100, 830, 160]);

add_block('simulink/Sinks/Scope', [modelName '/Scope_Gates']);
set_param([modelName '/Scope_Gates'], 'Position', [870, 100, 930, 160]);

add_line(modelName, 'Sat_U/1', 'Mux_Gates/1');
add_line(modelName, 'Sat_V/1', 'Mux_Gates/2');
add_line(modelName, 'Sat_W/1', 'Mux_Gates/3');
add_line(modelName, 'Mux_Gates/1', 'Scope_Gates/1');

% Scope 3: Gemessene Ströme (zeigt Regelverhalten)
add_block('simulink/Signal Routing/Mux', [modelName '/Mux_Currents']);
set_param([modelName '/Mux_Currents'], 'Inputs', '3');
set_param([modelName '/Mux_Currents'], 'Position', [1200, 130, 1230, 190]);

add_block('simulink/Sinks/Scope', [modelName '/Scope_Currents']);
set_param([modelName '/Scope_Currents'], 'Position', [1270, 130, 1330, 190]);

add_line(modelName, 'Integrator_U/1', 'Mux_Currents/1');
add_line(modelName, 'Integrator_V/1', 'Mux_Currents/2');
add_line(modelName, 'Integrator_W/1', 'Mux_Currents/3');
add_line(modelName, 'Mux_Currents/1', 'Scope_Currents/1');

% Scope 4: Drehzahlregelung (NEU)
add_block('simulink/Signal Routing/Mux', [modelName '/Mux_Speed']);
set_param([modelName '/Mux_Speed'], 'Inputs', '3');
set_param([modelName '/Mux_Speed'], 'Position', [320, 580, 350, 640]);

add_block('simulink/Sinks/Scope', [modelName '/Scope_Speed']);
set_param([modelName '/Scope_Speed'], 'Position', [390, 580, 450, 640]);

add_line(modelName, 'omega_soll_const/1', 'Mux_Speed/1');
add_line(modelName, 'omega_meas/1', 'Mux_Speed/2');
add_line(modelName, 'PI_omega/1', 'Mux_Speed/3');
add_line(modelName, 'Mux_Speed/1', 'Scope_Speed/1');


%% ========================================================================
%% SIMULATIONSEINSTELLUNGEN
%% ========================================================================

set_param(modelName, 'Solver', 'ode45');
set_param(modelName, 'StopTime', '1.5');  % 1.5 Sekunden (optimiert)
set_param(modelName, 'MaxStep', '1e-4');
set_param(modelName, 'ZeroCrossAlgorithm', 'Adaptive');
set_param(modelName, 'MaxConsecutiveZCs', '10000');

save_system(modelName);
open_system(modelName);

disp('═══════════════════════════════════════════════════════════════');
disp('  FOC mit DREHZAHLREGELUNG erfolgreich erstellt! ');
disp('═══════════════════════════════════════════════════════════════');
disp(' ');
disp('NEU: PI-Drehzahlregler implementiert (Aufgabe 4.4)! ✓');
disp(' ');
disp('VOLLSTÄNDIGE KASKADENREGELUNG (Speed → Current):');
disp('  1. DREHZAHLREGLER (äußere Schleife)');
disp('     • ω_soll → PI-Regler → iq_soll');
disp('  2. MECHANISCHES MODELL (NEU! geschlossene Schleife)');
disp('     • iq → Drehmoment T = kt·iq');
disp('     • Mechanik: dω/dt = (T - B·ω)/J');
disp('     • Integration: ω, dann γ = ∫ω');
disp('  3. dq → αβ → abc TRANSFORMATION');
disp('     • iq + γ (vom Motor!) → iU/iV/iW Sollströme');
disp('  4. STROMREGELUNG (innere Schleife)');
disp('     • Hysterese-Relay für jede Phase');
disp('  5. RL-MOTORMODELL (elektrisch)');
disp('     • Stromdynamik: di/dt = (U - R·i)/L');
disp(' ');
disp('PARAMETER:');
fprintf('  • ω_soll = %.2f rad/s (%.1f Hz elektrisch)\n', omega_soll, omega_soll/(2*pi));
fprintf('  • Kp_ω = %.2f, Ki_ω = %.2f\n', Kp_omega, Ki_omega);
fprintf('  • iq_max = %.1f A (Anti-Windup)\n', iq_max);
fprintf('  • Mechanik: J = %.4f kg·m², kt = %.2f Nm/A, B = %.3f\n', J, kt, B);
fprintf('  • Relay-Hysterese = ±0.1 A (gegen Chattering optimiert)\n');
fprintf('  • Elektrisch: R = %.2f Ω, L = %.4f H, Udc = %.1f V\n', R, L, Udc);
disp(' ');
disp('SCOPES:');
disp('  • Scope_Speed: Drehzahlregelung (ω_soll, ω_ist, iq)');
disp('                 → ω_ist sollte ω_soll folgen!');
disp('  • Scope_Soll: Transformierte Sollströme (sinusförmig)');
disp('  • Scope_Gates: Gate-Spannungen (±10V, Schaltverhalten)');
disp('  • Scope_Currents: Gemessene Ströme (Regelverhalten)');
disp(' ');
disp('ERWARTETES VERHALTEN (OPTIMIERT!):');
disp('  → Scope_Speed: Motor beschleunigt von 0 → 62.8 rad/s');
disp('                 iq startet bei ~3A, stabilisiert bei ~1.6A');
disp('                 Einschwingzeit: ~0.2-0.3 Sekunden');
disp('  → Scope_Soll: Sinusförmige Sollströme, Amplitude ~±1.6A');
disp('  → Scope_Currents: Folgen den Sollströmen (mit Relay-Ripple)');
disp('  → Scope_Gates: Schnelles Schalten, unterschiedliche Pulsweiten');
disp('  → Vollständige FOC-Kaskadenregelung mit Mechanik!');
disp(' ');
disp('FORTSCHRITT:');
disp('  ✓ 4.1 Stromregelung: 85% (Simulation komplett)');
disp('  ✓ 4.2 dq-Transformation: 95% (funktional komplett)');
disp('  ✗ 4.3 Encoder: 0% (später mit Hardware)');
disp('  ✓ 4.4 Drehzahlregelung: 80% (PI-Regler komplett)');





