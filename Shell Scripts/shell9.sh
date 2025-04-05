echo "MENU"
echo "1. Number of processes forked since the last boot"
echo "2. Number of processes currently in the system"
echo "3. Number of running processes"
echo "4. Number of blocked processes"
echo "5. PID of the current shell"
echo "6. Number of context switches performed by this shell"	
echo "7 Exit"

while true 
do
	echo ""
	read -p "Enter choice : " ch
	case $ch in 
		1) 	echo -n "Number of processes forked since the last boot : "
			awk '/processes/ {print $2}' /proc/stat;;

		2) 	echo -n "Number of processes currently in the system : "
			awk '{print $5}' /proc/loadavg;;

		3) 	echo "Number of running processes : " $(ps -eo stat | grep -c 'R');;

		4) 	echo "Number of blocked processes : " $(ps -eo stat | grep -c 'D');;

		5)	echo "PID of the current shell : $$";;

		6)	id=$(echo $$)
			echo "Number of context switches performed by this shell : "
			awk '/voluntary_ctxt_switches/ {print $0}' /proc/$id/status;;

		7) exit;;
		
		*) echo "Wrong Choice";;
	esac
done