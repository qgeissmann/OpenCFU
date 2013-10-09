#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <utility>
#include "ArgumentParser.hpp"

ArgumentParser::ArgumentParser(int argc, char **argv,ProcessingOptions& opts):
m_help_string("OpenCFU options:\n\
-i FILE\n\
    \tThe name of the input image file\n\
-h\n\
    \tPrint this help\n\
-v\n\
    \tPrint the version number\n\
-a\n\
    \tSet the auto-threshold ON. It overrides the argument -t\n\
-o FILE\n\
    \tThe name of the output image file (none by default)\n\
-m {FILE, auto}\n\
    \tThe name of the mask image file OR the keyword `auto'\n\
-d {reg,inv,bil}\n\
    \tThe type of threshold: regular, inverted OR bilateral (the default value `reg')\n\
-t NUM\n\
    \tSet the threshold (the default value is 10)\n\
-l NUM\n\
    \tSet a threshold for outlier exclusion(the default value is 10)\n\
-r NUM\n\
    \tSet the minimal radius (the default value is 3)\n\
-R NUM\n\
    \tSet a maximal radius (none by default)\n\
-c NUM\n\
    \tset a \"center\" value of the Hue/Colour threshold\n\
-C NUM\n\
    \tset a \"tolerance\" value of the Hue/Colour threshold\n\
")
{
    std::stringstream tss;
    std::pair<int,int> min_max_radius,cent_tol_hue;
    min_max_radius = opts.getMinMaxRad();
    cent_tol_hue = opts.getCenTolHue();
    signed char c=0;

    while ( (c = getopt (argc, argv, "hvad:i:m:r:R:c:C:t:l:o:")) != -1){
        switch(c){
            case 'h':
                 printHelp();
                 exit(EXIT_SUCCESS);
                 break;
            case 'v':
                 DEV_INFOS("test");
                 printVersion();
                 exit(EXIT_SUCCESS);
                 break;

            case 'a':
                 opts.setHasAutoThr(true);
                 break;
            case 'r':
                min_max_radius.first =atoi(optarg);
                break;

            case 'R':
                opts.setHasMaxRad(true);
                min_max_radius.second = atoi(optarg);
                break;

            case 'c':
                opts.setHasHueFilt(true);
                cent_tol_hue.first = atoi(optarg);
                break;

            case 'C':
                opts.setHasHueFilt(true);
                cent_tol_hue.second = atoi(optarg);
                break;
            case 'd':{
                std::string str_optarg(optarg);
                if(str_optarg == "reg")
                    opts.setThrMode(OCFU_THR_NORM);
                else if(str_optarg == "inv")
                    opts.setThrMode(OCFU_THR_INV);
                else if(str_optarg == "bil")
                    opts.setThrMode(OCFU_THR_BILAT);
                else{
                    std::cerr<<"ERROR the type of threshold ("<<tss.str()<<") must be one of {reg,inv,bil}"<<std::endl;
                    exit(EXIT_FAILURE);
                }
                break;
            }
            case 'i':
                tss.str(optarg);
                if (!opts.setImage(tss.str())){
                    std::cerr<<"ERROR setting-up the INPUT file\""<<tss.str()<<"\"."<<std::endl;
                    exit(EXIT_FAILURE);
                }

                break;

            case 'm':
                if(std::string(optarg) != "auto"){
                    if (!opts.setMask(MaskROI(optarg))){
                        std::cerr<<"ERROR setting up the MASK file\""<<tss.str()<<"\"."<<std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
                else
                    opts.setMask(MaskROI(MASK_TYPE_AUTO));

                break;
            case 't':
                 if (!opts.setThr(atoi(optarg))){
                    std::cerr<<atoi(optarg)<<"Incorrect value for threshold.\
                     It should be in the [0,255] range"<<std::endl;
                     exit(EXIT_FAILURE);
                }

                break;

            case 'l':
                 if (!opts.setLikeThr(atof(optarg))){
                    std::cerr<<atof(optarg)<<" Incorrect value for outlier\
                     exclusion threshold."<<std::endl;
                     exit(EXIT_FAILURE);
                }
                else
                    opts.setHasOutlierFilt(true);

                break;

            case '?':
                if (optopt == 'c')
                    std::cerr<<"Option -"<<optopt<<" requires an argument"<<std::endl;
                else if (isprint (optopt))
                    std::cerr<<"Unknown option -"<<optopt<<std::endl;
                else
                    std::cerr<<"Unknown option character"<<optopt<<std::endl;
                exit(EXIT_FAILURE);
                break;
            default:
                break;
        }
    }

    if (opts.getImage().empty()){
        std::cerr<<"ERROR no input file ! Use -i FILE"<<std::endl;
        exit(EXIT_FAILURE);
    }

    if(!opts.setMinMaxRad(min_max_radius)){
        std::cerr<<"ERROR setting up the radius limits\
         (arguments \"-r\" and \"-R\")."<<std::endl;
         exit(EXIT_FAILURE);
    }
    if(!opts.setCenTolHue(cent_tol_hue)){
        std::cerr<<"ERROR setting up the hue limits\
            (arguments \"-c\" and \"-C\"). They \
            must be in the [0,360] and [0,180] \
            range respecively"<<std::endl;
            exit(EXIT_FAILURE);
    }

}

void ArgumentParser::printHelp(){
    std::cout<<m_help_string<<std::endl;
}

void ArgumentParser::printVersion(){
    std::cout<<PACKAGE_VERSION<<std::endl;
}
