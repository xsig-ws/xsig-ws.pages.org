#!/usr/local/bin/perl 


$FILE = "../contents.html";
$BASEURL = 'http://www.hpcc.jp/sacsis/2008/cell/';

@FILESTATS = stat($FILE);
($sec, $min, $hour, $day, $month, $year, $wday) = gmtime( $FILESTATS[9] );
@MONTHNAME = ('Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun',
	      'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec');
@WEEKDAY = ('Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat');

$LASTMOD = sprintf( "%s, %2d %s %d %02d:%02d:%02d GMT",
                    $WEEKDAY[$wday], $day, $MONTHNAME[$month], $year+1900, $hour, $min, $sec);
$DATESTR = sprintf( "%04d-%02d-%02dT%02d:%02d:%02d+00:00",
		    $year+1900, $month+1, $day, $hour, $min, $sec);


open(FILE, $FILE);

binmode(FILE, ":encoding(7bit-jis)");
binmode(STDOUT, ":encoding(utf-8)");



while(<FILE>){
    if( /<dl class="date">/ ){ last; }
}

# %CONTENTS = {};

while(<FILE>){
    if( m!<dt[^>]*>(.*)</dt>! ){
	$DATE = $1;
    }elsif( m!<dd[^>]*>(.*)</dd>! ){
	$CONTENTS{ $DATE } = $1;
    }
}

print_header();

foreach $key (sort keys %CONTENTS ){
    print_item( $key, $CONTENTS{$key} );
}

print_footer();


sub print_header { # -----------------------------------------------
    print<<EOH
Last-Modified: ${LASTMOD}
Content-Type: application/xml; charset=utf-8

<rss version="0.91">

<channel>
 <title>Cell Speed Challenge 2008</title>
 <link>${BASEURL}</link>
 <description>Web Page for Cell Speed Challenge 2008</description>
 <copyright>Cell Speed Challenge 2008 Committee</copyright>
 <language>ja-JP</language>

EOH
}

sub print_item { # --------------------------------------------------
    my($date, $contents) = @_;
    $contents =~ s/<[^>]*>//g;
    print  "<item>\n";
    printf " <title>$date $contents</title>\n";
    printf " <link>$BASEURL</link>\n";
    printf "</item>\n";
}

sub print_footer { # -----------------------------------------------
    print "\n</channel>\n";
    print "</rss>\n\n";
}

