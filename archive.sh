tar -cvf - ./data | xz -9 > data.tar.xz
tar -cvf - ./data-per | xz -9 > data-per.tar.xz

rm -r ./data
rm -r ./data-per
