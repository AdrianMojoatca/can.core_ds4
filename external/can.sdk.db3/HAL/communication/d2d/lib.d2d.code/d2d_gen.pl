#! perl

# script to create d2d files in c or asm, in short stripped down version
# or in long normal version.

# CALL arguments :
# perl script_name.pl generator_file.gen (FULL or STRIPPED) (C or ASM)

use 5.8.8;

#use strict;
#use warnings;

# First, we'll set up the variables we want to use.
my $arg_count = @ARGV ;
if ($arg_count != 3)
{
    die "Arguments are : file to open, type of format (FULL or STRIPPED), type of language (ASM or C). Program Stopped" ;
}
my ($infile, $format, $lang_type) = @ARGV;    # these are arguments
my $outfile_fh ;

my $dir = get_directory ($infile) ;

my $infile_fh  = open_infile  ($infile) ;

if ($lang_type =~ /[aA][sS][mM]/)
{
    $outfile_fh = open_outfile ($dir, "d2d_code.asm") ;
    $lang_type = 'ASM' ;
}
elsif ($lang_type =~ /[cC]/)
{
    $outfile_fh = open_outfile ($dir, "d2d_code.h") ;
    $lang_type = 'C' ;
}
else
{
    die "File Extension not supported, program stopped" ;
}


while (<$infile_fh>)
{
    process_line ($_) ;
}

close $infile_fh ;
close $outfile_fh ;


sub open_infile 
{
    my ($infile) = @_;
    open my $infile_fh, '<', $infile or die "Can't open '$infile': $!";
    return $infile_fh;
}

sub open_outfile 
{
    my ($dir, $outfile) = @_;
 
    $outfile = join ("", $dir, $format, "/", $outfile) ;
    open my $outfile_fh, '>', $outfile or die "Can't create/open '$outfile': $!";
    return $outfile_fh;
}

sub get_directory 
{
    my $rev_infile = reverse @_ ;
    my $index      = index   $rev_infile, '/' ;
    
    if ($index == -1)
    { 
        return "";
    }
    my $directory  = substr  $rev_infile, $index ;
    $directory     = reverse $directory ;
    return $directory ;
}

sub process_line 
{                                                     
    my ($line) = @_;
    
    if ($line =~ /\@:FULL/)
    {
        process_more_lines ("FULL") ;
    }
    elsif ($line =~ /\@:STRIPPED/)
    {
        process_more_lines ("STRIPPED") ;
    }
    else
    {
        process_sub_line ($line) ;   
    }
}

sub process_sub_line
{
    my ($line) = @_;
    
    if ($lang_type eq 'ASM')
    {
        $line =~ s/@@/;/    ;
        $line =~ s/\@A/EQU/ ;
        $line =~ s/\@C//    ;        
    }
    else
    {
        $line =~ s/@@/\/\//     ; 
        $line =~ s/\@C/#define / ;
        $line =~ s/\@A//        ; 
    }  
    print $outfile_fh $line ;
}

sub process_more_lines
{
    my ($format_read) = @_ ;
    my $line ;
    
    while ()
    {
        $line = <$infile_fh> ;
        
        if ($line =~ /\@;/)
        {
            return ;
        }
        if ($format_read =~ /$format/i)
        {
            process_sub_line ($line) ;
        }
    }
}
