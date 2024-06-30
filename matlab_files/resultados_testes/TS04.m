% Dados fornecidos
data = [
    0.04,0.00,0.00,0.00,0.00,0.00,0.00,
    0.04,-0.30,-7.50,0.00,0.00,1.67,63.00,
    0.04,-0.30,0.00,-4.80,-120.00,0.60,51.00,
    0.04,0.30,15.00,-6.00,-30.00,-2.74,-75.00,
    0.04,0.90,15.00,1.20,180.00,-3.94,-88.00,
    0.04,1.50,15.00,10.80,240.00,-5.14,-113.00,
    0.04,-0.90,-60.00,13.20,60.00,10.36,175.00,
    0.04,-3.60,-67.50,-4.80,-450.00,16.82,200.00,
    0.04,-0.60,75.00,-8.40,-90.00,-9.51,-162.00,
    0.04,4.80,135.00,8.40,420.00,-28.86,-200.00,
    0.04,4.80,0.00,8.40,0.00,-9.58,-162.00,
    0.04,1.20,-90.00,10.80,60.00,10.45,175.00,
    0.04,-3.60,-120.00,-6.00,-420.00,24.32,200.00,
    0.04,-2.40,30.00,-7.20,-30.00,0.51,51.00,
    0.04,1.80,105.00,-7.20,0.00,-18.58,-200.00,
    0.04,5.70,97.50,9.60,420.00,-25.30,-200.00,
    0.04,4.50,-30.00,10.80,30.00,-4.70,-100.00,
    0.12,-10.80,-124.39,22.80,97.56,39.32,200.00
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
