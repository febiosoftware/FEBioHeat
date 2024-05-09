LIB_DIR="/usr/lib/x86_64-linux-gnu"

if [ -d febio4-sdk ]; then
    FEBIO_SDK="${FEBIO_SDK:-febio4-sdk}"
else
    FEBIO_SDK="${FEBIO_SDK:-..}"
fi

run_cmake() {
	cmake -L . -B cmbuild \
		-DFEBio_SDK=$FEBIO_SDK
}
