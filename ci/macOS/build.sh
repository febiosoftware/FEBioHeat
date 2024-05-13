#!/bin/bash
set -e

. $(dirname $0)/cmake.sh

main() {
    pushd $(dirname $0)/../..
	run_cmake
	pushd cmbuild
	make -j $(sysctl -n hw.ncpu)
	popd
    popd
}

main
