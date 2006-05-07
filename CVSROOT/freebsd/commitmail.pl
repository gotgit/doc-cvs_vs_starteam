#! /usr/bin/perl -w
#
# $FreeBSD: CVSROOT/freebsd/commitmail.pl,v 1.11 2003/02/04 18:32:34 peter Exp $

$list = shift(@ARGV);
$mailcmd = "/home/majordomo/wrapper resend.nobm -l $list -f owner-$list -h FreeBSD.org";
#$mailcmd = "/usr/sbin/sendmail -oem -f owner-$list";

@names = ();
while ($#ARGV > 0) {
	$access = shift(@ARGV);
	$access2 = shift(@ARGV);

	open(ACCESS, "< $access") || open(ACCESS, "< $access2") || exit 75;
	while (<ACCESS>) {
		chop;
		@words = split;
		if ($words[0] =~ /^[#\/;]/) {
			next;
		}
		if ($words[1]) {
			push @names, $words[1];
		} else {
			push @names, $words[0];
		}
	}
	close(ACCESS);
}
%uniq = ();
grep($uniq{$_} = 1, @names);
$list = join(" ", sort keys %uniq);
if ($list ne '') {
	exec "$mailcmd $list";
	die "cannot exec `$mailcmd': $!";
} else {
	exec "cat >/dev/null";
	die "cannot exec `cat >/dev/null': $!";
}


# Jan 15 09:47:55 hub sendmail[14995]: JAB14995: to=/home/mail/archive/cvs-committers, delay=00:00:00, xdelay=00:00:00, mailer=*file*, stat=Sent
# Jan 15 09:48:00 hub sendmail[14995]: JAB14995: to="|/home/mail/mailtogroup.pl ncvs cvs-committers", delay=00:00:05, xdelay=00:00:05, mailer=prog, stat=unknown mailer error 13

