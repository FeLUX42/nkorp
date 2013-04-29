# Copyright (C) 2013 Felix Passenberg
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
# 02110-1301, USA.
#
# Autor:
# 		Felix Passenberg <fc.passenberg@gmail.com>
#

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



