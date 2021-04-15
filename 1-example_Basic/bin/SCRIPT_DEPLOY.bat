@echo off 

echo "------------------ CREATING Windows INSTALLER ------------------"
echo "for an OpenFrameworks compiled binary app" 
echo "Setting Icon using rcedit: https://github.com/electron/rcedit"
OF_Windows_Installer\resources\rcedit-x64 "Paletto.exe" --set-icon Paletto.ico"

echo "Using NSIS: Nullsoft Scriptable Install System: https://sourceforge.net/projects/nsis/"
"c:\Program Files (x86)\NSIS\makensis.exe" OF_Windows_Installer\SCRIPT_OF_APP.nsi