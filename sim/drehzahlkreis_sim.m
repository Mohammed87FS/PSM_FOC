% Drehzahlkreis mit dem IT1-Modell aus dem Labor simulieren
%   Strecke: iq_soll -> PT1 (Te, Stromkreis + Ersatzzeitkonstante) -> 1/(s*Tm) -> n
%   Regler:  PI nach symmetrischem Optimum, Ausgang auf +-1 begrenzt (normierter Strom)
% Einmal mit Back-Calculation Anti-Windup (wie im Modell) und einmal ohne.
% Alles normiert: n = 1 entspricht 3000 1/min, iq = 1 entspricht Nennstrom.

Te = 0.045;            % gemessen bei 50/100/200 V, Mittelwert
Kp = 5.52;             % Kp = Tm/(2*Te)
Ti = 4*Te;             % 0.18 s
Tm = 2*Te*Kp;          % 0.497 s, passt zu ca. 0.5 s bis 3000 1/min bei iq = 1
Ki = Kp/Ti;            % 30.45, so steht es im PID-Block (Parallelform)
Kb = Ki;               % Back-Calculation Koeffizient

dt   = 1e-4;
tEnd = 2.0;
t    = (0:dt:tEnd)';
nSoll = 1500/3000 * (t >= 0.1);          % Sprung auf 1500 1/min

cases = {'mit Anti-Windup (Kb = 30.45)', 'ohne Anti-Windup'};
n_all  = zeros(numel(t), 2);
iq_all = zeros(numel(t), 2);

for c = 1:2
    xI = 0;  m = 0;  n = 0;
    for k = 1:numel(t)
        e   = nSoll(k) - n;
        u   = Kp*e + xI;                 % Reglerausgang vor der Begrenzung
        iq  = min(max(u, -1), 1);        % Begrenzung auf +-1
        if c == 1
            dxI = Ki*e + Kb*(iq - u);    % Back-Calculation: Integrator wird zurueckgeholt
        else
            dxI = Ki*e;                  % Integrator laeuft weiter (Wind-Up)
        end
        xI = xI + dt*dxI;
        m  = m + dt*(iq - m)/Te;         % PT1
        n  = n + dt*m/Tm;                % Integrator
        n_all(k, c)  = n;
        iq_all(k, c) = iq;
    end
end

blau   = [0 0.447 0.741];
orange = [0.85 0.325 0.098];

figure(1)
subplot(2,1,1)
plot(t, nSoll*3000, 'k:', t, n_all(:,1)*3000, '-', t, n_all(:,2)*3000, '-')
set(gca, 'ColorOrder', [0 0 0; blau; orange])
h = get(gca, 'Children');
set(h(2), 'Color', blau);  set(h(1), 'Color', orange);
grid on
ylabel('n   [1/min]')
legend(['Sollwert', cases], 'Location', 'southeast')
title('Drehzahlsprung 0 -> 1500 1/min, IT1-Modell mit T_e = 45 ms, T_m = 0.5 s')

subplot(2,1,2)
plot(t, iq_all(:,1), 'Color', blau)
hold on
plot(t, iq_all(:,2), 'Color', orange)
hold off
grid on
ylim([-1.2 1.2])
xlabel('t   [s]')
ylabel('i_q   (normiert)')

for c = 1:2
    os = (max(n_all(:,c)) - 0.5) / 0.5 * 100;
    fprintf('%-32s Ueberschwingen %5.1f %%\n', cases{c}, os);
end

if ~exist('../docs', 'dir'), mkdir('../docs'); end
exportgraphics(gcf, '../docs/drehzahlkreis_sim.png', 'Resolution', 150)
