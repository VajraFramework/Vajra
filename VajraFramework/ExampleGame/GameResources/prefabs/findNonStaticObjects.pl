use strict;
use warnings;
use English;

my $dir = './';
foreach my $fp (glob("$dir/*")) {
  open my $fh, "<", $fp or die "can't read open '$fp': $OS_ERROR";
  my $hasMeshRenderer = 0;
  my $isStatic = 0;
  while (<$fh>) {
	  if ($_ =~ /MeshRenderer/) {
		  $hasMeshRenderer += 1;
	  }
	  if ($_ =~ /SetIsStatic/) {
		  $isStatic += 1;
	  }
  }
  if ($isStatic < $hasMeshRenderer) {
	printf "%s\n", $fp;
  }
  close $fh or die "can't read close '$fp': $OS_ERROR";
}

