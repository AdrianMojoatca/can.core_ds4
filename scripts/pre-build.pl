#!/usr/local/bin/perl

use strict;
use FindBin;

my $cygwin_path = 'c:\cygwin\bin\perl.exe';
my $template_copy_path = $FindBin::Bin . '/templates_copy.pl';
my $config_gpio_path = $FindBin::Bin . '/config_gpio.pl';
my $config_d2di_exe = $FindBin::Bin . '/D2DiConfigFile.exe';
my $update_config_status = $FindBin::Bin . '/update_config_status.pl';

my $common = "common";

sub platform_402
{
    system($cygwin_path , $template_copy_path, @_[0]);
    system($cygwin_path , $template_copy_path, $common);
}

sub platform_403
{
    system($cygwin_path , $template_copy_path, @_[0]);
    system($cygwin_path , $template_copy_path, $common);
    system($config_d2di_exe);
}

sub platform_404
{
    system($cygwin_path , $template_copy_path, @_[0]);
    system($cygwin_path , $template_copy_path, $common);
    system($config_d2di_exe);
}

sub platform_800
{
    system($cygwin_path , $template_copy_path, @_[0]);
    system($cygwin_path , $template_copy_path, $common);
    system($cygwin_path , $config_gpio_path, @_[0]);
}

sub platform_933
{
    system($cygwin_path , $template_copy_path, @_[0]);
    system($cygwin_path , $template_copy_path, $common);
    system($cygwin_path , $config_gpio_path, @_[0]);
    system($cygwin_path , $update_config_status, @_[0]);
}


if ( @ARGV < 1 )
{
    die "#error: No arguments. Use dball, cm800_v1, 800 or 933";
}

my $project = $ARGV[0] ;

if($project eq "402")
{
    platform_402($project);
}

elsif($project eq "403")
{
    platform_403($project);
}

elsif($project eq "404")
{
    platform_404($project);
}

elsif($project eq "800")
{
    platform_800($project);
}
elsif($project eq "933")
{
    platform_933($project) ;
}
else
{
    die "#error: Did not enter a valid project (402, 403, 800 or 933)." . $project . "\n";
}
