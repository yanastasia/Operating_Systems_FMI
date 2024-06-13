#!/bin/bash

if [[ "$#" -ne 2 ]]; then
	echo "Invalid number of arguments"
	exit 1
fi

FILE="${1}"
DIR="${2}"

if [[ ! -d "${DIR}" ]]; then
	echo "${DIR} does not exist"
	exit 1
fi

if [[ ! -r "${DIR}" ]]; then
	echo "${DIR} is not readable"
	exit
fi

file_type="$(echo ${FILE} | rev | cut -d '.' -f 1 | rev)"

if [[ file_type != '.csv' ]]; then
	echo "File ${FILE} not csv."
	exit 1
fi

touch "${FILE}"

echo "hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key" > "${FILE}"


while read fileName; do
	hostname="echo ${fileName} | cut -d '.' -f 1 | rev | cut -d '/' -f 1 | rev"
	phy="cat ${fileName} | grep "Maximum Physical Interfaces" | tr -s ' '| cut -d ':' -f 2"
	vlans="cat ${fileName} | grep "VLANs" | tr -s ' ' | cut -d ':' -f 2"
	hosts="cat ${fileName} | grep "Inside Hosts" | tr -s ' ' | cut -d ':' -f 2"
	failover="cat ${fileName} | grep "Failover" | tr -s ' ' | cut -d ':' -f 2"
	vpns="cat ${fileName} | grep "VPN-3DES-AES" | tr -s ' ' | cut -d ':' -f 2"
	peers="cat ${fileName} | grep "Total VPN Peers" | tr -s ' ' | cut -d ':' -f 2"
	ports="cat ${fileName} | grep "VLAN Trunk Ports" | tr -s ' ' | cut -d ':' -f 2"
	license="cat ${fileName} | grep "license" | sed "s/ license.//"| cut -d ' ' -f 5-"
	sn="cat ${fileName} | grep "Serial Number" | tr -s ' ' | cut -d ':' -f 2"
	key="cat ${fileName} | grep "Running Activation Key" | tr -s ' ' | cut ':' -f 2"
	echo "${hostname},${phy},${vlans},${hosts},${failover},${vpns},${peers},${ports},${license},${sn},${key}" >> "${FILE}"
done < <(find "${DIR}" | grep ".log$")


