
modelName = 'psm_stromregelung';
if bdIsLoaded(modelName)
    close_system(modelName, 0);
end
if exist([modelName '.slx'], 'file')
    delete([modelName '.slx']);
end

new_system(modelName);

%% Motorparameter (RL-Modell)
R = 1.0;   % Widerstand in Ohm
L = 0.01;  % Induktivität in Henry
Udc = 10;  % DC-Link Spannung in Volt


% sollstrom Vorgabe und ON/OFF .. Konstanten
add_block('simulink/Sources/Constant', [modelName '/IU_SOLL']);  % Sollwert für Phase U
set_param([modelName '/IU_SOLL'], 'Value', '1');  % 1 Ampere 
set_param([modelName '/IU_SOLL'], 'Position', [50, 250, 80, 270]);

add_block('simulink/Sources/Constant', [modelName '/IV_SOLL']);  %  V
set_param([modelName '/IV_SOLL'], 'Value', '-0.5');  % Negativer für symmetrische Drehstrom
set_param([modelName '/IV_SOLL'], 'Position', [50, 300, 80, 320]);

add_block('simulink/Sources/Constant', [modelName '/IW_SOLL']);  %  W
set_param([modelName '/IW_SOLL'], 'Value', '-0.5');  % auch negativ
set_param([modelName '/IW_SOLL'], 'Position', [50, 350, 80, 370]);

% Startwert, der verwendet wird, wenn der Regler aus ist.
add_block('simulink/Sources/Constant', [modelName '/Initialisierung']);
set_param([modelName '/Initialisierung'], 'Value', '0');  % Start bei 0
set_param([modelName '/Initialisierung'], 'Position', [50, 400, 80, 420]);

% ON/OFF Schalter, um den Regler ein oder auszuschalten... 0 = aus, 1 = an.
add_block('simulink/Sources/Constant', [modelName '/ON_OFF']);
set_param([modelName '/ON_OFF'], 'Value', '1');  % Regler eingeschaltet
set_param([modelName '/ON_OFF'], 'Position', [50, 450, 80, 470]);


% regelkreis für Phase U auf,  vergleicht Soll und Istwert und passt an
% der Strom soll auf den Sollwert geregelt werden, um den Motor korrekt zu steuern


% der Switch wählt zwischen zwei Eingängen basierend auf einem Steuersignal
% wenn ON_OFF > 0.5, sollwert wird genommen, sonst startwert
add_block('simulink/Signal Routing/Switch', [modelName '/Switch_U']);
set_param([modelName '/Switch_U'], 'Threshold', '0.5');  % Schwellwert für Umschaltung
set_param([modelName '/Switch_U'], 'Position', [200, 50, 230, 80]);

%% RL-Motormodell für Phase U (Rückkopplung)
% Spannung von Relay -> RL-Modell -> Strom gemessen -> zurück zum Regler

% Sättigungsfunktion für Spannungsbegrenzung (±Udc)
add_block('simulink/Discontinuities/Saturation', [modelName '/Sat_U']);
set_param([modelName '/Sat_U'], 'UpperLimit', 'Udc');
set_param([modelName '/Sat_U'], 'LowerLimit', '-Udc');
set_param([modelName '/Sat_U'], 'Position', [550, 75, 580, 105]);

% Widerstandsspannungsabfall: U_R = R * I
add_block('simulink/Math Operations/Gain', [modelName '/R_U']);
set_param([modelName '/R_U'], 'Gain', 'R');
set_param([modelName '/R_U'], 'Position', [900, 150, 930, 180]);

% Spannungsdifferenz: U_L = U_in - U_R
add_block('simulink/Math Operations/Sum', [modelName '/Sum_Motor_U']);
set_param([modelName '/Sum_Motor_U'], 'Inputs', '+-');
set_param([modelName '/Sum_Motor_U'], 'Position', [1000, 75, 1030, 105]);

% Stromänderung: di/dt = U_L / L
add_block('simulink/Math Operations/Gain', [modelName '/InvL_U']);
set_param([modelName '/InvL_U'], 'Gain', '1/L');
set_param([modelName '/InvL_U'], 'Position', [1050, 75, 1080, 105]);

% Integration: I = integral(di/dt)
add_block('simulink/Continuous/Integrator', [modelName '/Integrator_U']);
set_param([modelName '/Integrator_U'], 'InitialCondition', '0');
set_param([modelName '/Integrator_U'], 'Position', [1100, 75, 1130, 105]);

