function [LPF_tfz_PID] = lpf_n1(wlpf_fhz,Ts)
%LEAD_LAG Summary of this function goes here
%   Detailed explanation goes here

wlpf = 2*pi*wlpf_fhz;


z = tf('z',Ts);

s_tustin = (2/Ts)*(z-1)/(z+1);

LPF_tfz_PID = 1/( (s_tustin/wlpf) + 1);

end

