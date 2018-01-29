RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NORMAL='\033[0m'


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
	DIFF=$(diff res$2.txt temp.txt)

	test -f has_leaks
	if  [ -f has_leaks ]
	then
	    echo "${RED}$1: leaks found${NORMAL}"
	fi
	if [ "$DIFF" != "" ]
	then
		echo "${RED}$1: wrong result${NORMAL}"
	elif [ -f has_leaks ]
	then
		rm has_leaks
	else
		echo "${GREEN}$1: OK :D${NORMAL}"
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
        echo "${YELLOW}>>Test for big file (BUFF_SIZE=4096): Timeout after ${time} seconds"
    else
    	chmod 666 temp.txt
    	do_test ">>Test for big file (BUFF_SIZE=4096)" 2
    fi
}

mv main.c main
mv change_buff.c change_buff

NORM=$(norminette | grep -e Error)
if [ "$NORM" != "" ]
then
	echo "${RED}NORM ERROR"
fi

mv main main.c
mv change_buff change_buff.c

gcc -o change_buff change_buff.c
max=100000000
for (( i=1; i <= $max; i = i * 10 ))
do
	./change_buff "$i"
	make_gnl
	./test_gnl 1
	chmod 666 temp.txt

	do_test ">>Test for multiple fd (BUFF_SIZE=$i)" 1
done

./change_buff 4096
make_gnl
timeout 3 "./test_gnl 2"

rm test_gnl
rm change_buff
rm get_next_line.o
rm main.o