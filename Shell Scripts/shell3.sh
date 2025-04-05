echo "MENU"
echo "1. Total memory"
echo "2. Amount of memory left unused"
echo "3. Amount of memory available"
echo "4. Cache Size"	
echo "5. Exit"

while true 
do
	echo ""
	read -p "Enter choice : " ch
	case $ch in 
		1) awk '/MemTotal/ {print $2 " kB"}' /proc/meminfo;;

		2) awk '/MemFree/ {print $2 " kB"}' /proc/meminfo;;

		3) awk '/MemAvailable/ {print $2 " kB"}' /proc/meminfo;;

		4) awk '/^Cached/ {print $2 " kB"}' /proc/meminfo;;

		5) exit;;
		
		*) echo "Wrong Choice";;
	esac
done