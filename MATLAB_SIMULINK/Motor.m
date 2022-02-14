clear a;
a = arduino();

time = 200;
fprintf('Spin Motor');
writeDigitalPin(a, 'D12', 0); %% Set direction of motor
writeDigitalPin(a, 'D13', 1);
while time > 0
    voltage = readVoltage(a, 'A0');         %% Get Voltage from potentiometer
    fprintf('Pot value/: %f \n', voltage);
    writePWMVoltage(a, 'D11', voltage);     %% Write Voltage to motor
    writePWMVoltage(a, 'D6', voltage);      %% Write Voltage to LED
    time = time - 1;
    pause(0.1);
end

time = 200;
fprintf('Switch Direction');
writeDigitalPin(a, 'D12', 1);%% Reverse direction of motor
writeDigitalPin(a, 'D13', 0);
while time > 0
    voltage = readVoltage(a, 'A0');         %% Get Voltage from potentiometer
    fprintf('Pot value/: %f \n', voltage);
    writePWMVoltage(a, 'D11', voltage);     %% Write Voltage to motor
    writePWMVoltage(a, 'D6', voltage);      %% Write Voltage to LED

    time = time - 1;
    pause(0.1);
end
writePWMVoltage(a, 'D11', 0);