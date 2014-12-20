#! /usr/bin/perl
use strict;
use warnings;

# Get information about a unit from it's descriptor
sub getUnit
{
	return ($_[0] =~ /UNIT:(\d+) tm:(\d+) vs:([^ ]+) dr:([^ ]+) ps:\[(\d+),(\d+)\]/);
}

# Get the units from a turn file
sub GetUnitsForTurn
{
	my $turnFile = shift;

	# Open the turn file
	open (my $TURNHANDLE, '<', $turnFile) or die "Could not open '$turnFile' $!";
	
	# Skip the header information
	my $headerLine = <$TURNHANDLE>;
	my $sizeLine = <$TURNHANDLE>;
	my $turnLine = <$TURNHANDLE>;
	( <$TURNHANDLE> =~ /~~~~/ ) or die "Gamestate file did not match expected format";

	my @units;
	while( my $unitLine = <$TURNHANDLE> )
	{
		chomp $unitLine;
		push(@units,$unitLine);
	}

	close $TURNHANDLE;

	return @units;
}

# Get information from the header for this turn
sub GetHeaderForTurn
{
	my $turnFile = shift;

	# Open the turn file
	open (my $TURNHANDLE, '<', $turnFile) or die "Could not open '$turnFile' $!";
	
	# Pull in the header information
	my $headerLine = <$TURNHANDLE>;
	chomp $headerLine;
	my $sizeLine = <$TURNHANDLE>;
	chomp $sizeLine;
	my $turnLine = <$TURNHANDLE>;
	chomp $turnLine;

	my ($gameName) = ( $headerLine =~ /===== ([^ ]+) =====/ );
	my ($gameX,$gameY) = ( $sizeLine =~ /SIZE:\[(\d+),(\d+)\]/ );

	( <$TURNHANDLE> =~ /~~~~/ ) or die "Gamestate file did not match expected format";

	close $TURNHANDLE;

	return ($gameName,$gameX,$gameY);
}

# Get units from a specific team
sub getUnitsOnTeam
{
	my $theTeam = shift;
	my @allUnits = @_;
	my @myUnits;

	for my $unit (@allUnits)
	{
		my ($unitTeam) = $unit =~ /tm:(\d+)/;
		if ( $unitTeam == $theTeam )
		{
			push(@myUnits,$unit);
		}
	}

	return @myUnits;
}

sub GetTurnFile
{
	my $turn = shift;
	my $turnFile = "Turn_TURN.txt";
	$turnFile =~ s/TURN/$turn/;
	return $turnFile;
}

sub GetCommandFile
{
	my $turn = shift;
	my $team = shift;
	my $cmdFileName = "Turn_TURN_Team_TEAM.txt";
	$cmdFileName =~ s/TURN/$turn/;
	$cmdFileName =~ s/TEAM/$team/;
	return $cmdFileName;
}

# Output the commands file
sub OutputCommandsFile
{
	my $turn = shift;
	my $team = shift;
	my $commands = shift;

	# Get output file
	our $cmdFileName = GetCommandFile($turn,$team);

	if (! -e $cmdFileName)
	{
		open(my $cmdFile, '>', $cmdFileName) or die "Couldn't open '$cmdFileName' $!";
		print $cmdFile $commands;
		close $cmdFile;

		printf "Outputted $cmdFileName\n";
		printf "$commands";
	}
	else
	{
		open(my $cmdFile, '<', $cmdFileName) or die "Couldn't open '$cmdFileName' $!";
		my $old_commands = do { <$cmdFile> };

		printf "Replaying $cmdFileName\n";
		printf "$old_commands";
	}
}

# Print a game map
sub PrintGameMap
{	
	my $gameX = shift;
	my $gameY = shift;
	my @units = @_;

	my @map;

	# Fill with blanks
	for my $x (0 .. $gameX-1)
	{
		for my $y (0 .. $gameY-1)
		{
			$map[$x][$y] = "-";
		}
	}

	# Fill with units
	for my $unit (@units)
	{
		my ($id,$tm,$vs,$dr,$psx,$psy) = getUnit($unit);

		$tm += 31;
		$vs = "\e[".$tm."m".$vs."\e[0m";

		$map[$psx][$psy] = $vs;
	}

	# Print whole map bottom left is 0,0
	for my $y ( reverse 0 .. $gameY-1 )
	{
		for my $x (0 .. $gameX-1)
		{
			printf($map[$x][$y]);
		}
		printf("\n");
	}
}

# Wait for a file to exist
sub WaitForFile
{
	my $file = $_[0];
	while( ! -e $file )
	{
		select(undef, undef, undef, 0.01); 
	}
}

return 1;