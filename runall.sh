#!/bin/bash

FILE=sample_result

cd build
touch $FILE
grep model $FILE
if [ $? -ne 0 ]; then
   grep "model name" < /proc/cpuinfo | head -1 >> $FILE
   grep "MemTotal" < /proc/meminfo  >> $FILE
fi
for args in `ls -1 ???`
do
  echo ${args}
  ./${args} | tail -1 >> $FILE
done

cd ..
mv ./build/$FILE .
python plot.py $FILE
