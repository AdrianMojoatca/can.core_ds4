#!/usr/local/bin/perl

my $DEI_FEATURE_VERSION  = "DEI_FEATURE_VERSION";
my $NGMM_FEATURE_VERSION = "NGMM_FEATURE_VERSION";
my $config_file     = '.\inc.config_fw\config_800_gpio.h';
my $config_file_933 = '.\inc.config_fw\config_933_gpio.h';
my $dei_file        = '.\inc.config_fw\config_dei_feature.h';
my $file_933        = '.\inc.config_fw\config_ngmm_feature.h';

use constant PROG_OUT_NOT_CONFIG_OLD => "20";
use constant PROG_OUT_NOT_CONFIG_NEW => "22";

sub platform_800
{
    if(open(INFILE, $config_file))
    {
        chmod 0666, $config_file;
        #chmod 0666, $as_file;
        chmod 0666, $dei_file;

        #check for version numbers
        while ($x=<INFILE>)
        {
            $y = $DEI_FEATURE_VERSION;
            if($x=~/$y/)
            {
                @a = split(/ +/,$x);
                $deiver=$a[2];
            }
        }
        close (INFILE);

        open (INFILE, $dei_file) or die $dei_file . ' not found';
        while ($x=<INFILE>)
        {
            $y = $DEI_FEATURE_VERSION;
            if($x=~/$y/)
            {
                @a = split(/ +/,$x);
                $deiverCore=$a[2];
            }
        }
        close (INFILE);

        if($deiver!=$deiverCore)
        {
            print "\n#error version mismatch\n";
            #READ DEI, 933 AND AS CONFIG TO TMP STORAGE
            open (INFILE, $dei_file) or die "file not found";
            $i=0;
            while ($x=<INFILE>)
            {
                $dei[$i]=$x;
                $i++;
            }
            close (INFILE);

            #WRITE BACK DEI, AS AND 933 CONFIG WITH CHANGES
            open (OUTFILE, '>' . $dei_file) or die $dei_file . "file not found";
            foreach $x (@dei)
            {
                $y=$DEI_FEATURE_VERSION;
                if($x=~/$y/)
                {
                    if($deiver!=$deiverCore)
                    {
                        $x=$x."\n #error \"". $DEI_FEATURE_VERSION . " number mismatch\"";
                    }
                }
                print OUTFILE $x;
            }
            close(OUTFILE);

        }
        else
        {
            #READ FROM USR CONFIG
            open (INFILE, $config_file) or die $config_file . "file not found";
            $i=31;
            while ($x=<INFILE>)
            {
                $y = "FEAT_DEI_SYS".$i;
                if($x =~ /$y/)
                {
                    if($x =~ /_ENABLE/)
                    {
                        @a = split(/0x/,$x);
                        $io_e[$i]= $a[1];
                    }
                    else
                    {
                        @a = split(/0x/,$x);
                        $io[$i]= $a[1];
                        $i++;
                    }
                }
            }
            close (INFILE);

            #COMPARE WITH SVN CONFIG
            open (INFILE, $dei_file)or die $dei_file . "file not found";
            $flag=0;
            $i=31;
            while ($x=<INFILE>)
            {
                if($i>58)
                {
                    last;
                }
                $y="FEAT_DEI_SYS".$i;
                if($x=~/$y/)
                {
                    if($x=~/_ENABLE/)
                    {
                        @a = split(/0x/,$x);
                        if($io_e[$i] ne $a[1])
                        {
                            $flag=1;
                            last;
                        }
                    }
                    else
                    {
                        @a = split(/0x/,$x);
                        if($io[$i] ne $a[1])
                        {
                            $flag=1;
                            last;
                        }
                        $i++;
                    }
                }
            }
            close (INFILE);

            #IF DIFFERENCES FOUND
            if($flag==1)
            {
                #READ AS AND DEI CONFIG TO TMP STORAGE
                open (INFILE, $dei_file) or die "file not found";
                $i=0;
                while ($x=<INFILE>)
                {
                    $dei[$i]=$x;
                    $i++;
                }
                close(INFILE);

                #WRITE BACK AS AND DEI CONFIG WITH CHANGES
                open (OUTFILE, '>' . $dei_file) or die $dei_file . " not found";
                $i=31;
                foreach $x (@dei)
                {
                    $y="FEAT_DEI_SYS".$i;
                    if($x=~/$y/ && $i<=58)
                    {
                        if($x=~/_ENABLE/)
                        {
                            @a = split(/0x/,$x);
                            $x=$a[0]."0x".$io_e[$i];
                        }
                        else
                        {
                            @a = split(/0x/,$x);
                            $x=$a[0]."0x".$io[$i];
                            $i++;
                        }
                    }
                    print OUTFILE $x;
                }
                close(OUTFILE);

            }
        }
    }
    else { die "#error: Could not find config_800_gpio.h\n"; }
}

