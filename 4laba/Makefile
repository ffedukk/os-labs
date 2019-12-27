all:
	clang sh_read.c -Weverything -O0 -o read.out
	clang sh_write.c -Weverything -O0 -o write.out
	echo "!Please start read.out in another console"
	./write.out
	rm write.out
	rm read.out
compile:
	clang sh_read.c -Weverything -O0 -o read.out
	clang sh_write.c -Weverything -O0 -o write.out
debug:
	echo "Write debug make"