
modelName = 'psm_stromregelung';
if bdIsLoaded(modelName)
    close_system(modelName, 0);
end
if exist([modelName '.slx'], 'file')
    delete([modelName '.slx']);
end

new_system(modelName);

% drei Ströme als Eingänge (Inports)
add_block('simulink/Sources/In1', [modelName '/IU_meas']);  % Phase U
add_block('simulink/Sources/In1', [modelName '/IV_meas']);  %       V
add_block('simulink/Sources/In1', [modelName '/IW_meas']);  %       W

set_param([modelName '/IU_meas'], 'Position', [50, 50, 80, 70]);
set_param([modelName '/IV_meas'], 'Position', [50, 100, 80, 120]);
set_param([modelName '/IW_meas'], 'Position', [50, 150, 80, 170]);


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
set_param([modelName '/ON_OFF'], 'Value', '0');  % spater auf 1 setzen
set_param([modelName '/ON_OFF'], 'Position', [50, 450, 80, 470]);


% regelkreis für Phase U auf,  vergleicht Soll und Istwert und passt an
% der Strom soll auf den Sollwert geregelt werden, um den Motor korrekt zu steuern


% der Switch wählt zwischen zwei Eingängen basierend auf einem Steuersignal
% wenn ON_OFF > 0.5, sollwert wird genommen, sonst startwert
add_block('simulink/Signal Routing/Switch', [modelName '/Switch_U']);
set_param([modelName '/Switch_U'], 'Threshold', '0.5');  % Schwellwert für Umschaltung
set_param([modelName '/Switch_U'], 'Position', [200, 50, 230, 80]);

% iststrom normierung ist ..gain..
% der gemessene Strom wird mit einem Faktor multipliziert, um ihn zu skalieren bzw normieren
% weil Sensoren oft andere Einheiten oder Bereiche haben.. hier wird auf den Regelbereich angepasst
add_block('simulink/Math Operations/Gain', [modelName '/K_U']);
set_param([modelName '/K_U'], 'Gain', '4.46');  % normierungsfaktor.. berechnet aus Systemparameter..
set_param([modelName '/K_U'], 'Position', [200, 100, 230, 130]);

% fehler berechnung .. also soll - ist
% jenachdem wie weit man vom sollwert entfernt ist, regelt man
add_block('simulink/Math Operations/Sum', [modelName '/Sum_U']);
set_param([modelName '/Sum_U'], 'Inputs', '+-');  % Plus und Minus: erster Eingang +, zweiter -
set_param([modelName '/Sum_U'], 'Position', [350, 75, 380, 105]);

% zweipunktregler (Relay)
%  Wenn Fehler positiv, Ausgang +0.01; wenn negativ, -0.01.. sonst 0.
% nur zwei Zustände (an/aus), wie ein Thermostat.
% In Motorsteuerung steuert das die Leistungselektronik (wie igbts).
add_block('simulink/Discontinuities/Relay', [modelName '/Relay_U']);
set_param([modelName '/Relay_U'], 'OnSwitchValue', '0.01');   % Wenn Fehler > 0, Ausgang +0.01
set_param([modelName '/Relay_U'], 'OffSwitchValue', '-0.01'); % Wenn Fehler < 0, Ausgang -0.01
set_param([modelName '/Relay_U'], 'Position', [500, 75, 530, 105]);


% der Ausgang des Reglers.. Gate Signal für die Phase U (steuert den Stromfluss)
add_block('simulink/Sinks/Out1', [modelName '/U_gate']);
set_param([modelName '/U_gate'], 'Position', [650, 75, 680, 105]);

% Verbindungen für Phase U

add_line(modelName, 'IU_SOLL/1',        'Switch_U/1'); % Sollwert geht zu Switch-Eingang 1
add_line(modelName, 'ON_OFF/1',         'Switch_U/2'); % ON/OFF steuert den Switch
add_line(modelName, 'Initialisierung/1','Switch_U/3'); % Initialisierung zu Switch-Eingang 2

