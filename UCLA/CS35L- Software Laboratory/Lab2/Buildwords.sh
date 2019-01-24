#!/bin/bash
sed '/<!DOCTYPE/ , /Adopt<\/td>/d’
sed ‘/<\/tr>/ , /<\/td>/d’
sed “s/<[^>]*>//g”
“s/\`/\’/g”
tr [:upper:] [:lower:]
sed “s/[ ,]/\n/g”
tr -d [:blank:]
sed “/[pk\^mnwlhaeiou]/d”
sort -u
