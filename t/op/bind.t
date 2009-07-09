#!./perl -w
# tests state variables

BEGIN {
    chdir 't' if -d 't';
    @INC = '../lib';
    require './test.pl';
}

use strict;
use feature ":5.11";

plan tests => 12;

{
    my $x = '';
    my $y;
    $y := $x;
    ok defined($y), '$y binds to $x';
    $x++;
    ok $y, 'changing $x changes $y';
}

{
    my $x = '';
    my $y := $x;
    ok defined($y), 'my $y binds to $x';
    $x++;
    ok $y, 'changing $x changes $y';
}

{
    my @x = (1);
    my @y;
    @y := @x;
    ok $y[0], '@y binds to @x';
    push @x, 2;
    ok $y[1], 'changing @x changes @y';
}

{
    my @x = (1);
    my @y := @x;
    ok $y[0], 'my @y binds to @x';
    push @x, 2;
    ok $y[1], 'changing @x changes @y';
}

{
    my %x = (a => 1);
    my %y;
    %y := %x;
    ok $y{a}, '%y binds to %x';
    $x{b} = 2;
    ok $y{b}, 'changing %x changes %y';
}

{
    my %x = (a => 1);
    my %y := %x;
    ok $y{a}, 'my %y binds to %x';
    $x{b} = 2;
    ok $y{b}, 'changing %x changes %y';
}
