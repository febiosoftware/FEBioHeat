#!/bin/bash
set -e

. $(dirname $0)/cmake.sh

main() {
	run_cmake
	pushd cmbuild
	make -j $(nproc)
	popd
}

main
