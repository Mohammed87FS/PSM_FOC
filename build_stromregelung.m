
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
