# xlsxTotxt
convert xlsx to txt
</br>
How to install:
</br>
    1.install libxml2
    
    2.edit Makefile 
        CINC = -I. -I changeHereToYourPath/libxml2 
        
    3.make file and use

usage: 
</br>
    ./xlsxTotxt file.xlsx 
</br>
output:
</br>
    sheet1.txt
