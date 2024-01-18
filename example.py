import flexflow.serve as ff

# Initialize the FlexFlow runtime. ff.init() takes a dictionary or the path to a JSON file with the configs
ff.init(
        num_gpus=4,
        memory_per_gpu=14000,
        zero_copy_memory_per_node=30000,
        tensor_parallelism_degree=4,
        pipeline_parallelism_degree=1
    )

# Create the FlexFlow LLM
llm = ff.LLM("meta-llama/Llama-2-7b-hf")

# Create the sampling configs
generation_config = ff.GenerationConfig(
    do_sample=True, temperature=0.9, topp=0.8, topk=1
)

# Compile the LLM for inference and load the weights into memory
llm.compile(generation_config,
            max_requests_per_batch = 16,
            max_seq_length = 256,
            max_tokens_per_batch = 128)

# Generation begins!
llm.start_server()
result = llm.generate("Here are some travel tips for Tokyo:\n")
llm.stop_server() # This invocation is optional


#export CUDA_VISIBLE_DEVICES=0
#./inference/incr_decoding/incr_decoding -ll:gpu 1 -ll:fsize 14000 -ll:zsize 30000 -llm-model meta-llama/Llama-2-7b-chat-hf  -prompt /ssd1/xiaoxias/flexflow/prompt/prompt.json  --fusion  -cache-folder /ssd1/xiaoxias/llama2/