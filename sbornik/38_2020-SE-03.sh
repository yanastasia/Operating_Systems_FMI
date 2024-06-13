#!/bin/bash

if [[ "#$" -ne 2 ]]; then 
	echo "Invalid number of arguments" 
	exit 1
fi

repo_path="$(realpath $1)"
package_path="$(realpath $2)"

if [[ $(find "${repo_path}" -mindepth 1 -type f -name "db"| wc -l)  -ne 1 ]]; then
	echo "Invalid repo -db"
	exit 1
fi

if [[ $(find "${repo_path}" -mindepth 1 -type d -name "packages" | wc -l) -ne 1 ]]; then
	echo "Invalid repo - packages"
	exit 1
fi

if [[ $(find "${package_path}" -mindepth 1 -type f -name "version" | wc -l) -ne 1 ]]; then 
	echo "Invalid package - version"
	exit 1
fi

if [[ $(find "${package_path}" -mindepth 1 -type d -name "tree" | wc -l) -ne 1 ]]; then
	echo "Invalid package - tree"
	exit 1
fi

package_name="$(basename "${package_path}")"
package_version="$(cat "${package_path}/version")"


if [[ ! "${package_version}"=~[0-9]+.[0-9]+ ]]; then
	echo "Invalid version"
	exit 1
fi

package_checksum="$(sha256sum "$package_path/tree.tar.xz" | awk '{print $1}')"

mv "${package_name}.tar.xz" "${repo_path}/packages/${package_checksum}.tar.xz"

same_versions="$(cat "${repo_path}/db" | egrep "${package_version}" | wc -l)"

if [[ "${same_versions}" -eq 0 ]]; then
	echo "${package_version}" "${package_checksum}" >> "${repo_path}/db"
	mv "${repo_path}/db" "${repo_path}/tmp"

	cat "${repo_path}/tmp" | sort -k 1,1 > "${repo_path}/db"
	rm "${repo_path}/temp"
else
	cat "${repo_path}/db" | grep -v "${package_version}" > "${repo_path}/tmp"
	echo "${version}" "${package_checksum}" >> "${repo_path}/tmp"
	rm "${repo_path}/db"
	mv "${repo_path}/tmp" "${repo_path}/db"
fi
