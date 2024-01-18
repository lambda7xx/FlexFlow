export CUDA_VISIBLE_DEVICES=0

./build/inference/incr_decoding/incr_decoding -ll:gpu 1 -ll:fsize 14000 -ll:zsize 30000 --fusion  -llm-model meta-llama/Llama-2-7b-chat-hf  -prompt /ssd1/xiaoxias/flexflow/prompt/prompt.json -output-file  output/Llama-2-7b-chat-hf.txt  -cache-folder /ssd1/xiaoxias/llama2/ -pipeline-parallelism-degree 1 > llama2-7b-chat-half.log 2>&1 
