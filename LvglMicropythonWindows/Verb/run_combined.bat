@echo off
echo Closing existing MicroPython UI process...
for /f "tokens=2 delims==," %%P in ('wmic process where "name='micropython.exe' and commandline like '%%ui_combined.py%%'" get processid /value ^| find "ProcessId"') do (
    echo Terminating process with PID: %%P
    taskkill /F /PID %%P
)

echo Starting new MicroPython UI process...
micropython.exe ui_combined.py > shell_output.txt 2>&1
