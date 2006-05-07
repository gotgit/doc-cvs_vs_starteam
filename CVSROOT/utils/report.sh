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
		outstr="用户 $user 检出源代码情况:";
		;;
	A*)
		outstr="用户 $user 代码更新频率:";
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
