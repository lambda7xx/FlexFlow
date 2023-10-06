# Copyright 2023 CMU, Facebook, LANL, MIT, NVIDIA, and Stanford (alphabetical)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from .llama import FlexFlowLLAMA, LLAMAConfig
from .opt import FlexFlowOPT, OPTConfig
from .falcon import FlexFlowFalcon, FalconConfig
from .starcoder import FlexFlowSTARCODER, STARCODERConfig
from .mpt import FlexFlowMPT, MPTConfig
from .baichuan import FlexFlowBAICHUAN