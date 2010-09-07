#!/usr/bin/perl

open FIN , "< idmef_create.h.in" || exit 1;
open FOUT, "> idmef_create.h"    || exit 1;

while(<FIN>)
{
	unless(/\@input\@/)
	{
		print FOUT;
		next;
	}

	open FD, "< ../libidmef-functions.map" || exit 1;
	while(<FD>)
	{
		next if /^#/;

		if(/([\d\w_-]+)\s+([\d\w_-]+)\s+([\d\w_-]+)/)
		{
			$func = $1;
			$elem = $2;
			$type = $3;
		}
		else
		{
			next
		}

		if($type =~ /DOC/ || $type =~ /NODE/)
		{
			print FOUT "IDMEFPtr $func(IDMEFPtr first, ...);\n"
		}
		elsif($type =~ /CONTENT/ || $type =~ /ATTRIBUTE/)
		{
			print FOUT "IDMEFPtr $func(const char *first, ...);\n"
		}
	}
	close FD;
}
close FIN;
close FOUT;

0;
