#! /usr/bin/perl
use strict;
use warnings;

our $ttrts_perlai_versioncompat_major = 0;
our $ttrts_perlai_versioncompat_minor = 3;

our $headerDelimiter="~~~~";

our $VERBOSE = $ENV{"VERBOSE"};

# Format of the a gamestate header
our $headerFormatter = qr/==== ttrts v(\d+)\.(\d+)\.(\d+)+ ====
NAME:(.+)
SIZE:\[(\d+),(\d+)\]
TURN:(\d+)
(WALL:.*?)
$headerDelimiter/;

# Formatter for coords
our $coordFormatter = qr/\[\d+,\d+\]/;

# Format of a unit descriptor
our $unitFormatterNonCapture = qr/UNIT:\d+ pl:\d+ vs:[^ ]+ dr:[^ ]+ ps:\[\d+,\d+\]\n?/;

# Format of a unit descriptor
our $unitFormatter = qr/UNIT:(\d+) pl:(\d+) vs:([^ ]+) dr:([^ ]+) ps:\[(\d+),(\d+)\]\n?/;

# Get x and y
sub getPositionsXandYString
{
	return (shift =~ /\[(\d+),(\d+)\]/);
}

# Get all positions
sub getPositionStringsFromLine
{
	return (shift =~ /$coordFormatter/gm );
}

# Get information about a unit from it's descriptor
sub getUnitInfo
{
	return (shift =~ /$unitFormatter/);
}

# Get set of units from a string
sub GetUnitStringsFromGamestate
{
	my $gamestate = shift;

	my @units = ( $gamestate =~ /$unitFormatterNonCapture/gm );

	foreach my $unit (@units)
	{
		chomp($unit);
	}

	return @units;
}

# in the format $major,$minor,$patch,$name,$sizex,$sizey,$turn,$invalidpositions+
sub GetGameInfoFromGamestate
{
	my $header = shift;
	(! defined $header) and die "GetGameInfoFromGamestate was not passed valid header parameter";

	my @info = ($header =~ /$headerFormatter/ );

	return @info;
}

# Get the units from a turn file
sub GetUnitStringsFromFile
{
	my $turnFile = shift or die "GetUnitStringsFromFile needs file parameter";

	# Read in the whole file method from http://www.perlmonks.org/?node_id=1952
	my $text;
	{
	  local $/=undef;
	  open FILE, $turnFile or die "Couldn't open file: $!";
	  $text = <FILE>;
	  close FILE;
	}

	return GetUnitStringsFromGamestate($text);
}

# Check version numbers against ttrts.pm version
sub verifyVersion
{
	my $version_major = shift;
	(! defined $version_major) and die "verifyVersion needs version_major parameter";
	my $version_minor = shift;
	(! defined $version_minor) and die "verifyVersion needs version_minor parameter";
	if( ($version_major != $ttrts_perlai_versioncompat_major) 
	or  ($version_minor != $ttrts_perlai_versioncompat_minor) ) 
	{
		printf "ttrts.pm version does not match with this ttrts version\n";
		die "ttrts.pm = v$ttrts_perlai_versioncompat_minor.$ttrts_perlai_versioncompat_major ttrts = v$version_major.$version_minor";
	}
}


# Get information from the header for this turn
sub GetGameInfoFromFile
{
	my $turnFile = shift or die "GetGameInfoFromFile needs turnFile parameter";

	# Read in the whole file method from http://www.perlmonks.org/?node_id=1952
	my $text;
	{
	  local $/=undef;
	  open FILE, $turnFile or die "Couldn't open file: $!";
	  $text = <FILE>;
	  close FILE;
	}

	my @info = GetGameInfoFromGamestate($text);
	verifyVersion @info;

	return @info;
}

# Get units from a specific player
sub GetPlayerUnits
{
	my $thePlayer = shift;
	(! defined $thePlayer) and die "GetPlayerUnits needs player parameter";
	my @allUnits = @_;
	(! @allUnits) and die "GetPlayerUnits needs units parameters";
	my @myUnits;

	for my $unit (@allUnits)
	{
		my ($unitplayer) = $unit =~ /pl:(\d+)/;
		if ( $unitplayer == $thePlayer )
		{
			push(@myUnits,$unit);
		}
	}

	return @myUnits;
}

