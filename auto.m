close all
clear, clc

subplot(4,1,1);
ideal = load('20.txt');
xi = ideal(:, 1);
yi = ideal(:, 2);
plot(xi, yi);
title('Series of even inversed');
grid on;
hold on;

ideal = load('21.txt');
xi = ideal(:, 1);
yi = ideal(:, 2);
plot(xi, yi);
hold on;

legend('0', '1');

subplot(4,1,2);
ideal = load('80.txt');
xi = ideal(:, 1);
yi = ideal(:, 2);
plot(xi, yi);
title('Series of eighth inversed');
grid on;
hold on;

ideal = load('81.txt');
xi = ideal(:, 1);
yi = ideal(:, 2);
plot(xi, yi);
hold on;
legend('0', '1');

subplot(4,1,3);
ideal = load('auto.txt');
xi = ideal(:, 1);
yi = ideal(:, 2);
plot(xi, yi);
title('Autocorrelation Test');
ylim([-1 1]);
xlim([0 64]);
grid on;
hold on;

x = 0:1:64;
y = x * 0;
plot(x, y, '--', 'Color',[0,0,0]);
grid on;
hold on;

subplot(4,1,4);
ideal = load('freqs.txt');
yi = ideal(:, 1);
xi = 0:1:63;
plot(xi, yi);
xlim([0 64]);
title('Freqs');
grid on;
hold on;
