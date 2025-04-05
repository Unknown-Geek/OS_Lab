read -p "Enter the value of N : " N
read -p "Enter the string word : " word
read -p "Enter filename : " filename

echo ""
content=$(cat $filename)
echo  "Initial Contents in $filename :"
echo "$content"
count=$(cat $filename | grep -i -o $word | wc -l)
echo ""
echo "Occurrence of $word : $count"
echo ""
if ((count > n)); then
	sed -i "s/$word//" $filename
fi
content=$(cat $filename)
echo  "Final Contents in $filename :"
echo "$content"