#!/usr/bin/python                                                                                                                                                                                                                             

"""                                                                                                                                                                                                                                           
Output lines selected randomly from a file using shuf method
                                                                                                                                                                                                                                              
Copyright 2005, 2007 Paul Eggert.                                                                                                                                                                                                             
Copyright 2010 Darrell Benjamin Carbajal.                                                                                                                                                                                                     
                                                                                                                                                                                                                                              
This program is free software: you can redistribute it and/or modify                                                                                                                                                                          
it under the terms of the GNU General Public License as published by                                                                                                                                                                          
the Free Software Foundation; either version 3 of the License, or                                                                                                                                                                             
(at your option) any later version.                                                                                                                                                                                                           
                                                                                                                                                                                                                                              
This program is distributed in the hope that it will be useful,                                                                                                                                                                               
but WITHOUT ANY WARRANTY; without even the implied warranty of                                                                                                                                                                                
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                                                                                                                                                                                 
GNU General Public License for more details.                                                                                                                                                                                                  
                                                                                                                                                                                                                                              
Please see <http://www.gnu.org/licenses/> for a copy of the license.                                                                                                                                                                          
                                                                                                                                                                                                                                              
$Id: randline.py,v 1.4 2010/04/05 20:04:43 eggert Exp $                                                                                                                                                                                       
"""

import random, sys
import argparser


def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE                                                                                                                                                                                                     
                                                                                                                                                                                                                                              
Output randomly selected lines from FILE."""

    parser = argparse.ArgumentParser(description='shuffle the input')
    parser.add_argument("-n","--numlines",action"store",dest="numlines",help="output at most COUNT lines")
    parser.add_argument("-e", "--echo",
                      action="store", dest="echolines", default=None, nargs='+',
                      help="treat each ARG as an input line")
    parser.add_argument("-r", "--repeat",
                        action="store_const", dest="repeat", default=False, const=True,
                        help="whether to shuffle with replacement or without")
    args=parser.parse_args()
    input_lines=args.echolines

    try:
        numlines = int(args.numlines)
    except:
        numlines=len(input_lines)
    if numlines < 0:
        parser.error("negative count: {0}".
                     format(numlines))
    if args.repeat:
        for i in range (numlines):
            line=random.choice(input_lines)
            print(line)
    else:
        lines=random.sample(input_lines,numlines)
        for line in lines:
            print(line)


if __name__ == "__main__":
    main()
