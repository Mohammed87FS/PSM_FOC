
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
