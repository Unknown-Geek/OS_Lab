recedit() {
	case "$1" in 
		"update")	
			if (($(cat s4csb2.txt | grep "^$2" | wc -l) > 0)); then
				echo -n "Enter Name: "	
				read name
				echo -n "Enter Address: "
				read address
				echo -n "Enter PhoneNo: "
				read phNo
				echo -n "Enter Email: "
				read email
				$(cat s4csb2.txt | grep -v "^$2" > temp.txt)
				cat temp.txt > s4csb2.txt w
				echo "$2 $name $address $phNo $email" >> s4csb2.txt
			else
				echo "$2 ID not found"
			fi;;
		"add")
			if (($(cat s4csb2.txt | grep "^$2" | wc -l) < 1)); then
				echo -n "Enter Name: "	
				read name
				echo -n "Enter Address: "
				read address
				echo -n "Enter PhoneNo: "
				read phNo
				echo -n "Enter Email: "
				read email
				echo "$2 $name $address $phNo $email" >> s4csb2.txt
				echo "Record Added" 
			else
				echo "$2 ID already in the records"
			fi;;
		"search")
			if (($(cat s4csb2.txt | grep "^$2" | wc -l) > 0)); then
				cat s4csb2.txt | grep -w "^$2"
			else
				echo "$2 ID does not exist"
			fi;;
		"delete")
			if (($(cat s4csb2.txt | grep "^$2" | wc -l) > 0)); then
				$(cat s4csb2.txt | grep -v "^$2" > temp.txt)
				cat temp.txt > s4csb2.txt 
				echo "Record Deleted"
			else
				echo "$2 ID not found"
			fi;;
	esac	
}

