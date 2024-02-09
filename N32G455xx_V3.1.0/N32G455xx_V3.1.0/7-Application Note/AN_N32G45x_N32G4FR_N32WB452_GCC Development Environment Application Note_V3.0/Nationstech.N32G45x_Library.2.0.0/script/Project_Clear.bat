@echo on
rem  Init project clear ....
pause

::延时
::ping -n 30 127.0.0.1 > null
cd ..
set "f_path=%cd%"
::for Keil
for /r %f_path% %%d in (MDK-ARM) do (
  
    del "%%d\*.scvd"
    del "%%d\*.uvguix.*"
 
    rd /s /q "%%d\Objects"
    rd /s /q "%%d\Listings"
    rd /s /q "%%d\bin"

)

::for IAR
for /r %f_path% %%d in (EWARM) do (
    del "%%d\*.ewt"
    del "%%d\*.uvoptx"
    del "%%d\*.uvprojx"
    del "%%d\ConvertToIARReport.txt"

    rd /s /q "%%d\settings"
    rd /s /q "%%d\Debug"

)

::for GCC
for /r %f_path% %%d in (GCC) do (
    del "%%d\*.o"
    del "%%d\*.d"
    del "%%d\*.lst"
    del "%%d\*.elf"
    del "%%d\*.bin"
    del "%%d\*.hex"
    del "%%d\*.map"

    rd /s /q "%%d\build"

)
rem Project cleared done
pause