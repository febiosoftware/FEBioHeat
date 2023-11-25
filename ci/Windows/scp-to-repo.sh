#! /bin/bash
ls -l cmbuild/lib/
scp cmbuild/lib/* repo:~/update2/FEBioStudio2Dev/macOS/stage/FEBioStudio.app/Contents/Frameworks
