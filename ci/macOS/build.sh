#!/bin/bash
set -e

. $(dirname $0)/cmake.sh

main() {
	run_cmake
	pushd cmbuild
	make -j $(sysctl -n hw.ncpu)
	popd
}

main
