echo "1. Add"
echo "2. Subtract"
echo "3. Multiply"
echo "4. Divide"
echo "5. Remainder"
echo "6. sin"
echo "7. cos"
echo "8. tan"
echo "9. Square Root"
echo "10. Power"
echo "11. Exit"

while true
do
	echo ""
    read -p "Enter your choice : " ch
    case $ch in
        1)	read -p "Enter first number : " n1
            read -p "Enter second number : " n2
            result=$(echo "$n1+$n2" | bc -l)
            echo "Result : $result";;

        2)  read -p "Enter first number : " n1
            read -p "Enter second number : " n2
            result=$(echo "$n1-$n2" | bc -l)
            echo "Result : $result";;

        3)  read -p "Enter first number : " n1
            read -p "Enter second number : " n2
            result=$(echo "$n1*$n2" | bc -l)
            echo "Result : $result";;

        4)  read -p "Enter first number : " n1
            read -p "Enter second number : " n2
            result=$(echo "scale=2; $n1/$n2" | bc -l)
            echo "Result : $result";;

        5)  read -p "Enter first number : " n1
            read -p "Enter second number : " n2
            result=$(echo "$n1%$n2" | bc)
            echo "Result : $result";;

        6)  read -p "Enter angle : " ang
            result=$(echo "scale = 3; s($ang*a(1)/45)" | bc -l)
            echo "Result : $result";;

        7)  read -p "Enter angle : " ang
            result=$(echo "scale = 3; c($ang*a(1)/45)" | bc -l)
            echo "Result : $result";;
            
        8)  read -p "Enter angle : " ang
            sin=$(echo "s($ang*a(1)/45)" | bc -l)
            cos=$(echo "c($ang*a(1)/45)" | bc -l)
            result=$(echo "scale = 3; $sin/$cos" | bc -l)
            echo "Result : $result";;

        9)  read -p "Enter the number : " base
            result=$(echo "sqrt($base)" | bc -l)
            echo "Result : $result";;

        10) read -p "Enter base : " base
            read -p "Enter exponent : " exp
            result=$(echo "$base^$exp" | bc -l)
            echo "Result : $result";;

        11) exit;;

        *)	echo "Wrong Choice";;
    esac
done
