FEBIO_SDK="${FEBIO_SDK:-febio4-sdk}"
FEBIO_SDK="$(realpath $FEBIO_SDK)"
run_cmake() {
	cmake -L . -B cmbuild \
		-DCMAKE_PREFIX_PATH=$FEBIO_SDK \
		-DCMAKE_OSX_ARCHITECTURES=x86_64
}
