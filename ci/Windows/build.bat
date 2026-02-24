call "%ONEAPI_ROOT%\setvars.bat"
set /a "PROC=%NUMBER_OF_PROCESSORS%"
cmake -version

SET STARTDIR=%cd%
SET SCRIPTDIR=%~dp0

cd %SCRIPTDIR%\..\..\

if not defined FEBIO_SDK set "FEBIO_SDK=%STARTDIR%\febio4-sdk"

cmake -L . -B cmbuild ^
  -DCMAKE_PREFIX_PATH=%FEBIO_SDK%

cd cmbuild
msbuild /P:Configuration=Release /P:WarningLevel=0 /m:%PROC% ALL_BUILD.vcxproj
cd %STARTDIR%
exit 0
