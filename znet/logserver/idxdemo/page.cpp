#include <iostream>

int main()
{
	int fd = open("/home/lijinhua/log/any.log", O_RDONLY);
	char buffer[4096];
	lseek(fd, 4096, SEEK_SET);
	read(fd, buffer, 4096 );
	
	return 0;
}
