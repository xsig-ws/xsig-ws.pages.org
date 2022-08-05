#!/usr/bin/perl
# make-pic.pl pic1-command dir prob-no #pic-X #pic-Y pic-width pic-height span-flag

# $(O$3$l$O(Bmake-pic1$(O$r;H$C$F!"A42hA|$r%m!<%+%k$K:n$k%3%^%s%I!#(B
# $(OJ,;6:n@.$N$H$-$O(Bgendata.py$(O$J$I$r;H$&$3$H!#(B

if ($#ARGV+1 != 8) {
    printf("Specify pic1-command dir prob-no num-pic-X num-pic-Y pic-width pic-height span-flag\n");
    exit(1);
}

$pic1com = $ARGV[0];
$dir = $ARGV[1];
$prob = $ARGV[2];
$nx = $ARGV[3];
$ny = $ARGV[4];
$w = $ARGV[5];
$h = $ARGV[6];
$span = $ARGV[7];

system("mkdir -p $dir");

for ($iy = 0; $iy < $ny; $iy++) {
    for ($ix = 0; $ix < $nx; $ix++) {
	$fn = sprintf("%s/%02d%03d%03d.mdt", $dir, $prob, $ix, $iy);
	system("$pic1com $fn $nx $ny $w $h $span");
	printf(".");
    }
}
printf("\n");