% Gemessener Strom (mit Normierung)
add_block('simulink/Math Operations/Gain', [modelName '/K_U']);
set_param([modelName '/K_U'], 'Gain', '1');  % Normierungsfaktor (angepasst)
set_param([modelName '/K_U'], 'Position', [1150, 75, 1180, 105]);

% fehler berechnung .. also soll - ist
% jenachdem wie weit man vom sollwert entfernt ist, regelt man
add_block('simulink/Math Operations/Sum', [modelName '/Sum_U']);
set_param([modelName '/Sum_U'], 'Inputs', '+-');  % Plus und Minus: erster Eingang +, zweiter -
set_param([modelName '/Sum_U'], 'Position', [350, 75, 380, 105]);

% zweipunktregler (Relay)
% nur zwei Zustände: +Udc oder -Udc (PWM-ähnlich für Motorsteuerung)
add_block('simulink/Discontinuities/Relay', [modelName '/Relay_U']);
set_param([modelName '/Relay_U'], 'OnSwitchValue', '0.01');   % Schwellwert für Umschalten nach oben
set_param([modelName '/Relay_U'], 'OffSwitchValue', '-0.01'); % Schwellwert für Umschalten nach unten
set_param([modelName '/Relay_U'], 'OnOutputValue', '1');      % Ausgang wenn ON
set_param([modelName '/Relay_U'], 'OffOutputValue', '-1');    % Ausgang wenn OFF
set_param([modelName '/Relay_U'], 'Position', [450, 75, 480, 105]);


% der Ausgang des Reglers.. Gate Signal für die Phase U (steuert den Stromfluss)
add_block('simulink/Sinks/Out1', [modelName '/U_gate']);
set_param([modelName '/U_gate'], 'Position', [650, 75, 680, 105]);

% Verbindungen für Phase U (geschlossener Regelkreis)

% Regelkreis
add_line(modelName, 'IU_SOLL/1',        'Switch_U/1'); % Sollwert zu Switch
add_line(modelName, 'ON_OFF/1',         'Switch_U/2'); % ON/OFF steuert Switch
add_line(modelName, 'Initialisierung/1','Switch_U/3'); % Initialisierung zu Switch
add_line(modelName, 'Switch_U/1', 'Sum_U/1');          % Sollwert zu Sum (+)
add_line(modelName, 'K_U/1',      'Sum_U/2');          % Gemessener Strom zu Sum (-)
add_line(modelName, 'Sum_U/1',    'Relay_U/1');        % Fehler zu Relay
add_line(modelName, 'Relay_U/1',  'Sat_U/1');          % Relay zu Sättigung
add_line(modelName, 'Sat_U/1',    'U_gate/1');         % Spannung zu Gate-Ausgang

% RL-Motormodell (Rückkopplung)
add_line(modelName, 'Sat_U/1',         'Sum_Motor_U/1'); % Eingangsspannung
add_line(modelName, 'R_U/1',           'Sum_Motor_U/2'); % Widerstandsspannung (-)
add_line(modelName, 'Sum_Motor_U/1',   'InvL_U/1');      % U_L zu 1/L
add_line(modelName, 'InvL_U/1',        'Integrator_U/1'); % di/dt zu Integrator
add_line(modelName, 'Integrator_U/1',  'K_U/1');         % Strom zu Normierung
add_line(modelName, 'Integrator_U/1',  'R_U/1');         % Strom auch zu R (Feedback)

%% Phase V - identisch zu Phase U
add_block('simulink/Signal Routing/Switch', [modelName '/Switch_V']);
set_param([modelName '/Switch_V'], 'Threshold', '0.5');
set_param([modelName '/Switch_V'], 'Position', [200, 200, 230, 230]);

add_block('simulink/Math Operations/Sum', [modelName '/Sum_V']);
set_param([modelName '/Sum_V'], 'Inputs', '+-');
set_param([modelName '/Sum_V'], 'Position', [350, 225, 380, 255]);

add_block('simulink/Discontinuities/Relay', [modelName '/Relay_V']);
set_param([modelName '/Relay_V'], 'OnSwitchValue', '0.01');
set_param([modelName '/Relay_V'], 'OffSwitchValue', '-0.01');
set_param([modelName '/Relay_V'], 'OnOutputValue', '1');
set_param([modelName '/Relay_V'], 'OffOutputValue', '-1');
set_param([modelName '/Relay_V'], 'Position', [450, 225, 480, 255]);

add_block('simulink/Discontinuities/Saturation', [modelName '/Sat_V']);
set_param([modelName '/Sat_V'], 'UpperLimit', 'Udc');
set_param([modelName '/Sat_V'], 'LowerLimit', '-Udc');
set_param([modelName '/Sat_V'], 'Position', [550, 225, 580, 255]);

