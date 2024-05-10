call "%ONEAPI_ROOT%\setvars.bat"
set /a "PROC=%NUMBER_OF_PROCESSORS%"
cmake -version

SET STARTDIR=%cd%
SET SCRIPTDIR=%~dp0

cd %SCRIPTDIR%\..\..\

cmake -L . -B cmbuild ^
  -DFEBio_SDK=%STARTDIR%\febio4-sdk
)
cd cmbuild
msbuild /P:Configuration=Release /P:WarningLevel=0 /m:%PROC% ALL_BUILD.vcxproj
cd %STARTDIR%
exit 0
