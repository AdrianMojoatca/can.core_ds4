#!/usr/local/bin/perl


my $config_status_file          = '..\common\com.inc.config.fw\config_status.h';
my $config_status_file_template = '..\common\core\inc.config.template\com.inc.config.status\config_status.h';

my $CORE_CONFIG_STATUS_VERSION  = "CORE_CONFIG_STATUS_VERSION";

use constant CONFIG_STATUS_VERSION_IN_PRJ => "1_00";
use constant CONFIG_STATUS_VERSION_IN_TEMPLATE => "2_00";
use constant M2M_SECTOR_BEG => "M2M Status";
use constant M2M_SECTOR_END => "#endif";
use constant CONFIG_STATUS_END => "#endif";

sub updateToNewVersion
{
    #open file in template, get the m2m sector
    open (INFILE, $config_status_file_template) or die $config_status_file_template . ' not found';
    $i = 0;
    $m2m_appear = 0;
    while ($x=<INFILE>)
    {
        $y = M2M_SECTOR_BEG;
        if ($x =~ /$y/)
        {
            $m2m_appear = 1;
        }

        if ($m2m_appear == 1)
        {
            $m2m_lines[$i] = $x;
            $i++;
        }
        # include the last line of "endif"
    }
    close(INFILE);

    # open file in project, (1) replace the version number; (2) insert the m2m lines from file in template
    # new material in new array
    open (INFILE, $config_status_file) or die $config_status_file . ' not found';
    $j=0;
    while ($x=<INFILE>)
    {
        $y = $CORE_CONFIG_STATUS_VERSION;
        if ($x =~ /$y/)
        {
            # replace the version number
            my $search = CONFIG_STATUS_VERSION_IN_PRJ;
            my $replace = CONFIG_STATUS_VERSION_IN_TEMPLATE;
            $x =~ s/$search/$replace/;
        }
        $z = CONFIG_STATUS_END;
        if ($x =~ /$z/)
        {
            $last_config_line = $j;
        }
        $new_lines[$j] = $x;
        $j++;
    }
    close(INFILE);
    $j = $last_config_line;  # don't count the last line
    # insert the m2m line to new lines
    foreach $x (@m2m_lines)
    {
        $new_lines[$j] = $x;
        $j++;
    }

    # write back the new materials
    open(OUTFILE, '>' . $config_status_file) or die $config_status_file . ' not found';
    foreach $x (@new_lines)
    {
        print OUTFILE $x;
    }
    print OUTFILE "\n";  #newline for keil IDE
    close(OUTFILE);
}

sub platform_933
{
    if(open(INFILE, $config_status_file))
    {
        chmod 0666, $config_status_file;

        #check for version numbers
        while ($x=<INFILE>)
        {
            $y = $CORE_CONFIG_STATUS_VERSION;
            if($x =~ /$y/)
            {
                @a = split(/_/,$x);
                $ver_in_prj=$a[4]."_".$a[5];    #version number
            }
        }
        close (INFILE);

        chmod 0666, $config_status_file_template;

        open (INFILE, $config_status_file_template) or die $config_status_file_template . ' not found';
        while ($x=<INFILE>)
        {
            $y = $CORE_CONFIG_STATUS_VERSION;
            if($x =~ /$y/)
            {
                @a = split(/_/,$x);
                $ver_in_template = $a[4]."_".$a[5]; #version number
            }
        }
        close (INFILE);

        if($ver_in_template == $ver_in_prj)
        {
            #equal, no need update
            die "\n version match\n";
        }
        else
        {
            #version not match, update the version in project with version in template.
            if($ver_in_template == CONFIG_STATUS_VERSION_IN_TEMPLATE)
            {
                updateToNewVersion();
            }
        }
    }
    else { die "#error: Could not find config_status_file.h\n"; }
}

if ( @ARGV < 1 )
{
    die "#error: No arguments.";
}
my $project = $ARGV[0] ;

if($project eq "933")
{
    platform_933();
}
else
{
    die "#error: bad project. Use 933 ONLY";
}
