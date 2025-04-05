read -p "Enter basic salary : " basic
dp=$(echo "0.5*$basic" | bc)
da=$(echo "0.35*$(echo "$basic+$dp" | bc)" | bc)
hra=$(echo "0.08*$(echo "$basic+$dp" | bc)" | bc)
ma=$(echo "0.03*$(echo "$basic+$dp" | bc)" | bc)
pf=$(echo "0.1*$(echo "$basic+$dp" | bc)" | bc)
salary=$(echo "$basic+$dp+$da+$hra+$ma-$pf" | bc)
echo "Salary : $salary"
