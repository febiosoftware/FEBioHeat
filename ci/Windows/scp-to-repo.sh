#! /bin/bash
ls -l cmbuild/Release/
scp cmbuild/Release/* repo:~/update2/FEBioStudio2Dev/Windows/stage/bin
