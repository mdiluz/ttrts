#! /usr/bin/perl
use strict;
use warnings;

our $usage_text=<<TEXT;
NAME
	simple_player.pl

USAGE
	simple_player.pl TEAM [OPTIONS]

DESCRIPTION
	A very simple perl AI for ttrts

TEXT

# Wait for a file to exist
sub WaitForFile
{
	my $file = $_[0];
	printf("Waiting for %s\n", $file);
	while( ! -e $file )
	{ 
		sleep 1; 
	}
}

# Exit with usage if not given a number
scalar(@ARGV) or printf $usage_text and exit 1;

# Grab the team
our $team = $ARGV[0];

# If team is non-numeric
($team =~ m/\D+/) and printf $usage_text and exit 1;

printf("Launching with team %i\n",$team);

our $turn = 0;

# Stay looping the AI
while ( 1 )
{
	# Wait for turn file
	our $turnFile = "Turn_TURN.txt";
	$turnFile =~ s/TURN/$turn/;

	# Wait for the turn file
	WaitForFile $turnFile;

	# Read in the game state from turnFile

	# Generate some commands
	my $commands = "";

	# Get output file
	our $orderFile = "Turn_TURN_Team_TEAM.txt";
	$orderFile =~ s/TURN/$turn/;
	$orderFile =~ s/TEAM/$team/;

	system ("echo $commands > $orderFile");

	$turn++;

}