add_line(modelName, 'IU_meas/1', 'K_U/1');       % Gemessener Strom zu Gain
add_line(modelName, 'Switch_U/1', 'Sum_U/1');    % Ausgewählter Sollwert zu Sum (positiv)
add_line(modelName, 'K_U/1',      'Sum_U/2');    % Normierter Istwert zu Sum (negativ)
add_line(modelName, 'Sum_U/1',    'Relay_U/1');  % Fehler zu Relay
add_line(modelName, 'Relay_U/1',  'U_gate/1');   % Relay-Ausgang zu Gate

%% Phasen V und W aufbauen
% Dasselbe wie für U

% Phase V
add_block('simulink/Signal Routing/Switch', [modelName '/Switch_V']);
set_param([modelName '/Switch_V'], 'Threshold', '0.5');
set_param([modelName '/Switch_V'], 'Position', [200, 200, 230, 230]);

add_block('simulink/Math Operations/Gain', [modelName '/K_V']);
set_param([modelName '/K_V'], 'Gain', '4.46');
set_param([modelName '/K_V'], 'Position', [200, 250, 230, 280]);

add_block('simulink/Math Operations/Sum', [modelName '/Sum_V']);
set_param([modelName '/Sum_V'], 'Inputs', '+-');
set_param([modelName '/Sum_V'], 'Position', [350, 225, 380, 255]);

add_block('simulink/Discontinuities/Relay', [modelName '/Relay_V']);
set_param([modelName '/Relay_V'], 'OnSwitchValue', '0.01');
set_param([modelName '/Relay_V'], 'OffSwitchValue', '-0.01');
set_param([modelName '/Relay_V'], 'Position', [500, 225, 530, 255]);

add_block('simulink/Sinks/Out1', [modelName '/V_gate']);
set_param([modelName '/V_gate'], 'Position', [650, 225, 680, 255]);

% Verbindungen für Phase V
add_line(modelName, 'IV_SOLL/1',        'Switch_V/1');
add_line(modelName, 'ON_OFF/1',         'Switch_V/2');
add_line(modelName, 'Initialisierung/1','Switch_V/3');

add_line(modelName, 'IV_meas/1', 'K_V/1');
add_line(modelName, 'Switch_V/1', 'Sum_V/1');
add_line(modelName, 'K_V/1',      'Sum_V/2');
add_line(modelName, 'Sum_V/1',    'Relay_V/1');
add_line(modelName, 'Relay_V/1',  'V_gate/1');

% Phase W
add_block('simulink/Signal Routing/Switch', [modelName '/Switch_W']);
set_param([modelName '/Switch_W'], 'Threshold', '0.5');
set_param([modelName '/Switch_W'], 'Position', [200, 350, 230, 380]);

add_block('simulink/Math Operations/Gain', [modelName '/K_W']);
set_param([modelName '/K_W'], 'Gain', '4.46');
set_param([modelName '/K_W'], 'Position', [200, 400, 230, 430]);

add_block('simulink/Math Operations/Sum', [modelName '/Sum_W']);
set_param([modelName '/Sum_W'], 'Inputs', '+-');
set_param([modelName '/Sum_W'], 'Position', [350, 375, 380, 405]);

add_block('simulink/Discontinuities/Relay', [modelName '/Relay_W']);
set_param([modelName '/Relay_W'], 'OnSwitchValue', '0.01');
set_param([modelName '/Relay_W'], 'OffSwitchValue', '-0.01');
set_param([modelName '/Relay_W'], 'Position', [500, 375, 530, 405]);

add_block('simulink/Sinks/Out1', [modelName '/W_gate']);
set_param([modelName '/W_gate'], 'Position', [650, 375, 680, 405]);

% Verbindungen für Phase W
add_line(modelName, 'IW_SOLL/1',        'Switch_W/1');
add_line(modelName, 'ON_OFF/1',         'Switch_W/2');
add_line(modelName, 'Initialisierung/1','Switch_W/3');

add_line(modelName, 'IW_meas/1', 'K_W/1');
add_line(modelName, 'Switch_W/1', 'Sum_W/1');
add_line(modelName, 'K_W/1',      'Sum_W/2');
add_line(modelName, 'Sum_W/1',    'Relay_W/1');
add_line(modelName, 'Relay_W/1',  'W_gate/1');
