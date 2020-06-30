#!/bin/bash
dir=`pwd`
bazel clean
count=2
check=0
while [ $count -gt 1 ]
do
    sleep 60
    count=`ps -ef | grep test | wc -l`
    echo "==========================================================================================================================================="

    process=`ps -ef  | grep test`
    echo $process
    echo ${count}
    echo "==========================================================================================================================================="
    f1=""
    f2=""
    f1_file=""
    f2_file=""
    now_test=`ps -ef | grep '\-test' | grep -v grep | awk '{print $8}'`
    echo "now_test case is "$now_test
    $check = 1
    if [ $check -eq 1 ];then
         echo "=========================test fail,Here is the logs of failed use cases========================="
         echo "=========================test fail,Here is the logs of failed use cases========================="
         echo "=========================test fail,Here is the logs of failed use cases========================="
         exit -1
    fi
done

