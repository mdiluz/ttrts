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

# Exit with usage if not given a number
scalar(@ARGV) or printf $usage_text and exit 1;

# Grab the team
our $team = $ARGV[0];
our $turn = 0;

# If team is non-numeric
($team =~ m/\D+/) and printf $usage_text and exit 1;

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

# Get the units from a turn file
sub GetUnitsForTurn
{
	my $turnFile = $_[0];

	# Open the turn file
	open (my $TURNHANDLE, '<', $turnFile) or die "Could not open '$turnFile' $!";
	
	# Pull in the header information
	my $headerLine = <$TURNHANDLE>;
	chomp $headerLine;
	my $sizeLine = <$TURNHANDLE>;
	chomp $sizeLine;
	my $turnLine = <$TURNHANDLE>;
	chomp $turnLine;
	( <$TURNHANDLE> =~ m/~~~~/ ) or die "Gamestate file did not match expected format";

	my @units;
	while( my $unitLine = <$TURNHANDLE> )
	{
		chomp $unitLine;
		push(@units,$unitLine);
	}

	return @units;
}

# Output the commands file
sub OutputCommandsFile
{
	my $commands = $_[0];

	# Get output file
	our $orderFile = "Turn_TURN_Team_TEAM.txt";
	$orderFile =~ s/TURN/$turn/;
	$orderFile =~ s/TEAM/$team/;

	system ("echo $commands > $orderFile");
}

# Get commands for a turn
sub GetCommandsForTurn
{
	my @units = @_;

	# perform AI here

	return "";
}

# Show launch params
printf("Launching with team %i\n",$team);

# Stay looping the AI
while ( 1 )
{
	# Wait for turn file
	our $turnFile = "Turn_TURN.txt";
	$turnFile =~ s/TURN/$turn/;

	# Wait for the turn file
	WaitForFile $turnFile;

	# Read in the game state from turnFile
	my @units = GetUnitsForTurn($turnFile);

	# Generate some commands
	my $commands = GetCommandsForTurn @units;

	OutputCommandsFile $commands;

	$turn++;

}