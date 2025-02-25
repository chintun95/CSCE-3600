#! /bin/bash

curve(){
	incr="$1"
	shift
	arr=("$@")
	index=0

	for i in "${arr[@]}"
	do
		let grades[$index]=$i+$incr
		let index=$index+1
	done
}

if [ $# -ne 1 ]
	then	
		echo "Usage: $0 <curve_amount>"
		exit 1
fi

grades=()
for i in {1..5}
do
	echo -n " Enter QUIZ #$i: "
	read grade
	grades[$i-1]=$grade
done

curve $1 "${grades[@]}"

echo "Curved grades:"

for((i=0; i<${#grades[@]}; i++))
do
	echo "grades[$i] = ${grades[$i]}"
done