add_block('simulink/Sinks/Out1', [modelName '/V_gate']);
set_param([modelName '/V_gate'], 'Position', [650, 225, 680, 255]);

% RL-Motormodell Phase V
add_block('simulink/Math Operations/Gain', [modelName '/R_V']);
set_param([modelName '/R_V'], 'Gain', 'R');
set_param([modelName '/R_V'], 'Position', [900, 300, 930, 330]);

add_block('simulink/Math Operations/Sum', [modelName '/Sum_Motor_V']);
set_param([modelName '/Sum_Motor_V'], 'Inputs', '+-');
set_param([modelName '/Sum_Motor_V'], 'Position', [1000, 225, 1030, 255]);

add_block('simulink/Math Operations/Gain', [modelName '/InvL_V']);
set_param([modelName '/InvL_V'], 'Gain', '1/L');
set_param([modelName '/InvL_V'], 'Position', [1050, 225, 1080, 255]);

add_block('simulink/Continuous/Integrator', [modelName '/Integrator_V']);
set_param([modelName '/Integrator_V'], 'InitialCondition', '0');
set_param([modelName '/Integrator_V'], 'Position', [1100, 225, 1130, 255]);

add_block('simulink/Math Operations/Gain', [modelName '/K_V']);
set_param([modelName '/K_V'], 'Gain', '1');
set_param([modelName '/K_V'], 'Position', [1150, 225, 1180, 255]);

% Verbindungen für Phase V
add_line(modelName, 'IV_SOLL/1',        'Switch_V/1');
add_line(modelName, 'ON_OFF/1',         'Switch_V/2');
add_line(modelName, 'Initialisierung/1','Switch_V/3');
add_line(modelName, 'Switch_V/1',       'Sum_V/1');
add_line(modelName, 'K_V/1',            'Sum_V/2');
add_line(modelName, 'Sum_V/1',          'Relay_V/1');
add_line(modelName, 'Relay_V/1',        'Sat_V/1');
add_line(modelName, 'Sat_V/1',          'V_gate/1');

add_line(modelName, 'Sat_V/1',          'Sum_Motor_V/1');
add_line(modelName, 'R_V/1',            'Sum_Motor_V/2');
add_line(modelName, 'Sum_Motor_V/1',    'InvL_V/1');
add_line(modelName, 'InvL_V/1',         'Integrator_V/1');
add_line(modelName, 'Integrator_V/1',   'K_V/1');
add_line(modelName, 'Integrator_V/1',   'R_V/1');

%% Phase W - identisch zu Phase U und V
add_block('simulink/Signal Routing/Switch', [modelName '/Switch_W']);
set_param([modelName '/Switch_W'], 'Threshold', '0.5');
set_param([modelName '/Switch_W'], 'Position', [200, 350, 230, 380]);

add_block('simulink/Math Operations/Sum', [modelName '/Sum_W']);
set_param([modelName '/Sum_W'], 'Inputs', '+-');
set_param([modelName '/Sum_W'], 'Position', [350, 375, 380, 405]);

add_block('simulink/Discontinuities/Relay', [modelName '/Relay_W']);
set_param([modelName '/Relay_W'], 'OnSwitchValue', '0.01');
set_param([modelName '/Relay_W'], 'OffSwitchValue', '-0.01');
set_param([modelName '/Relay_W'], 'OnOutputValue', '1');
set_param([modelName '/Relay_W'], 'OffOutputValue', '-1');
set_param([modelName '/Relay_W'], 'Position', [450, 375, 480, 405]);

add_block('simulink/Discontinuities/Saturation', [modelName '/Sat_W']);
set_param([modelName '/Sat_W'], 'UpperLimit', 'Udc');
set_param([modelName '/Sat_W'], 'LowerLimit', '-Udc');
set_param([modelName '/Sat_W'], 'Position', [550, 375, 580, 405]);

add_block('simulink/Sinks/Out1', [modelName '/W_gate']);
set_param([modelName '/W_gate'], 'Position', [650, 375, 680, 405]);

% RL-Motormodell Phase W
add_block('simulink/Math Operations/Gain', [modelName '/R_W']);
set_param([modelName '/R_W'], 'Gain', 'R');
set_param([modelName '/R_W'], 'Position', [900, 450, 930, 480]);

add_block('simulink/Math Operations/Sum', [modelName '/Sum_Motor_W']);
set_param([modelName '/Sum_Motor_W'], 'Inputs', '+-');
set_param([modelName '/Sum_Motor_W'], 'Position', [1000, 375, 1030, 405]);

