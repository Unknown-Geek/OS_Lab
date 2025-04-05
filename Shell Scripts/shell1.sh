echo "(1) OS name, its version and kernel version"
echo "(2) number of user accounts"
echo "(3) currently logged user and his login name"
echo "(4) all available shells"
echo "(5) your current shell"
echo "(6) your home directory"
echo "(7) your current path setting"
echo "(8) your current working directory"
echo "(9) Exit"

while true
do
	echo ""
	read -p "Enter your choice : " ch
	case $ch in 
		1) 	echo "OS name : " $(uname -o ) 
			echo "Version : " $(uname -or) 
			echo "Kernel Version : " $(uname -v);;

		2)	echo "Number of users : " $(wc -l /etc/passwd);;

		3)	echo "Current User : " $(whoami);;

		4)	echo "Available Shells : "
			echo $(cat /etc/shells);;

		5)	echo "Current Shell :"
			echo $SHELL;;

		6)	cd
			pwd;;

		7)	echo "$PATH";;

		8)	pwd;;

		9) exit;;

		*) echo "Wrong Choice"
	esac
done