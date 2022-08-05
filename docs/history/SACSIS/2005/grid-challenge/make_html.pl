#!/usr/bin/perl

$dir = "orig";
$template_path = "$dir/template.html";
$list_path     = "$dir/menu.list";

#------------------------------------------#

opendir(DIR, $dir) or die();

foreach $fn_orig (readdir(DIR)){
  next unless($fn_orig =~ /(.+)\.orig$/i);
  my $fn_html = $1;
  
  my $content = read_file($fn_orig);
  my $menu    = create_menu($fn_html);

  open  OUT, ">$fn_html";
  print OUT apply_template(\$content, \$menu);
  close OUT;
}

closedir(DIR);


sub read_file {
  my $ret;
  open IN, "$dir/$_[0]";
  $ret .= $_ while(<IN>);
  close IN;
  return $ret;
}


sub create_menu {
  my $ret;
  open ML, "$dir/menu.list";
  for(;;){
    my $title = <ML>;
    my $fn = <ML>;
    chomp($title);
    chomp($fn);
    if($title =~ /^>(.+)/){
      $title = $1;
      if($_[0] eq $fn){
        $ret .= "<li><span class=\"menu_h_sub\">$title</span></li>\n";
      } else {
	$ret .= "<li><a href=\"$fn\" class=\"menu_d_sub\">$title</a></li>\n";
      }
    } else {
      if($_[0] eq $fn){
        $ret .= "<li><span class=\"menu_h\">$title</span></li>\n";
      } else {
	$ret .= "<li><a href=\"$fn\" class=\"menu_d\">$title</a></li>\n";
      }
    }
    $_ = <ML>;
    if(/^\+/){ next;}
    elsif (/^-/) {$ret .= "<hr />\n";}
    else {last;}
  }
  return $ret;
}


sub apply_template {
  my $content = ${$_[0]};
  my $menu    = ${$_[1]};
  my $ret;
  open IN, $template_path;
  foreach(<IN>){
    if(/^__MENU__/){
      $ret .= $menu;
    } elsif(/^__CONTENT__/){
      $ret .= $content;
    } else {
      $ret .= $_;
    }
  }
  return $ret;
}
