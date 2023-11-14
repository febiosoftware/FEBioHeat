call "%ONEAPI_ROOT%\setvars.bat"
set /a "PROC=%NUMBER_OF_PROCESSORS%"
cmake -version

cmake -L . -B cmbuild ^
  -DFEBio_SDK=febio4-sdk
)
cd cmbuild
msbuild /P:Configuration=Release /P:WarningLevel=0 /m:%PROC% ALL_BUILD.vcxproj
cd ..
exit 0
