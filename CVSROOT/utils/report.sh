#!/bin/sh

set -x
users="linmf wupeng xuzr zengdw wangshuai liyong zengzf sunbing jiangxin"
actions="O AMR"
date="-D '2003-04-01 00:00' -z CST"
date="-z CST"


for act in $actions ; do
    for user in $users; do
	case $act in
	O)
		cmd="cvs history -x $act -u $user $date"
		outstr="�û� $user ���Դ�������:";
		;;
	A*)
		outstr="�û� $user �������Ƶ��:";
		cmd="cvs history -x $act -u $user $date | wc -l"
		;;
	*)
		continue
		;;
	esac

	echo ""
	echo $outstr
	echo ----------------------------------------------------------
	eval $cmd
	echo ""
    done
done
