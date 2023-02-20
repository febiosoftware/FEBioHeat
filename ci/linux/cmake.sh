LIB_DIR="/usr/lib/x86_64-linux-gnu"
FEBIO_SDK="${FEBIO_SDK:-/febio-sdk}"
run_cmake() {
	cmake -L . -B cmbuild \
		-DFEBio_SDK=$FEBIO_SDK
}
