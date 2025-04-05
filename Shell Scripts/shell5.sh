read -p "Enter the lower bound  : " a
read -p "Enter the upper bound  : " b
echo "Prime numbers between $a and $b (including limits) are :"
for	;
do
	if (($i==1)); then
		continue
	fi
	
	flag=0
	limit=$(echo "$i/2" | bc)
	for((j=2 ; j<=$limit ; j++));
	do
		mod=$(echo "$i%$j" | bc)
		if (($mod==0)); then
			flag=1
			break
		fi
	done
	if (($flag==0)); then
		echo $i
	fi
done
