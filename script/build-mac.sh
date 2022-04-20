#!/bin/bash

set -e

root=$(cd `dirname $0`; pwd)
install_prefix=${root}/../output/mac
proj_root=${root}/../build/mac


build(){
	mkdir -p ${proj_root}/$2
	cd ${proj_root}/$2
	archs="arm64;x86_64"
	cmake -G Xcode ${root}/proj.cmake -DPLATFORM=MAC -DCMAKE_OSX_ARCHITECTURES=${archs} -DBUILD_TYPE=$2 -DCMAKE_INSTALL_PREFIX=${install_prefix}/$2

	cmake --build . --config $2
	cmake --install . --config $2
	unset archs
}

build MAC Debug
build MAC Release

cd ${root}
