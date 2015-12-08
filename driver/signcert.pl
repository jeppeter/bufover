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


sub GenRsa($$)
{
	my ($keyfile,$bits)=@_;
	my ($cmd);

	$cmd = "openssl genrsa -out $keyfile $bits";
	RunCmd($cmd);
	return;
}

sub MakeRootCert($$$)
{
	my ($keyfile,$certfile,$days)=@_;
	my ($cmd);
	$cmd = "openssl req -new -x509 -days $days -key $keyfile -out $certfile";
	RunCmd($cmd);
}

sub CreateSubCA($$)
{
	my ($keyfile,$certfile)=@_;
	my ($cmd);
	$cmd = "openssl req -new -key $keyfile -out $certfile";
	RunCmd($cmd);
}

sub SignCA($$$$$)
{
	my ($rootcert,$rootkey,$signca,$days,$outcert) = @_;
	my ($cmd);
	$cmd = "openssl x509 -req -days $days -in $signca -CA $rootcert -CAkey $rootkey -set_serial 01 -out $outcert";
	RunCmd($cmd);
}

my ($rootkey,$rootcert,$signkey,$signcert,$outcert,$days,$bits);
$rootkey="root.key";
$rootcert="root.crt";
$days = 365*3;
$signkey="sign.key";
$signcert = "sign.csr";
$outcert = "out.crt";
$bits=4096;

GenRsa($rootkey,$bits);
MakeRootCert($rootkey,$rootcert,$days);
GenRsa($signkey,$bits);
CreateSubCA($signkey,$signcert);
SignCA($rootcert,$rootkey,$signcert,$days,$outcert);