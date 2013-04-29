cd ./images
#png
mplayer -vo png:z=1 ./animate.gif  -fps 1024 -af framestep=1   #einzelbilder
avconv  -i %08d.png -y -vcodec png   -an -r 25 animated.mov
rm *.png
mplayer -vo png:z=1 ./animate-period.gif  -fps 1024 -af framestep=1 # perioden
avconv  -i %08d.png -y -vcodec png   -an -r 25 animated-period.mov

#ffmpeg -r 25 -i %08d.png -y -vcodec png   -an -r 25 animated.mov
#ffmpeg -r 25 -i %08d.png -y -vcodec qtrle -an -r 25 animated.mov
#ffmpeg -r 25 -i %08d.png -y -vcodec ffv1  -an -r 25 animated.avi
rm *.png


#jpg
#mplayer -vo jpeg ./animate.gif -really-quiet -fps 1000 -af framestep=1
#ffmpeg -r 25 -i %08d.jpg -y -vcodec qtrle -an animatedjpg.mov
#ffmpeg -r 25 -i %08d.jpg -y -vcodec ffv1  -an animatedjpg.avi
rm *.jpg

cd ..



