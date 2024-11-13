#!/bin/bash

cd build
touch trials
grep model trials
if [ $? -ne 0 ]; then
	 grep "model name" < /proc/cpuinfo | head -1 >> trials
	 grep "MemTotal" < /proc/meminfo  >> trials
fi
for args in `ls -1 ???`
do
  echo ${args}
  ./${args} | tail -1 >>trials
  ./${args} | tail -1 >>trials
  ./${args} | tail -1 >>trials
done

cd ..
mv ./build/trials .
python plot.py trials
