


tp=4
pp=1
bs=2
#opt
../build/inference/spec_infer/spec_infer -ll:gpu 4 -ll:fsize 14000 -ll:zsize 30000 -llm-model opt -llm-weight ../inference/weights/opt_13B_weights_half/ -llm-config ../inference/models/configs/opt_13B.json -ssm-model opt -ssm-weight ../inference/weights/opt_125M_weights_half/ -ssm-config ../inference/models/configs/opt_125M.json -tokenizer ../inference/tokenizer/ -prompt ../inference/prompt/chatgpt.json -tensor-parallelism-degree $tp -pipeline-parallelism-degree $pp  -output-file ../inference/output/spec_inference_opt_half_13B_tp$tp-pp-$pp-batch-size-$bs.txt > spec_infer_13b_half_tp$tp-pp-$pp-batch-size-$bs.log 2>&1 
