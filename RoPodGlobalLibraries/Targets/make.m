disp('%%%%%%%%%%%%% Installing motion target software %%%%%%%%%%%%%%%%')
% copy motion target to /usr/local/matlab*/rtw/c
eval( ['!sudo cp -rf mttarget ' matlabroot '/rtw/c']);
eval( ['!sudo cp -f mttarget/mt_main.c ' matlabroot '/rtw/c/src/common']);
disp('Motion target installed')

disp('%%%%%%%%%%%%% Installing multitask target software %%%%%%%%%%%%%%%%')
% copy motion target to /usr/local/matlab*/rtw/c
eval( ['!sudo cp -rf mtt ' matlabroot '/rtw/c']);
eval( ['!sudo cp -f mtt/mtt_main.c ' matlabroot '/rtw/c/src/common']);
disp('multitask target installed')
