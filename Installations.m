disp("----------Installing SOEM, Targets and Timer_posix---------");
run("RoPodGlobalLibraries/ECAT_ropod_Lib_src/soem/Build_and_Install_SOEM.m");
run("RoPodGlobalLibraries/Targets/make.m");
run("RoPodGlobalLibraries/Timer_posix/Build_and_install_timer_posix.m");
disp("----------All Installations Complete----------");