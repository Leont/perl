#!/usr/bin/perl -w
#
# regen.pl - a wrapper that runs all *.pl scripts to to autogenerate files

require 5.003;	# keep this compatible, an old perl is all we may have before
                # we build the new one

# The idea is to move the regen_headers target out of the Makefile so that
# it is possible to rebuild the headers before the Makefile is available.
# (and the Makefile is unavailable until after Configure is run, and we may
# wish to make a clean source tree but with current headers without running
# anything else.

use strict;
my $perl = $^X;

# keep warnings.pl in sync with the CPAN distribution by not requiring core
# changes.  Um, what ?
# safer_unlink ("warnings.h", "lib/warnings.pm");

# Which scripts to run. Note the ordering: embed.pl must run after
# opcode.pl, since it depends on pp.sym, and autodoc.pl should run last as
# it reads all *.[ch] files, some of which may have been changed by other
# scripts (eg reentr.c)

my @scripts = qw(
keywords.pl
opcode.pl
overload.pl
reentr.pl
regcomp.pl
warnings.pl

embed.pl
autodoc.pl
);

# Which files are (re)generated by each script.
# *** We no longer need these values, as the "changed" message is
# now generated by regen_lib.pl, so should we just drop them?

my %gen = (
	   'autodoc.pl'  => [qw[pod/perlapi.pod pod/perlintern.pod]],
	   'embed.pl'    => [qw[proto.h embed.h embedvar.h global.sym
				perlapi.h perlapi.c]],
	   'keywords.pl' => [qw[keywords.h]],
	   'opcode.pl'   => [qw[opcode.h opnames.h pp_proto.h pp.sym]],
	   'regcomp.pl'  => [qw[regnodes.h]],
	   'warnings.pl' => [qw[warnings.h lib/warnings.pm]],
	   'reentr.pl'   => [qw[reentr.c reentr.h]],
	   'overload.pl' => [qw[overload.c overload.h lib/overload/numbers.pm]],
	   );

sub do_cksum {
    my $pl = shift;
    my %cksum;
    for my $f (@{ $gen{$pl} }) {
	local *FH;
	if (open(FH, $f)) {
	    local $/;
	    $cksum{$f} = unpack("%32C*", <FH>);
	    close FH;
	} else {
	    warn "$0: $f: $!\n";
	}
    }
    return %cksum;
}

foreach my $pl (@scripts) {
  my @command =  ($^X, $pl, @ARGV);
  print "@command\n";
  system @command;
}
