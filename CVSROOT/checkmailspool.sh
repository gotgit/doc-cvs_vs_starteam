#!/bin/sh
# checkmailspool.sh
# Auther: Jiang Xin
#
# $CVSMAILPATH (cvs mail spool) is a spool directory created by user, 
# and cvs tag message will store in it.
# run this script timely to check cvsmail spool and send messages...
# please put this script in crontab. 


CVSMAILPATH=/var/spool/cvsmail
if [ ! -d $CVSMAILPATH ]; then
	mkdir -p $CVSMAILPATH
	chmod 777 $CVSMAILPATH
fi

cd $CVSMAILPATH
for i in `ls `; do
	pid=`echo $i| sed -e "s/.*\.\([0-9]*\)$/\1/"`
	
	xxx=0
	
	while ps -p $pid>/dev/null; do
		xxx=`expr $xxx + 1`
		if [ $xxx -gt 10 ]; then
			break
		fi
		sleep 3
		echo waiting $pid, $xxx times ...
	done
	echo -e "\n\n========================================" >> $i
	echo -e "End\n" >> $i
	cat $i | sendmail -oi -oem -odb -t
	rm -f $i
done
