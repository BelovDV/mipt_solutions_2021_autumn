all: clear tidy format run clean

tidy:
	clang-tidy -extra-arg=-std=c++17 *.cpp

format:
	clang-format -i *.cpp

run:
	g++ -g -std=c++17 -Wall -Wextra -pedantic -Werror -fsanitize=address,undefined -D MY *.cpp
	cat in.txt | ./a.out

all_old: clear run clean

clear:
	clear

clean:
	rm -f ./a.out