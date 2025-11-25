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
Udc = 10;  % DC-Link Spannung in Volt

%% dq-Transformationsparameter (NEU - Aufgabe 4.2)
iq_soll = 1.0;      % Drehmoment-Sollstrom in A (entspricht Drehmoment)
f_elektrisch = 10;  % Elektrische Frequenz in Hz (Drehfeld-Frequenz)
omega = 2*pi*f_elektrisch;  % Winkelgeschwindigkeit in rad/s

%% ========================================================================
%% dq → αβ → abc TRANSFORMATION (Aufgabe 4.2)
%% ========================================================================
% Statt fester Sollströme erzeugen wir sie aus iq und Winkel γ
% Dies simuliert feldorientierte Regelung (FOC)

% Winkel γ (Rampe für rotierendes Drehfeld)
add_block('simulink/Sources/Ramp', [modelName '/Gamma']);
set_param([modelName '/Gamma'], 'Slope', num2str(omega));
set_param([modelName '/Gamma'], 'InitialOutput', '0');
set_param([modelName '/Gamma'], 'Position', [50, 50, 80, 80]);

% iq Sollstrom (Drehmoment-Vorgabe)
add_block('simulink/Sources/Constant', [modelName '/iq_soll']);
set_param([modelName '/iq_soll'], 'Value', num2str(iq_soll));
set_param([modelName '/iq_soll'], 'Position', [50, 100, 80, 130]);

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

% Multiplikation: iα = -iq·sin(γ)
add_block('simulink/Math Operations/Product', [modelName '/prod_alpha']);
set_param([modelName '/prod_alpha'], 'Inputs', '2');
set_param([modelName '/prod_alpha'], 'Position', [240, 50, 270, 80]);

% Multiplikation: iβ = iq·cos(γ)
add_block('simulink/Math Operations/Product', [modelName '/prod_beta']);
set_param([modelName '/prod_beta'], 'Inputs', '2');
set_param([modelName '/prod_beta'], 'Position', [240, 100, 270, 130]);

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

% Verbindungen für dq-Transformation
add_line(modelName, 'Gamma/1', 'sin_gamma/1');
add_line(modelName, 'Gamma/1', 'cos_gamma/1');
add_line(modelName, 'sin_gamma/1', 'neg_sin/1');
add_line(modelName, 'neg_sin/1', 'prod_alpha/1');
add_line(modelName, 'iq_soll/1', 'prod_alpha/2');
add_line(modelName, 'iq_soll/1', 'prod_beta/1');
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
set_param([modelName '/Relay_U'], 'OnSwitchValue', '0.01');
set_param([modelName '/Relay_U'], 'OffSwitchValue', '-0.01');
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
set_param([modelName '/Relay_V'], 'OnSwitchValue', '0.01');
set_param([modelName '/Relay_V'], 'OffSwitchValue', '-0.01');
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
set_param([modelName '/Relay_W'], 'OnSwitchValue', '0.01');
set_param([modelName '/Relay_W'], 'OffSwitchValue', '-0.01');
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


%% ========================================================================
%% SIMULATIONSEINSTELLUNGEN
%% ========================================================================

set_param(modelName, 'Solver', 'ode45');
set_param(modelName, 'StopTime', '0.5');  % 0.5 Sekunden (mehrere Perioden bei 10 Hz)
set_param(modelName, 'MaxStep', '1e-4');

save_system(modelName);
open_system(modelName);

disp('═══════════════════════════════════════════════════════════════');
disp('  Simulink-Modell "psm_stromregelung.slx" erfolgreich erstellt!');
disp('═══════════════════════════════════════════════════════════════');
disp(' ');
disp('NEU: dq-Transformation implementiert (Aufgabe 4.2)! ✓');
disp(' ');
disp('MODELL-STRUKTUR:');
disp('  1. dq → αβ → abc Transformation (FOC-Sollströme)');
disp('  2. Drei-Phasen Stromregler mit Hystere-Relay');
disp('  3. RL-Motormodell (geschlossener Regelkreis)');
disp(' ');
disp('PARAMETER:');
fprintf('  • iq_soll = %.2f A (Drehmoment-Sollstrom)\n', iq_soll);
fprintf('  • f_elektrisch = %.1f Hz (Drehfeld-Frequenz)\n', f_elektrisch);
fprintf('  • R = %.2f Ω, L = %.4f H, Udc = %.1f V\n', R, L, Udc);
disp(' ');
disp('SCOPES:');
disp('  • Scope_Soll: Zeigt die transformierten Sollströme iU/iV/iW');
disp('               → Sinusförmig, 120° phasenverschoben!');
disp('  • Scope_Gates: Zeigt die Gate-Spannungen (±10V)');
disp('  • Scope_Currents: Zeigt die gemessenen Ströme');
disp('               → Sollte den Sollströmen folgen (mit Hysterese)');
disp(' ');
disp('ERWARTETES VERHALTEN:');
disp('  → Die Ströme sollten sinusförmig um 120° phasenverschoben sein');
disp('  → Ein rotierendes Drehfeld wird simuliert!');
disp('  → Vergleiche Scope_Soll mit Scope_Currents');
disp(' ');




