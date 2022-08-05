#!/usr/local/bin/perl

while (<>) {
  chomp;
  $path = $_;
  if ($path =~ /\.txt$/) {
    local(*IF, *OF);
    $h = $path;
    $h =~ s/\.txt$/\.html/;
    if (open(OF, "> $h") && open(IF, "$path")) {
      print OF qq{<html><body bgcolor="white"><pre>\n};
      while (<IF>) {
        print OF;
      }
      print OF qq{</pre></body></html>\n};
      print STDERR qq{$path => $h\n};
      $path = $h;
      close(IF);
      close(OF);
    }
  }
  $fname = &get_basename($path);
  $fname =~ s/\.html$//;
  print qq{<a href="$path">$fname</a> /\n};
}

sub get_basename {
  my ($f) = shift or return "";
  $f =~ s:.*/([^/]+)$:$1:;
  return $f;
}

