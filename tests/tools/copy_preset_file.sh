#!/bin/bash

filepath=$1
num_copy=$2

# Copy <filepath> to output/ <num_copy> times.
# e.g.
# ./tests/tools/copy_preset_file.sh assets/presets/Default.oapreset 10

script_dir=$(cd $(dirname $0); pwd)
mkdir -p $script_dir/output 2>/dev/null
input_filename="${filepath##*/}"
for i in $(seq 0 $(expr $num_copy - 1)); do
  cp $1 $script_dir/output/Copied-$i-$input_filename;
done
