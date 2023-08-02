
bs=16
../build/inference/incr_decoding/incr_decoding \
-ll:gpu 4 -ll:fsize 14000 -ll:zsize 30000 \
-llm-model opt -llm-weight ../inference/weights/opt_6B_weights_half/ -llm-config ../inference/models/configs/opt_6B.json \
-tokenizer ../inference/tokenizer/ -prompt ../inference/prompt/chatgpt.json \
-output-file ../inference/output/incr_decoding_opt_6B_half_bs$bs.txt -pipeline-parallelism-degree 4 > incr_6b_bs$bs.log 2>&1 
