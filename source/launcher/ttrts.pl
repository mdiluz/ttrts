#! /usr/bin/perl
# Main ttrts launcher script

use strict;
use warnings;
use 5.0;

use Getopt::Long qw(GetOptions);

sub print_usage
{
  print "Unknown option: @_\n" if ( @_ );
  print "Usage: ttrts [--server] [--client] [--host=HOSTNAME] [--map=MAPFILE]\n";
  exit;
}

our $VERBOSE = $ENV{"VERBOSE"};

our $server;
our $client;
our $host;
our $map;

print_usage() if ( @ARGV < 1 or 
	!GetOptions( 
	'client' => \$client,
	'server' => \$server,
	'host=s' => \$host,
	'map=s'  => \$map,
	) );

# Verify we have the right parameters
print "Cannot run as both client and server\n" and exit if $client and $server;
print "Client requires hostname\n" and exit if $client and not $host;
print "Server requires mapfile\n" and exit if $server and not $map;
print "Running locally requires mapfile\n" and exit if not $server and not $client and not $map;

# Run client, server or local
my $res = -1;
$res = system("ttrts-client $host") 	if $client and $host;
$res = system("ttrts-server $map") 		if $server and $map;
$res = system("ttrts-local $map") 		if not $server and not $client and $map;

return $res