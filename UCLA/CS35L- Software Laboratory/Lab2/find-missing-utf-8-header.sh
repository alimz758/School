#!/bin/bash

#  find-missing-utf-8-header.sh
#  
#
#  Created by ali mirabzadeh on 1/22/19.
#

for file in "$@"
do
    #if the arguement is a directory
    if [ -d "$file" ]
    then
    #loop through the directory
    content=$(ls -A "$file")
    for i in $content
    do
        $0 $(find "$file/$i" -maxdepth 0)
    done
    #if it is a file
    elif [ -f "$file" ]
    then
        #check if it is ascii
        isutf=`file -i $file | grep -i charset=utf-8\$`
        if [ -n "$isutf" ]
        then
            firstLine=$(head -n 1 < "$file")
            if [ "$firstLine" != "-*- coding: utf-8 -*-" ]
            then
                echo "$passed"
            fi
        fi
    fi
done
