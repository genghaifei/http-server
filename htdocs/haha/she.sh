#!/bin/bash 
set -x
i=0
file=`ls -l | awk '{print $9}'| grep .jpg`
for name in ${file}
	do
		mv ${name} ${i}.jpg
		let i=${i}+1
	done
set +x
