#!/usr/bin/perl
# for English Menu
open IN, "menu.list" or die;
open OUT, ">menu.shtml" or die;

print OUT << "_END_";
<!-- Don't edit this file!
If you want to add some menu, edit "menu.list" and run "perl gen_menu.pl" on this directory. -->  
<ul class="menu">
_END_

for(;;){
  my $title = <IN>;
  my $fn = <IN>;
  chomp($title);
  chomp($fn);

  my $fn_shtml = $fn;
  $fn_shtml =~ s/.html$/.shtml/;
  print OUT << "_END_";
<li>
<!--#if expr="\$DOCUMENT_NAME = $fn_shtml" -->
<span class="menu_h">$title</span>
<!--#else -->
<a href="$fn" class="menu_d">$title</a>
<!--#endif -->
</li>
_END_
  last unless(<IN> eq "+\n");
}
print OUT << "_END_";
</ul>
_END_

    
close IN;
close OUT;

