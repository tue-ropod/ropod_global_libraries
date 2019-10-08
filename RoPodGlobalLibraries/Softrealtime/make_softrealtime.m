%% Path defines:

SRCDIR = ['./'];    % Folder where s-function c-source files are located to build

%% List s-functions to compile
C_files = {     'sfun_time.c'
           };

%% Build sfunctions
for i=1:length(C_files)
    mex([SRCDIR,C_files{i}],'-g',['-L','/usr/lib/gcc/x86_64-linux-gnu/5/'],'-lstdc++','-v', 'GCC=''/usr/bin/gcc-4.9''');
end



%% Add path to mex-files and simulink blocks to MATLAB-PATH 
addpath(SRCDIR);


