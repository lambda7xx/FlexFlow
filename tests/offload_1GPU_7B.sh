
bs=2
offload_memory=6000
model=opt-6B
#opt-6B
../build/inference/spec_infer/spec_infer -ll:gpu 1 -ll:fsize 14000 \
-ll:zsize 40000  -llm-model opt \
-llm-weight ../inference/weights/opt_6B_weights_half -llm-config ../inference/models/configs/opt_6B.json \
-ssm-model opt -ssm-weight ../inference/weights/opt_125M_weights_half/ \
-ssm-config ../inference/models/configs/opt_125M.json \
-tokenizer ../inference/tokenizer/ \
-prompt ../inference/prompt/chatgpt.json \
-offload -offload-reserve-space-size 6000 \
-output-file ../inference/output/spec_inference_opt6B_half_BS$bs-cpu-memory-$offload_memory.txt \
> model-$model-cpu-memory-$offload_memory-batch-size-$bs.log 2>&1 


#./inference/spec_infer/spec_infer -ll:gpu 1 -ll:fsize 14000 -ll:zsize 30000 -llm-model llama -llm-weight ../inference/weights/ -llm-config ../inference/models/config/opt_13B.json  -ssm-model opt -ssm-weight ../inference/weights/opt_125M_weights/ -ssm-config ../inference/models/configs/opt_125M.json -tokenizer ../inference/tokenizer/ -prompt ../inference/prompt/chatgpt.json  -offload -offload-reserve-space-size $offload_memory > model-$model-cpu-memory-$offload_memory-batch-size-$bs.log 2>&1 
