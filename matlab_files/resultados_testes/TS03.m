% Dados fornecidos
data = [
    13.00,-1.50,0.00,-10.80,-210.00,3.00,75.00,
13.00,1.80,82.50,-13.20,-60.00,-15.37,-200.00,
13.00,4.20,60.00,4.80,450.00,-16.95,-200.00,
13.00,1.50,-67.50,6.00,30.00,6.64,125.00,
13.00,-4.20,-142.50,-8.40,-360.00,28.73,200.00,
13.00,-4.20,0.00,-10.80,-60.00,8.39,150.00,
13.00,0.60,120.00,-13.20,-60.00,-18.33,-200.00,
14.00,5.10,112.50,2.40,390.00,-26.24,-200.00,
14.00,5.40,7.50,1.20,-30.00,-11.85,-187.00,
14.00,0.00,-135.00,4.80,90.00,19.27,200.00,
14.00,-1.50,-12.20,-15.60,-165.85,4.74,100.00,
14.00,3.90,135.00,-21.60,-150.00,-27.06,-200.00,
14.00,9.60,142.50,-7.20,360.00,-39.51,-200.00,
14.00,8.40,-30.00,-6.00,30.00,-12.49,-200.00,
14.00,2.10,-157.50,-2.40,90.00,18.29,200.00,
14.00,-5.10,-180.00,-19.20,-420.00,35.88,200.00,
14.00,-6.30,-30.00,-19.20,0.00,16.86,200.00,
14.00,-3.90,60.00,-22.80,-90.00,-0.78,-51.00,
14.00,2.40,51.22,-18.00,39.02,-12.10,-200.00,
14.00,-2.40,-120.00,-16.80,30.00,21.92,200.00,
14.00,-11.10,-217.50,-36.00,-480.00,53.21,200.00,
14.00,-16.50,-135.00,-37.20,-30.00,52.22,200.00,
14.00,-20.70,-105.00,-42.00,-120.00,56.32,200.00,
];

% Calcula o tempo acumulado
time = cumsum(data(:, 1));

% Extrai alpha, alpha_ponto, theta, theta_ponto
alpha = data(:, 2);
alpha_ponto = data(:, 3);
theta = data(:, 4);
theta_ponto = data(:, 5);
sinal_controle = data(:, 6);
pwm = data(:, 7);

% Saturar o sinal de controle
sinal_controle(sinal_controle > 12) = 12;
sinal_controle(sinal_controle < -12) = -12;

% Plota alpha_ponto em função do tempo
figure;
plot(time, alpha_ponto);
xlabel('Tempo (s)');
ylabel('alpha\_ponto');
title('alpha\_ponto vs. Tempo');
grid on;

% Plota theta_ponto em função do tempo
figure;
plot(time, theta_ponto);
xlabel('Tempo (s)');
ylabel('theta\_ponto');
title('theta\_ponto vs. Tempo');
grid on;

% Plota theta em função do tempo
figure;
plot(time, theta);
xlabel('Tempo (s)');
ylabel('theta');
title('theta vs. Tempo');
grid on;

% Plota alpha em função do tempo
figure;
plot(time, alpha);
xlabel('Tempo (s)');
ylabel('alpha');
title('alpha vs. Tempo');
grid on;

% Plota alpha e alpha_ponto em função do tempo
figure;
plot(time, alpha, 'b', time, alpha_ponto, 'r');
xlabel('Tempo (s)');
ylabel('alpha e alpha\_ponto');
title('alpha e alpha\_ponto vs. Tempo');
legend('alpha', 'alpha\_ponto');
grid on;

% Plota sinal de controle em função do tempo
figure;
plot(time, sinal_controle);
xlabel('Tempo (s)');
ylabel('sinal de controle');
title('sinal de controle vs. Tempo');
grid on;

% Plota pwm em função do tempo
figure;
plot(time, pwm);
xlabel('Tempo (s)');
ylabel('pwm');
title('pwm vs. Tempo');
grid on;