sub overwriteGpio933
{
	#check the version and overwrite PROG_OUT_NOT_CONFIG;
	#re-write to config_933_gpio.h
	open(INFILE, $config_file_933) or die "file not found";
	chmod 0666, $config_file_933;
	$min_sys = 31;   # feat version > 10, old version
	$max_sys = 56;   # last programmble output

	$i = 0;
	while ($x=<INFILE>)
	{
		$y = $NGMM_FEATURE_VERSION;
		if($x =~ /$y/)
		{
			@a = split(/ +/,$x);
			$ver933=$a[2];
			if($ver933 >= 10)
			{
				$min_sys = 40;
			}
		}

		$mPrgOI[$i] = $x;
		$i++;
	}
	close(INFILE);
	
	if($ver933 == 22)   #check the latest NGMM_FEATURE_VERSION ONLY
	{
		open(OUTFILE, '>' . $config_file_933) or die $config_file_933 . " not found";
		$i = $min_sys;
		foreach $x (@mPrgOI)
		{
			$y="FEAT_DEI_SYS".$i;
			if($x=~/$y/ && $i<=$max_sys)
			{
				if($x=~/_ENABLE/)
				{
				}
				else
				{
					my $search  = PROG_OUT_NOT_CONFIG_OLD;
					my $replace = PROG_OUT_NOT_CONFIG_NEW;
					$x =~ s/$search/$replace/;

					$i++;
				}
			}
			print OUTFILE $x;
		}
		close(OUTFILE);
	}
}

sub platform_933
{
    my $min_sys = 31;
    my $max_sys = 58;
    if(open(INFILE, $config_file_933))
    {
        chmod 0666, $config_file_933;
        chmod 0666, $file_933;

        #check for version numbers
        while ($x=<INFILE>)
        {
            $y = $NGMM_FEATURE_VERSION;
            if($x =~ /$y/)
            {
                @a = split(/ +/,$x);
                $ver933=$a[2];
                if($ver933 >= 10)
                {
                    $min_sys = 40;
                    $max_sys = 65;
                }
            }
        }
        close (INFILE);

        open (INFILE, $file_933) or die $file_933 . ' not found';
        while ($x=<INFILE>)
        {
            $y = $NGMM_FEATURE_VERSION;
            if($x =~ /$y/)
            {
                @a = split(/ +/,$x);
                $verCore933 = $a[2];
            }
        }
        close (INFILE);
        if($ver933 != $verCore933)
        {
            die "\n#error version mismatch\n";
            #READ DEI, 933 AND AS CONFIG TO TMP STORAGE

            open (INFILE, $file_933) or die "file not found";
            $i=0;
            while ($x=<INFILE>)
            {
                $m933[$i]=$x;
                $i++;
            }
            close (INFILE);

            #WRITE BACK DEI, AS AND 933 CONFIG WITH CHANGES

            open (OUTFILE, '>' . $file_933) or die $file_933 . "file not found";
            foreach $x (@m933)
            {
                $y = $NGMM_FEATURE_VERSION;
                if($x =~ /$y/)
                {
                    if($ver933!=$verCore933)
                    {
                        $x = $x."\n #error \"". $NGMM_FEATURE_VERSION . " number mismatch\"";
                    }
                }
                print OUTFILE $x;
            }
            close(OUTFILE);
        }
        else
        {
            #READ FROM USR CONFIG
            open (INFILE, $config_file_933) or die $config_file_933 . "file not found";
            $i=$min_sys;
            while ($x=<INFILE>)
            {
                $y = "FEAT_DEI_SYS".$i;
                if($x =~ /$y/)
                {
                    if($x =~ /_ENABLE/)
                    {
                        @a = split(/0x/,$x);
                        $io_e[$i]= $a[1];
                    }
                    else
                    {
                        @a = split(/0x/,$x);
                        $io[$i]= $a[1];
                        $i++;
                    }
                }
            }
            close (INFILE);

            #COMPARE WITH SVN CONFIG
            open (INFILE, $file_933)or die $file_933 . "file not found";
            $flag=0;
            $i=$min_sys;
            while ($x=<INFILE>)
            {
                if($i>$max_sys)
                {
                    last;
                }
                $y="FEAT_DEI_SYS".$i;
                if($x=~/$y/)
                {
                    if($x=~/_ENABLE/)
                    {
                        @a = split(/0x/,$x);
                        if($io_e[$i] ne $a[1])
                        {
                            $flag=1;
                            last;
                        }
                    }
                    else
                    {
                        @a = split(/0x/,$x);
                        if($io[$i] ne $a[1])
                        {
                            $flag=1;
                            last;
                        }
                        $i++;
                    }
                }
            }
            close (INFILE);

            #IF DIFFERENCES FOUND
            if($flag==1)
            {
                open (INFILE, $file_933) or die "file not found";
                $i=0;
                while ($x=<INFILE>)
                {
                    $m933[$i]=$x;
                    $i++;
                }
                close(INFILE);

                open (OUTFILE, '>' . $file_933) or die $file_933 . " not found";
                $i=$min_sys;
                foreach $x (@m933)
                {
                    $y="FEAT_DEI_SYS".$i;
                    if($x=~/$y/ && $i<=$max_sys)
                    {
                        if($x=~/_ENABLE/)
                        {
                            @a = split(/0x/,$x);
                            $x=$a[0]."0x".$io_e[$i];
                        }
                        else
                        {
                            @a = split(/0x/,$x);
                            $x=$a[0]."0x".$io[$i];
                            $i++;
                        }
                    }
                    print OUTFILE $x;
                }
                close(OUTFILE);
            }
        }
    }
    else { die "#error: Could not find config_933_gpio.h\n"; }
}

if ( @ARGV < 1 )
{
    die "#error: No arguments.";
}
my $project = $ARGV[0] ;

if($project eq "800")
{
    platform_800();
}
elsif($project eq "933")
{
    overwriteGpio933();
    platform_933();
}
else
{
    die "#error: bad project. Use 800 or 933";
}
