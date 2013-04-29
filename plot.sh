#!/bin/bash

POUT=plotper.gpi
OUT=plot.gpi
echo "set output \"images/animate-period.gif\"" > $POUT
echo "set output \"images/animate.gif\"" > $OUT

PI=3.14159265
#echo "$POUT"
#echo "$OUT"

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

SIZE=-10e20:10e20   # galaxie
#SIZE=-2e12:2e12   # ausseres Sonnensystem
#SIZE=-5e11:5e11   # inneres Sonnensystem

SIZEx=$SIZE
SIZEy=$SIZE
SIZEz=$SIZE

for FILE in *.gpi
do
	echo "${FILE}"
	#echo "set terminal gif animate delay 100 optimize" >>"${FILE}"
	echo "set isosample 40" >> "${FILE}"
	echo "set hidden3d" >> "${FILE}"
	echo "set xrange [$SIZEx]; set yrange [$SIZEy]; set zrange[$SIZEz]" >> "${FILE}"
	
	echo "set view 0,0" >> "${FILE}"
	echo "set terminal gif animate delay 80 optimize size 1024,1024" >> "${FILE}"
	echo "set key off" >> "${FILE}"
	#echo "set autoscale" >> "${FILE}"
	echo "set size square" >> "${FILE}"
	
done


I=0

for file in $(ls data-per); do

			#data-per ordner einlesen
	I=$(($I+1))
	
	heigth=$((($I*1)%360))
	if [ $heigth -lt 0 ]; then 
		heigth=$((-$heigth))
	fi
	rot=$((($I*1)%360))
	#heigth=00 # drauf-sicht
	heigth=10 
	#heigth=90 # seiten-ansicht
	rot=45
	heigth=`echo "s ( $heigth / 180 * $PI /2 +0.5) * 90" | bc -l`
	
	echo "set view $heigth,$rot" >> $POUT

	echo 'splot "data-per/'$file'" using 1:2:3 w l , "data-per/'$file'" using 4:5:6 w l, "data-per/'$file'" using 7:8:9 w l, "data-per/'$file'" using 10:11:12 w l, "data-per/'$file'" using 13:14:15 w l , "data-per/'$file'" using 16:17:18 w l , "data-per/'$file'" using 19:20:21 w l , "data-per/'$file'" using 22:23:24 w l , "data-per/'$file'" using 25:26:27 w l , "data-per/'$file'" using 28:29:30 w l , "data-per/'$file'" using 31:32:33 w l , "data-per/'$file'" using 34:35:36 w l , "data-per/'$file'" using 37:38:39 w l , "data-per/'$file'" using 40:41:42 w l , "data-per/'$file'" using 43:44:45 w l , "data-per/'$file'" using 46:47:48 w l , "data-per/'$file'" using 49:50:51 w l , "data-per/'$file'" using 52:53:54 w l , "data-per/'$file'" using 55:56:57 w l , "data-per/'$file'" using 58:59:60 w l ' >> $POUT
		#Sonnensystem Perioden 2D
done

echo "$I"

	heigth=10 
	heigth=`echo "(s ( $heigth / 180 * $PI )/2 +0.5) * 90 " | bc -l`

I=0
for file in $(ls data); do
			#data Ordner einlesen
	I=$(($I+1))
#	heigth=$((( $I*1 ) %360))
#	if [ $heigth -lt 0 ]; then 
#		heigth=$((-$heigth))
#	fi
#	rot=$((($I*1)%360))
	#heigth=00 # drauf-sicht
#	heigth=10 
	#heigth=90 # seiten-ansicht
#	heigth=`echo "(s ( $heigth / 180 * $PI )/2 +0.5) * 90 " | bc -l`
	rot=45
	echo "set view $heigth,$rot" >> $OUT

	#$(($day+1)) 
	#echo 'splot "data/'$file'" w d' >> $OUT
		#ausgabe aller als punktfolge 3d
	echo 'splot "data/'$file'" w d ' >> $OUT
		#ausgabe aller als kreuzfolge 2d
	
	#echo 'plot "data/'$file'" using 1:2, "data/'$file'" using 4:5 , "data/'$file'" using 7:8, "data/'$file'" using 10:11, "data/'$file'" using 13:14, "data/'$file'" using 16:17, "data/'$file'" using 19:20, "data/'$file'" using 22:23' >> $OUT
		#Sonnensystem 2D
done

echo "$I"


echo "start animation of $POUT"
gnuplot $POUT
rm $POUT

echo ""
echo "start animation of $OUT"
gnuplot $OUT
rm $OUT



sleep 1



#animate images/* -delay 10
