#!/usr/bin/env bash

telegraf=$(which telegraf)
if [ ! -x "$telegraf" ] ; then
    echo "telegraf must be in path for this test to run"
    exit 1
fi

tests/integration/sample_server&
sample_server_pid=$!
sleep 1
telegraf_output="$(telegraf -test -config tests/integration/scrape.conf)"
telegraf_run_result=$?
kill -9 $sample_server_pid

if [ $telegraf_run_result -ne 0 ] ; then
    exit $telegraf_run_result
fi

if [[ ! $telegraf_output == *"time_running_seconds"* ]] ; then
   echo "Could not find time_running_seconds in exposed metrics:"
   echo "${telegraf_run_output}"
   exit 1
fi

echo "Success:"
echo "${telegraf_output}"

exit 0
