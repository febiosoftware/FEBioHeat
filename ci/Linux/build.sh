#!/bin/bash
set -e

. $(dirname $0)/cmake.sh

main() {
    pushd $(dirname $0)/../..
	run_cmake
	pushd cmbuild
	make -j $(nproc)
	popd
    popd
}

main
