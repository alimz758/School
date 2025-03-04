#!/bin/bash
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
        isutf=`file -i $file | grep -i charset=ascii\$`
        if [ -n "$isutf" ]
        then
            echo "$file"
        fi
    fi
done
