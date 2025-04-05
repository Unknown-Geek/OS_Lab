#!/bin/bash
is_hyperthreaded() {
    siblings=$(grep -m 1 "siblings" /proc/cpuinfo | awk '{print $3}')
    cores=$(grep -m 1 "cpu cores" /proc/cpuinfo | awk '{print $4}')
    if [ "$siblings" -gt "$cores" ]; then
        echo "Yes"
    else
        echo "No"
    fi
}

echo "MENU"
echo "(1) Vendor (manufacturer) ID"
echo "(2) Model Name"
echo "(3) Processor Generation"
echo "(4) Number of Processor Chips"
echo "(5) Number of Processor Cores"
echo "(6) Is Your Processor Hyperthreaded?"
echo "(7) Number of Logical Processors"
echo "(8) ID of the Core to Which Each Logical Processor is Mapped"
echo "(9) Speed of Each Logical Processor"
echo "(10) Cache Size"
echo "(11) Exit"

while true; do

    echo ""
    read -p "Enter your choice: " ch

    case $ch in
        1)
            echo -n "Vendor ID: "
            grep -m 1 "vendor_id" /proc/cpuinfo | awk '{print $3}';;

        2)
            echo -n "Model Name: "
            grep -m 1 "model name" /proc/cpuinfo | cut -d ':' -f 2 | sed 's/^ *//';;

        3)
            echo -n "Processor Generation: "
            grep -m 1 "model" /proc/cpuinfo | awk '{print $3}';;

        4)
            echo -n "Number of Processor Chips: "
            grep "physical id" /proc/cpuinfo | sort -u | wc -l;;

        5)
            echo -n "Number of Processor Cores: "
            grep -m 1 "cpu cores" /proc/cpuinfo | awk '{print $4}';;

        6)
            echo -n "Is Your Processor Hyperthreaded? "
            is_hyperthreaded;;

        7)
            echo -n "Number of Logical Processors: "
            grep -c "processor" /proc/cpuinfo;;

        8)
            echo "Core ID Mapping for Each Logical Processor: "
            grep "processor\|core id" /proc/cpuinfo;;

        9)
            echo "Speed of Each Logical Processor: "
            grep "processor\|cpu MHz" /proc/cpuinfo;;

        10)
            echo -n "Cache Size: "
            grep -m 1 "cache size" /proc/cpuinfo | cut -d ':' -f 2 | sed 's/^ *//' ;;

        11)
            exit;;

        *)
            echo "Invalid choice. Please try again."
            ;;
    esac
    echo
done
