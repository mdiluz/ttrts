#! /usr/bin/perl
use strict;
use warnings;

our $ttrts_perlai_versioncompat_major = 0;
our $ttrts_perlai_versioncompat_minor = 1;

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
	my $num = 0;
	while( !( <$TURNHANDLE> =~ /~~~~/ ) )
	{
		$num++;
		$num > 20 and die "gamestate file did not reach ~~~~ line within 10 lines";
	} 

	my @units;
	while( my $unitLine = <$TURNHANDLE> )
	{
		chomp $unitLine;
		if( !($unitLine eq "END") )
		{
			push(@units,$unitLine);
		}
	}

	close $TURNHANDLE;

	return @units;
}

# Check version numbers against ttrts.pm version
sub checkVersion
{
	my $version_major = shift;
	my $version_minor = shift;
	if( ($version_major != $ttrts_perlai_versioncompat_major) 
	or  ($version_minor != $ttrts_perlai_versioncompat_minor) ) 
	{
		printf "ttrts.pm version does not match with this ttrts version\n";
		die "ttrts.pm = v$ttrts_perlai_versioncompat_minor.$ttrts_perlai_versioncompat_major ttrts = v$version_major.$version_minor";
	}
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
	my $nameLine = <$TURNHANDLE>;
	chomp $nameLine;
	my $sizeLine = <$TURNHANDLE>;
	chomp $sizeLine;
	my $turnLine = <$TURNHANDLE>;
	chomp $turnLine;

	my ($version_major,$version_minor) = ( $headerLine =~ /==== ttrts v(\d+)\.(\d+)\.\d+ ====/ );
	checkVersion $version_major,$version_minor;

	my ($gameName) = ( $nameLine =~ /NAME:(.+)/ );
	my ($gameX,$gameY) = ( $sizeLine =~ /SIZE:\[(\d+),(\d+)\]/ );

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
		print $cmdFile "END";
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