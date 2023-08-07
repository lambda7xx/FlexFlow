gpu_batch_sizes=(1 2 4 8 16 32)
# gpu_batch_sizes=(1)
gpu_weight_percent=0
cpu_weight_percent=100 

gen_len=128

time_current=0807
for gpu_batch_size in "${gpu_batch_sizes[@]}"
do 
    python3 -m flexgen.flex_opt --model facebook/opt-30b --percent  $gpu_weight_percent $cpu_weight_percent 100 0 100 0 --gpu_batch_size $gpu_batch_size --overlap > opt_30B-gpu_batch_size-$gpu_batch_size-gpu_weight_percent-$gpu_weight_percent-cpu_weight_percent-$cpu_weight_percent-gen_len-$gen_len-time-$time_current.log 2>&1 


    #code  opt_6.7B-gpu_batch_size-$gpu_batch_size-gpu_weight_percent-$gpu_weight_ercent-cpu_weight_percent-$cpu_weight_percent.log
done

# gpu_weight_percent=10
# cpu_weight_percent=90 

# for gpu_batch_size in "${gpu_batch_sizes[@]}"
# do 
#     python3 -m flexgen.flex_opt --model facebook/opt-30b --percent  $gpu_weight_percent $cpu_weight_percent 100 0 100 0 --gpu_batch_size $gpu_batch_size --overlap > opt_30B-gpu_batch_size-$gpu_batch_size-gpu_weight_percent-$gpu_weight_percent-cpu_weight_percent-$cpu_weight_percent.log 2>&1 


#     #code  opt_30B-gpu_batch_size-$gpu_batch_size-gpu_weight_percent-$gpu_weight_ercent-cpu_weight_percent-$cpu_weight_percent.log
# done

gpu_weight_percent=20
cpu_weight_percent=80 

for gpu_batch_size in "${gpu_batch_sizes[@]}"
do 
    python3 -m flexgen.flex_opt --model facebook/opt-30b --percent  $gpu_weight_percent $cpu_weight_percent 100 0 100 0 --gpu_batch_size $gpu_batch_size --overlap > opt_30B-gpu_batch_size-$gpu_batch_size-gpu_weight_percent-$gpu_weight_percent-cpu_weight_percent-$cpu_weight_percent.log 2>&1 


    #code  opt_30B-gpu_batch_size-$gpu_batch_size-gpu_weight_percent-$gpu_weight_ercent-cpu_weight_percent-$cpu_weight_percent.log
done

# gpu_weight_percent=30
# cpu_weight_percent=70 

# for gpu_batch_size in "${gpu_batch_sizes[@]}"
# do 
#     python3 -m flexgen.flex_opt --model facebook/opt-30b --percent  $gpu_weight_percent $cpu_weight_percent 100 0 100 0 --gpu_batch_size $gpu_batch_size --overlap > opt_30B-gpu_batch_size-$gpu_batch_size-gpu_weight_percent-$gpu_weight_percent-cpu_weight_percent-$cpu_weight_percent.log 2>&1 


#     #code  opt_30B-gpu_batch_size-$gpu_batch_size-gpu_weight_percent-$gpu_weight_ercent-cpu_weight_percent-$cpu_weight_percent.log
# done

# gpu_weight_percent=40
# cpu_weight_percent=60 

# for gpu_batch_size in "${gpu_batch_sizes[@]}"
# do 
#     python3 -m flexgen.flex_opt --model facebook/opt-30b --percent  $gpu_weight_percent $cpu_weight_percent 100 0 100 0 --gpu_batch_size $gpu_batch_size --overlap > opt_30B-gpu_batch_size-$gpu_batch_size-gpu_weight_percent-$gpu_weight_percent-cpu_weight_percent-$cpu_weight_percent.log 2>&1 


#     #code  opt_30B-gpu_batch_size-$gpu_batch_size-gpu_weight_percent-$gpu_weight_ercent-cpu_weight_percent-$cpu_weight_percent.log
# done

# gpu_weight_percent=50
# cpu_weight_percent=50 

# for gpu_batch_size in "${gpu_batch_sizes[@]}"
# do 
#     python3 -m flexgen.flex_opt --model facebook/opt-30b --percent  $gpu_weight_percent $cpu_weight_percent 100 0 100 0 --gpu_batch_size $gpu_batch_size --overlap > opt_30B-gpu_batch_size-$gpu_batch_size-gpu_weight_percent-$gpu_weight_percent-cpu_weight_percent-$cpu_weight_percent.log 2>&1 


#     #code  opt_30B-gpu_batch_size-$gpu_batch_size-gpu_weight_percent-$gpu_weight_ercent-cpu_weight_percent-$cpu_weight_percent.log
# done

# gpu_weight_percent=60
# cpu_weight_percent=40 

# for gpu_batch_size in "${gpu_batch_sizes[@]}"
# do 
#     python3 -m flexgen.flex_opt --model facebook/opt-30b --percent  $gpu_weight_percent $cpu_weight_percent 100 0 100 0 --gpu_batch_size $gpu_batch_size --overlap > opt_30B-gpu_batch_size-$gpu_batch_size-gpu_weight_percent-$gpu_weight_percent-cpu_weight_percent-$cpu_weight_percent.log 2>&1 


#     #code  opt_30B-gpu_batch_size-$gpu_batch_size-gpu_weight_percent-$gpu_weight_ercent-cpu_weight_percent-$cpu_weight_percent.log
# done



# gpu_weight_percent=70
# cpu_weight_percent=30 

# for gpu_batch_size in "${gpu_batch_sizes[@]}"
# do 
#     python3 -m flexgen.flex_opt --model facebook/opt-30b --percent  $gpu_weight_percent $cpu_weight_percent 100 0 100 0 --gpu_batch_size $gpu_batch_size --overlap > opt_30B-gpu_batch_size-$gpu_batch_size-gpu_weight_percent-$gpu_weight_percent-cpu_weight_percent-$cpu_weight_percent.log 2>&1 


#     #code  opt_30B-gpu_batch_size-$gpu_batch_size-gpu_weight_percent-$gpu_weight_ercent-cpu_weight_percent-$cpu_weight_percent.log
# done