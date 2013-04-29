while true; 
do
	inotifywait -qr --timefmt '%d/%m/%y %H:%M' --format '%T %w %f %e' \
-e create "./data-per" | while read DATE TIME DIR FILE EVENT;do
	
		#echo "in: $FILE"
		FILECHANGE=${DIR}${FILE}
		# convert absolute path to relative
		FILECHANGEREL=`echo "$FILECHANGE" | sed 's_'$CURPATH'/__'`
	
		#echo "At ${TIME} on ${DATE}, file $FILECHANGE was backed up via rsync"
		./out.sh
		
	done
	sleep 600
done