sub GetTurnFileName
{
	my $turn = shift;
	(! defined $turn) and die "GetTurnFileName needs turn parameter";
	my $turnFile = "Turn_TURN.txt";
	$turnFile =~ s/TURN/$turn/;
	return $turnFile;
}

sub GetCommandFileName
{
	my $turn = shift;
	(! defined $turn) and die "GetCommandFileName needs turn parameter";
	my $player = shift;
	(! defined $player) and die "GetCommandFileName needs player parameter";
	my $cmdFileName = "Player_PLAYER_Turn_TURN.txt";
	$cmdFileName =~ s/TURN/$turn/;
	$cmdFileName =~ s/PLAYER/$player/;
	return $cmdFileName;
}

# Output the commands file
sub OutputCommandsFile
{
	my $turn = shift;
	(! defined $turn) and die "OutputCommandsFile needs turn parameter";
	my $player = shift;
	(! defined $player) and die "OutputCommandsFile needs player parameter";
	my $commands = shift or die "OutputCommandsFile needs commands parameter";

	# Get output file
	our $cmdFileName = GetCommandFileName($turn,$player);

	if (! -e $cmdFileName)
	{
		open(my $cmdFile, '>', $cmdFileName) or die "Couldn't open '$cmdFileName' $!";
		print $cmdFile $commands;
		print $cmdFile "END";
		close $cmdFile;

		$VERBOSE and printf "Outputted $cmdFileName\n";
		printf "$commands";
	}
	else
	{
		# Read in the whole file method from http://www.perlmonks.org/?node_id=1952
		my $text;
		{
		  local $/=undef;
		  open FILE, $cmdFileName or die "Couldn't open file: $!";
		  $text = <FILE>;
		  close FILE;
		}

		$text =~ s/\nEND//;

		printf "Replaying $cmdFileName\n";
		printf "$text\n";
	}
}

# Print a game map
sub PrintGameFromGamestateString
{	
	my $gamestateString = shift or die "PrintGameFromGamestateString needs string parameter";

	my @info = GetGameInfoFromGamestate($gamestateString);
	my @units = GetUnitStringsFromGamestate($gamestateString);

	# $major,$minor,$patch,$name,$sizex,$sizey,$turn,$invalidpositions+
	my $gameX = $info[4];
	my $gameY = $info[5];

	# Shift into info to where the invalid positions are stored
	my @invalids = getPositionStringsFromLine($info[7]);

	my @map;

	# Fill with blanks
	for my $x (0 .. $gameX-1)
	{
		for my $y (0 .. $gameY-1)
		{
			$map[$x][$y] = "-";
		}
	}

	# Fill in all invalid coordinates
	foreach my $coord (@invalids)
	{
		my @invalidPos = getPositionsXandYString($coord);
		$map[$invalidPos[0]][$invalidPos[1]] = "~";
	}

	# Fill with units
	for my $unit (@units)
	{
		my ($id,$pl,$vs,$dr,$psx,$psy) = getUnitInfo($unit);

		$pl += 31;
		$vs = "\e[".$pl."m".$vs."\e[0m";

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

# Print a game map
sub PrintGameFromFile
{
	my $turnFile = shift or die "PrintGameFromFile needs file parameter";

	# Read in the whole file method from http://www.perlmonks.org/?node_id=1952
	my $text;
	{
	  local $/=undef;
	  open FILE, $turnFile or die "Couldn't open file: $!";
	  $text = <FILE>;
	  close FILE;
	}

	PrintGameFromGamestateString($text);
}

# Print a turn
sub PrintGameMapForTurn
{
	my $turn = shift;
	(! defined $turn) and die "PrintGameMapForTurn needs turn parameter";
	$turn = GetTurnFileName($turn);
	PrintGameFromFile( $turn );
}

# Wait for a file to exist
sub WaitForFile
{
	my $file = shift or die "WaitForFile needs file parameter";
	while( ! -e $file )
	{
		select(undef, undef, undef, 0.01); 
	}
}

return 1;