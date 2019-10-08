function [LL_tfz_PID] = lead_lag(wz_fhz,wp_fhz,Ts)
%LEAD_LAG Summary of this function goes here
%   Detailed explanation goes here

wz = 2*pi*wz_fhz;
wp = 2*pi*wp_fhz;

P = 1;
I = 0;
N = wp;
D = (1-wz/wp)/wz;

z = tf('z',Ts);

s_tustin = (2/Ts)*(z-1)/(z+1);

LL_tfz_PID = P + I/s_tustin + D*N/(1+N/s_tustin);

end

