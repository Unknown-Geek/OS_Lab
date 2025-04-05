echo "MENU"
echo "1.Convert to binary"
echo "2.Convert to octal"
echo "3.Convert to hexadecimal"

convert_to_binary() {
	num=$1
	binary=""
	while(($num > 0)); do
		binary=$(($num%2))$binary
		num=$(($num/2))
	done
	echo $binary
}

convert_to_octal() {
	num=$1
	octal=""
	while(($num > 0)); do
		octal=$(($num%8))$octal
		num=$(($num/8))
	done
	echo $octal
}

convert_to_hexadecimal() {
	num=$1
	hexa=""
	hexa_chars=("0" "1" "2" "3" "4" "5" "6" "7" "8" "9" "A" "B" "C" "D" "E" "F")
	while(($num > 0)); do
		rem=$(($num%16))
		hexa=${hexa_chars[$rem]}$hexa
		num=$(($num/16))
	done
	echo $hexa
}

echo ""
read -p "Enter a positive decimal number : " decimal
if ((decimal < 0)); then
	echo "Entered number should be positive"
	exit
fi

while true 
do 
	echo ""
	read -p "Enter your choice : " ch

	case $ch in
	   	1) echo -n "Binary : " 
			convert_to_binary $decimal;;

		2) echo -n "Octal : " 
			convert_to_octal $decimal;;

		3) echo -n "Hexadecimal : " 
			convert_to_hexadecimal $decimal;;

		4) exit;;

	    *)	echo "Wrong Choice";;
	esac
done
