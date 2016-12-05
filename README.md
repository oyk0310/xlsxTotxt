# xlsxTotxt
convert xlsx to txt
How to install:
    1.install libxml2  
    2.edit Makefile 
        CINC = -I. -I changeHereToYourPath/libxml2     
    3.make file and use
usage: 
    ./xlsxTotxt file.xlsx 
output:
    sheet1.txt
