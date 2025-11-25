
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
