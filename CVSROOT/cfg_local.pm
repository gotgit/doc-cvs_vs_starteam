# $FreeBSD: CVSROOT-src/cfg_local.pm,v 1.26 2003/02/28 21:17:06 peter Exp $

####################################################################
####################################################################
# This file contains local configuration for the CVSROOT perl
# scripts.  It is loaded by cfg.pm and overrides the default
# configuration in that file.
#
# It is advised that you test it with
#     'env CVSROOT=/path/to/cvsroot perl -cw cfg.pm'
# before you commit any changes.  The check is to cfg.pm which
# loads this file.
####################################################################
####################################################################

$CHECK_HEADERS = 0;
$IDHEADER = 'FreeBSD';
$UNEXPAND_RCSID = 1;

%TEMPLATE_HEADERS = (
	"Reviewed by"		=> '.*',
	"Submitted by"		=> '.*',
	"Obtained from"		=> '.*',
	"Approved by"		=> '.*',
	"PR"			=> '.*',
	"MFC after"		=> '\d+(\s+(days?|weeks?|months?))?'
);

$MAILCMD = "/usr/local/bin/mailsend -H";
$MAIL_BRANCH_HDR  = "X-CVS-Branch";
$ADD_TO_LINE = 0;
#$MAILBANNER = "My repository";
#if (defined $ENV{'CVS_COMMIT_ATTRIB'}) {
#  my $attrib = $ENV{'CVS_COMMIT_ATTRIB'};
#  $MAILBANNER .= " ($attrib committer)";
#}
# The minimum version of cvs that we will work with.
$MINCVSVERSION = "1110200";  # 1.11.2

$MAIL_ON_DIR_CREATION = 0;

# Sanity check to make sure we've been run through the wrapper and are
# now primary group 'ncvs'.
#

#$COMMITCHECK_EXTRA = sub {
#	my $GRP=`/usr/bin/id -gn`;
#	chomp $GRP;
#	unless ( $GRP =~ /^ncvs$/ ) {
#		print "You do not have group ncvs (commitcheck)!\n";
#		exit 1;	# We could return false here.  But there's
#			# nothing to stop us taking action here instead.
#	}
#	return 1;
#};

# Wrap this in a hostname check to prevent mail to the FreeBSD
# list if someone borrows this file and forgets to change it.
my $hostname = `/bin/hostname`;
die "Can't determine hostname!\n" if $? >> 8;
chomp $hostname;
if ($hostname =~ /^repoman\.freebsd\.org$/i) {
	#$MAILADDRS='src-committers@FreeBSD.org cvs-src@FreeBSD.org cvs-all@FreeBSD.org';
	#$MAILADDRS = 'cvs@FreeBSD.org' if $DEBUG;

	@COMMIT_HOSTS = qw(repoman.freebsd.org);
}


@LOG_FILE_MAP = (
	'CVSROOT'	=> '^CVSROOT/',

	'main.log'		=> '.*'
);
# CVSROOT is still shared between src, ports, doc at the moment. projects has
# its own CVSROOT.
@MAIL_MAP = (
	'maillist1' => '^CVSROOT/',
	'maillist2' => '^src/',
	'cvsnone' => '.*',
); 

@TAG_MAP = (
	'jiangxin' => '^(milestone|rel).*',
); 

@TAG_ACCESS_MAP = (
	'null' => '^(Frozen-Tag|Frozen-Branch)$',
	'jiangxin' => '.*',
); 

# Email addresses of recipients of commit mail.
#$MAILADDRS = 'nobody';
$MAILADDRS = 'cvsnone';

1; # Perl requires all modules to return true.  Don't delete!!!!
#end
