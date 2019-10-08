%% Path defines:
SOEMIDIR = '/usr/local/include/soem/';  % Folder where SOEM include files are located
LIBDIR = '/usr/local/lib/';             % Folder where libsoem.so library is located
SRCDIR = [pwd,'/ECAT_s_functions/'];    % Folder where s-function c-source files are located to build
SLBDIR = [pwd,'/ECAT_Simulink_Lib/'];   % Folder where EtherCAT simulink library is located

%% List s-functions to compile
C_files = {'ECAT_Interface.c';
           'ec_EL3102_adc.c';
           'ec_EL4132_dac.c';
           'ec_EL1008_di.c';
           'ec_EL2008_do.c';
           'ec_TU_EightWheelBot.c';
           'ec_ROPOD_SmartWheel.c';
           'ec_ROPOD_SmartWheel2.c';
           };

%% Build sfunctions
for i=1:length(C_files)
    mex([SRCDIR,C_files{i}],'-g',['-I',SOEMIDIR],['-L',LIBDIR],'-lsoem',['-L','/usr/lib/gcc/x86_64-linux-gnu/5/'],'-lstdc++','-v', 'GCC=''/usr/bin/gcc-4.9''');
end

%% Move sfunctions to SRCDIR
movefile('*.mexa64*',SRCDIR)

%% Add path to mex-files and simulink blocks to MATLAB-PATH
addpath(SRCDIR);
addpath(SLBDIR);

