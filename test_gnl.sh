make_gnl()
{
	make -C libft > log
	clang -Wall -Wextra -Werror -I libft/includes -o get_next_line.o -c get_next_line.c
	clang -Wall -Wextra -Werror -I libft/includes -o main.o -c main.c
	clang -o test_gnl main.o get_next_line.o -I libft/includes -L libft/ -lft
	make fclean -C libft > log
	rm log
}
do_test()
{
	chmod 666 temp.txt
	DIFF=$(diff res$2.txt temp.txt)

	test -f has_leaks
	if  [ -f has_leaks ]
	then
	    echo "$1: leaks found"
	fi
	if [ "$DIFF" != "" ]
	then
		echo "$1: wrong result"
	elif [ -f has_leaks ]
	then
		rm has_leaks
	else
		echo "$1: OK :D"
	fi

	rm temp.txt
}
timeout() 
{
    time=$1
    command="$2"
    expect -c "set echo \"-noecho\"; set timeout $time; spawn -noecho $command; expect timeout { exit 1 } eof { exit 0 }"    

    if [ $? = 1 ]
    then
        echo "Timeout after ${time} seconds"
    else
    	do_test ">>Test for big file (BUFF_SIZE=4096)" 2
    fi
}

gcc -o change_buff change_buff.c
max=100000000
for (( i=1; i <= $max; i = i * 10 ))
do
	./change_buff "$i"
	make_gnl
	./test_gnl 1
	
	do_test ">>Test for multiple fd (BUFF_SIZE=$i)" 1
done

./change_buff 4096
make_gnl
timeout 3 "./test_gnl 2"

rm test_gnl
rm change_buff
rm get_next_line.o
rm main.o