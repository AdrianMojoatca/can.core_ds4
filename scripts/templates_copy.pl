#!/usr/local/bin/perl

use strict;
use File::Compare;

sub TemplatesCopy
{
    my $project = @_[0];
    use File::Copy;
    my @files;
    my @template_files;
    
    if($project eq "cm800_v2")
    {
        die "#error: Change cm800_v2 argument to 800 or 933";
    }
    
    ##paths are relative to the keil project
    if($project eq "402" || $project eq "403" || $project eq "404")
    {
        $project = "dball";  ##for now, 402 and 403 are identical. No need to double check logic every time.
    }
    if($project eq "dball")
    {
        @template_files = ('.\core\lib.rsr\inc\config_rsr.h',
                                '.\core\lib.rsr_feature\inc\config_rsr_feature.h',
                                    '.\core\lib.rxt\inc\config_rxt.h');
                                    
        @files = ('.\inc.config.fw\config_rsr.h',
                    '.\inc.config.fw\config_rsr_feature.h',
                        '.\inc.config.fw\config_rxt.h');
    }
    elsif($project eq "cm800_v1")
    {
        if( -d '.\core') #the core is located in the same folder as the project (old architecture).
        {
            @template_files = ('.\core\common\lib.800_gpio\TEMPLATE\config_800_gpio.h',
                                    '.\core\autostart\lib.as.feature\TEMPLATE\config_as_feature.h',
                                        '.\core\dei\lib.dei_feature\TEMPLATE\config_dei_feature.h');
        }
        elsif( -d '..\cmall\core\core_v1') #the core is located in the CMALL folder as the project (new architecture).
        {
            @template_files = ('..\cmall\core\core_v1\common\lib.800_gpio\TEMPLATE\config_800_gpio.h',
                                    '..\cmall\core\core_v1\autostart\lib.as.feature\TEMPLATE\config_as_feature.h',
                                        '..\cmall\core\core_v1\dei\lib.dei_feature\TEMPLATE\config_dei_feature.h');
        }
        else
        {
            die "Could not find the templates";
        }
        @files = ('.\inc.config_fw\config_800_gpio.h',
                       # '.\inc.config_fw\config_as_feature.h',
                            '.\inc.config_fw\config_dei_feature.h');
    }
    elsif($project eq "800")
    {
	    if( -e '.\core_800\lib.800_gpio_2300\TEMPLATE\config_800_gpio.h')
        {
			@template_files = ('.\core_800\lib.800_gpio_2300\TEMPLATE\config_800_gpio.h',
								#    '.\core_800\lib.as.feature\TEMPLATE\config_as_feature.h',
										'.\core_800\lib.dei_feature\TEMPLATE\config_dei_feature.h');
			
		}
		else
		{
			@template_files = ('.\core\lib.800_gpio_2300\TEMPLATE\config_800_gpio.h',
								#    '.\core\lib.as.feature\TEMPLATE\config_as_feature.h',
										'.\core\lib.dei_feature\TEMPLATE\config_dei_feature.h');
		}           
        @files = ( '.\inc.config_fw\config_800_gpio.h',
                        # '.\inc.config_fw\config_as_feature.h',
                            '.\inc.config_fw\config_dei_feature.h');
    }
    elsif($project eq "933")
    {
        @template_files = ('.\core\lib.800_gpio_1700\TEMPLATE\config_933_gpio.h',
                                '.\core\lib.ngmm_feature\TEMPLATE\config_ngmm_feature.h');
                                
        @files = ('.\inc.config_fw\config_933_gpio.h',
                    '.\inc.config_fw\config_ngmm_feature.h');
        
        my $dest_file = '.\inc.config_fw\config_933.h';
        if( ! -e $dest_file)
        {
            use File::Copy qw(copy);
            copy '.\core\inc.config_1700\TEMPLATE\config_933.h', $dest_file ;
            print "\n#warning: Don't forget to add file " . $dest_file . " to SVN";
        }
    }
    elsif($project eq "common")
    {
        @template_files = ('..\common\core\lib.obd_can\template\config_obd_can.h');
                                
        @files = ('..\common\com.inc.config.fw\config_obd_can.h');
    }
    else
    {
        die "Did not enter a valid project (dball, cm800_v1, 800, 933 or common)." . $project ;
    }

    if($project ne "dball")
    {
        for my $file (@files)
        {
            if( ! -e $file)
            {
                print "\n#warning: Don't forget to add file " . $file . " to SVN";
            }
        }
    }

        
    for my $file (@template_files)
    {
        chmod 0666, $file or next;
    }
    
    my $size = scalar @files;
    for (my $i = 0; $i < $size; $i++)
    {
        my $in_file = $template_files[$i];
        my $bak_filename = $files[$i];
        if($project eq "dball")
        {
            if(! -e $bak_filename )
            {
                next ;
            }
        }
        my $out_file = $bak_filename . '.bak';
        if(open my $in,  '<', $in_file)
        {
            if(open my $out, '>', $out_file)
            {
                my $lines = do { local $/; <$in> };
                my @lines_split = split('\n', $lines);
                
                
                my $define = '#define';
                my $version = 'VERSION' ;
                if(open(BAKFILE, $bak_filename))
                {
                    while(my $bak_line=<BAKFILE>)
                    {
                        if($bak_line=~/$define/ and $bak_line!~/$version/)
                        {
                            my @words = split(' ',$bak_line);
                            if($project eq "933")
                            {
                                if($words[1] ne "FEAT_DEI_SS_SIZE" and $words[1] ne "FEAT_DEI_RS_SIZE" and $words[1] ne "FEAT_DEI_SYS_SIZE")
                                {
                                    foreach my $new_line (@lines_split)
                                    {
                                        my @new_split = split(' ',$new_line);
                                        if($new_split[0] eq $define and $new_split[1] eq $words[1])
                                        {
                                            chomp $bak_line;
                                            $new_line = $bak_line;
                                            last;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                if($words[1] ne "FEAT_DEI_SYS39_ENABLE" and $words[1] ne "FEAT_DEI_SYS40_ENABLE" and $words[1] ne "FEAT_DEI_SYS41_ENABLE" and $words[1] ne "FEAT_DEI_SYS42_ENABLE" and 
                                        $words[1] ne "FEAT_DEI_SYS39" and $words[1] ne "FEAT_DEI_SYS40" and $words[1] ne "FEAT_DEI_SYS41" and $words[1] ne "FEAT_DEI_SYS42")
                                {
                                    foreach my $new_line (@lines_split)
                                    {
                                        my @new_split = split(' ',$new_line);
                                        if($new_split[0] eq $define and $new_split[1] eq $words[1])
                                        {
                                            chomp $bak_line;
                                            $new_line = $bak_line;
                                            last;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    close(BAKFILE);
                }
                foreach(@lines_split)
                {
                    print $out "$_\n";
                }
                close($out);
            }
            close($in);
        }
        
    }
    
    for my $file (@files)
    {
        my $new_file = $file . '.bak';
        my $result = compare($file, $new_file);
		if($result == 0)  #new file created is identical
        {
            unlink $new_file; #no need to update the old file.
        }
        elsif($result < 0) #an error happened, file has not been copied
        {
			#do nothing
		}
		else #files are different, we keep the new one.
		{
			unlink $file;
            rename $new_file, $file;
        }
    }
    
}

TemplatesCopy($ARGV[0]);