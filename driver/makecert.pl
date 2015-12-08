#!/usr/bin/perl

use strict;

sub DebugString($)
{
	my ($s) = @_;
	my ($f,$l,$p);
	my ($fmt);

	($p,$f,$l) = caller(1);
	$fmt = "$f:$l $s\n";
	print STDOUT $fmt;
	return;
}

sub RunCmd($)
{
	my ($cmd) = @_;
	my ($ret);
	DebugString("run cmd ($cmd)");
	$ret = system($cmd);
	if ($ret != 0 ){
		die "can not run($cmd)";
	}
	return;
}


sub CreateRootCA($$$)
{
	my ($pemname,$days,$bits)= @_;
	my ($cmd);

	$cmd = "openssl req -x509 -nodes -days $days -newkey rsa:$bits -keyout $pemname -out $pemname";
	RunCmd($cmd);
	return ;
}

sub CreatePfx($$)
{
	my ($pfxfile,$pemfile,$desc) = @_;
	my ($cmd);

	$cmd = "openssl pkcs12 -export -out $pfxfile -in $pemfile -name '$desc'";
	RunCmd($cmd);
	return;
}


my ($pemfile,$pfxfile,$desc,$days,$bits);

$pemfile="root.pem";
$pfxfile="root.pfx";
$desc="root description";
$days = 365*3;
$bits = 4096;

CreateRootCA($pemfile,$days,$bits);
CreatePfx($pfxfile,$pemfile);
