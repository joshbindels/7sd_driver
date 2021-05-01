gpio mode 0 out
gpio mode 1 out
gpio mode 2 out
gpio mode 3 out
gpio mode 4 out
gpio mode 5 out
gpio mode 6 out
gpio mode 7 out

function clear {
	gpio write 0 0
	gpio write 1 0
	gpio write 2 0
	gpio write 3 0
	gpio write 4 0
	gpio write 5 0
	gpio write 6 0
	gpio write 7 0
}

clear

# 1

gpio write 2 1
gpio write 7 1

sleep 1
clear

#2

gpio write 0 1
gpio write 1 1
gpio write 4 1
gpio write 6 1
gpio write 7 1

sleep 1
clear

# 3

gpio write 1 1
gpio write 2 1
gpio write 4 1
gpio write 6 1
gpio write 7 1

sleep 1
clear

gpio write 3 1

