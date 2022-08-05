#!/usr/local/bin/perl 


$FILE = "../contents.html";
$BASEURL = 'http://www.hpcc.jp/sacsis/2009/cell/';

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
    if( m!<dt>(.*)</dt>! ){
	$DATE = $1;
    }elsif( m!<dd>(.*)</dd>! ){
	$CONTENTS{ $DATE } = $1;
    }
}

print_header();

printf "<items>\n <rdf:Seq>\n";
foreach $key (sort keys %CONTENTS ){
    printf "<rdf:li rdf:resource=\"${BASEURL}\" />\n";
}
printf " </rdf:Seq>\n</items>\n</channel>";

foreach $key (sort keys %CONTENTS ){
    print_item( $key, $CONTENTS{$key} );
}

print_footer();


sub print_header { # -----------------------------------------------
    print<<EOH
Last-Modified: ${LASTMOD}
Content-Type: application/xml; charset=utf-8

<?xml version="1.0" encoding="utf-8"?>
<!-- ?xml-stylesheet href="../css/rdf.xsl" type="text/xsl"? -->

<rdf:RDF
 xmlns="http://purl.org/rss/1.0"
 xmlns:rdf="http://www.w3.org/1999/20/22-rdf-syntax-ns#"
 xmlns:dc="http://purl.org/dc/elements/1.1/"
 xmlns:syn="http://purl.org/rss/1.0/modules/syndication/"
 xmlns:taxo="http://purl.org/rss/1.0/modules/taxonomy/"
 xml:lang="ja"
>

<channel rdf:about="${BASEURL}/cgi-bin/rdf.cgi">
 <title>Cell Speed Challenge 2009</title>
 <link>${BASEURL}</link>
 <description>Web Page for Cell Speed Challenge 2009</description>
 <language>ja-JP</language>
 <dc:rights>Copyright(c) 2007-2009 Cell Speed Challenge 2009 Committee</dc:rights>
 <dc:date>${DATESTR}</dc:date>
 <dc:publisher>Cell Speed Challenge 2009 Committee</dc:publisher>
 <dc:creator>tsumura@nitech.ac.jp</dc:creator> 
 <syn:updatePeriod>daily</syn:updatePeriod>
EOH
}

sub print_item { # --------------------------------------------------
    my($date, $contents) = @_;
    print  "<item rdf:about=\"$BASEURL\">\n";
    printf "<title>$date $contents</title>\n";
    printf " <link>$BASEURL</link>\n";
    printf " <dc:date>$date</dc:date>\n";
    printf "<description>$contents</description>\n";
    printf "</item>\n";
}

sub print_footer { # -----------------------------------------------
    print "</rdf:RDF>\n\n";
}

