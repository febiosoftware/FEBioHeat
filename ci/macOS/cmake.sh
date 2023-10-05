FEBIO_SDK="${FEBIO_SDK:-febio4-sdk}"
run_cmake() {
	cmake -L . -B cmbuild \
		-DFEBio_SDK=$FEBIO_SDK \
		-DCMAKE_OSX_ARCHITECTURES=x86_64
}
