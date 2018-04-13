#!/usr/bin/zsh

for file in "../vrp/M-Sets/*"
do
    mv $file ` echo $file | cut -c 5-`
done