add_block('simulink/Math Operations/Gain', [modelName '/InvL_W']);
set_param([modelName '/InvL_W'], 'Gain', '1/L');
set_param([modelName '/InvL_W'], 'Position', [1050, 375, 1080, 405]);

add_block('simulink/Continuous/Integrator', [modelName '/Integrator_W']);
set_param([modelName '/Integrator_W'], 'InitialCondition', '0');
set_param([modelName '/Integrator_W'], 'Position', [1100, 375, 1130, 405]);

add_block('simulink/Math Operations/Gain', [modelName '/K_W']);
set_param([modelName '/K_W'], 'Gain', '1');
set_param([modelName '/K_W'], 'Position', [1150, 375, 1180, 405]);

% Verbindungen für Phase W
add_line(modelName, 'IW_SOLL/1',        'Switch_W/1');
add_line(modelName, 'ON_OFF/1',         'Switch_W/2');
add_line(modelName, 'Initialisierung/1','Switch_W/3');
add_line(modelName, 'Switch_W/1',       'Sum_W/1');
add_line(modelName, 'K_W/1',            'Sum_W/2');
add_line(modelName, 'Sum_W/1',          'Relay_W/1');
add_line(modelName, 'Relay_W/1',        'Sat_W/1');
add_line(modelName, 'Sat_W/1',          'W_gate/1');

add_line(modelName, 'Sat_W/1',          'Sum_Motor_W/1');
add_line(modelName, 'R_W/1',            'Sum_Motor_W/2');
add_line(modelName, 'Sum_Motor_W/1',    'InvL_W/1');
add_line(modelName, 'InvL_W/1',         'Integrator_W/1');
add_line(modelName, 'Integrator_W/1',   'K_W/1');
add_line(modelName, 'Integrator_W/1',   'R_W/1');

%% Scopes für Visualisierung

% Scope 1: Gate-Signale (Spannungen)
add_block('simulink/Signal Routing/Mux', [modelName '/Mux_Gates']);
set_param([modelName '/Mux_Gates'], 'Inputs', '3');
set_param([modelName '/Mux_Gates'], 'Position', [750, 75, 780, 135]);

add_block('simulink/Sinks/Scope', [modelName '/Scope_Gates']);
set_param([modelName '/Scope_Gates'], 'Position', [820, 75, 880, 135]);

add_line(modelName, 'Sat_U/1',      'Mux_Gates/1');
add_line(modelName, 'Sat_V/1',      'Mux_Gates/2');
add_line(modelName, 'Sat_W/1',      'Mux_Gates/3');
add_line(modelName, 'Mux_Gates/1',  'Scope_Gates/1');

% Scope 2: Gemessene Ströme (zeigt Regelverhalten)
add_block('simulink/Signal Routing/Mux', [modelName '/Mux_Currents']);
set_param([modelName '/Mux_Currents'], 'Inputs', '3');
set_param([modelName '/Mux_Currents'], 'Position', [1220, 175, 1250, 235]);

add_block('simulink/Sinks/Scope', [modelName '/Scope_Currents']);
set_param([modelName '/Scope_Currents'], 'Position', [1280, 175, 1340, 235]);

add_line(modelName, 'Integrator_U/1',  'Mux_Currents/1');
add_line(modelName, 'Integrator_V/1',  'Mux_Currents/2');
add_line(modelName, 'Integrator_W/1',  'Mux_Currents/3');
add_line(modelName, 'Mux_Currents/1',  'Scope_Currents/1');


%% Simulationsparameter setzen
set_param(modelName, 'Solver', 'ode45');           % Standard Solver
set_param(modelName, 'StopTime', '1.0');           % 1 Sekunde Simulation
set_param(modelName, 'MaxStep', '1e-4');           % Kleine Zeitschritte für Genauigkeit

save_system(modelName);
open_system(modelName);
disp('Simulink-Modell "psm_stromregelung.slx" mit geschlossenem Regelkreis wurde erfolgreich erstellt!');
disp(' ');
disp('Das Modell enthält:');
disp('  - Drei-Phasen Stromregler mit Zweipunktreglern (Relay)');
disp('  - RL-Motormodell für jede Phase (geschlossener Regelkreis)');
disp('  - Scope_Gates: Zeigt die Gate-Spannungen (±10V)');
disp('  - Scope_Currents: Zeigt die gemessenen Ströme (Regelverhalten)');
disp(' ');
disp('Parameter:');
fprintf('  R = %.2f Ohm\n', R);
fprintf('  L = %.4f H\n', L);
fprintf('  Udc = %.1f V\n', Udc);



