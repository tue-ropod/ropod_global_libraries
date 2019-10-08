Ts = 1/1000;
% lead-lag

z = tf('z');

s = tf('z');



%% test
%% Wheel Vel controller
% No integrators at the wheel velocity level!;
load wheel_vel_cntr_GLL.mat
dvarphi_wheel_cntr = shapeit_data.C_tf_z;

K_gain_dvarphi_cntr = 0.028;
I_fhz_dvarphi_cntr = 0;
LL_wz_fhz_dvarphi_cntr = 2; % Use the same value for not having the LL
LL_wp_fhz_dvarphi_cntr = 20;
LPF_fhz_dvarphi_cntr = 100;

Cntr_PIDlike = K_gain_dvarphi_cntr*lead_lag(LL_wz_fhz_dvarphi_cntr,LL_wp_fhz_dvarphi_cntr,Ts)...
    * lpf_n1(LPF_fhz_dvarphi_cntr,Ts);

figure
bode(0.28*dvarphi_wheel_cntr);
hold on;
bode(Cntr_PIDlike);
