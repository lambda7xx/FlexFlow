#!/bin/bash
rm -rf /home/lambda/flexgen_offload_dir
MY_IPADDR=$(hostname -i)
all_hosts=$MY_IPADDR
N_GPUS=2
N_CORES_PER_GPU=6

PYTHON_EXEC=$CONDA_PREFIX/bin/python
PYTHON_SCRIPT=flexgen.dist_flex_opt

pgrep -fl python | awk '!/dist_flex_opt\.py/{print $1}' | xargs sudo kill

set -x

gpu_batch_sizes=(1 2 4 8 16 32)
gpu_weight_percent=100
cpu_weight_percent=0

for gpu_batch_size in "${gpu_batch_sizes[@]}"
do 

rm -rf /home/lambda/flexgen_offload_dir

mpirun \
  --mca btl_tcp_if_exclude lo,docker0 \
  --mca oob_tcp_if_exclude lo,docker0 \
  --map-by ppr:$N_GPUS:node:pe=$N_CORES_PER_GPU --oversubscribe -H $all_hosts \
  --bind-to core -x OMP_NUM_THREADS=$N_CORES_PER_GPU \
  $PYTHON_EXEC -m $PYTHON_SCRIPT \
    --head-ip $MY_IPADDR \
    --port 7777 \
    --use-mpi \
    --model facebook/opt-6.7b \
    --gpu_batch_size $gpu_batch_size \
    --percent $gpu_weight_percent $cpu_weight_percent 100 0 100 0 \
    --comm-device cpu \
    --cut_gen_len 5 \
    --overlap \
    --path _DUMMY_ > gpus_$N_GPUS-opt_6.7B-gpu_batch_size-$gpu_batch_size-gpu_weight_percent-$gpu_weight_percent-cpu_weight_percent-$cpu_weight_percent.log 2>&1 

pgrep -fl python | awk '!/dist_flex_opt\.py/{print $1}' | xargs sudo kill

done  