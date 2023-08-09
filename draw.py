import matplotlib.pyplot as plt
import numpy as np
#plt.rcParams['font.serif'] = "Times New Roman"
#plt.rcParams['font.family'] = "serif"
plt.rcParams['pdf.fonttype'] = 42
plt.rcParams['ps.fonttype'] = 42

c1 = (31.0/255, 119/255.0, 180/255.0)
c2 = (174/255.0, 199/255.0, 232/255.0)
c3 = (1, 127/255.0, 14/255.0)
c4 = (1, 187/255.0, 120/255.0)
c5 = (44/255.0, 160/255.0, 44/255.0)
c6 = (152/255.0, 223/255.0, 138/255.0)
c7 = (214/255.0, 39/255.0, 40/255.0)
c8 = (1, 152/255.0, 150/255.0)
c9 = (148/255.0, 103/255.0, 189/255.0)
c10 = (192/255.0, 176/255.0, 213/255.0)

FlexGen = [
    [370.8327553, 403.8242456, 416.0919291, 415.056092, 416.3340878],
    [595.169719, 592.8307257 ,632.9586692 , 735.6939688, 750.8852422],
    [1284.350462, 1367.901345, 1491.596517, 1497.331617, 1668.599014]
]
spec_infer =[
    [1023.884848, 1023.51699, 1023.61249, 1023.626708, 1023.596357],
    [1952.241426, 1952.226066, 1952.17335, 1952.17335, 1952.17335],
    [4.51062433819496*1000,4.51062433819496*1000,4.51062433819496*1000,4.51062433819496*1000,4.51062433819496*1000]]
print(len(spec_infer))
# vllm = [
# [52.3716686,
# 53.2265734,
# 55.440323,
# 64.884101,
# 106.787828],
# [50.6002969,
# 51.3247629,
# 53.1505398,
# 60.4448059,
# 78.7203643]]

# hf_tgi = [
# [45.36731553,
# 49.69571741,
# 54.59911763,
# 62.0441023,
# 96.78166596],
# [43.8328527,
# 45.991527,
# 52.3440777,
# 61.1114743,
# 70.606995]]

# ft = [
# [42.89945313,
# 45.02945313,
# 50.47695313,
# 62.123125,
# 84.1871875],
# [38.49335938,
# 40.37757813,
# 45.26210938,
# 55.87609375,
# 74.30695313]]

# inc_decoding = [
# [44.0970093,
# 47.3987778,
# 54.9796665,
# 68.863389,
# 98.6489444],
# [40.2914683,
# 45.544,
# 51.4672698,
# 64.9975952,
# 91.6489444]]

# spec_infer = [
# [21.0304795,
# 23.9184932,
# 27.72705479,
# 37.45145377,
# 59.5836161],
# [24.61849315,
# 0,
# 0,
# 0,
# 0]]

def autolabel(ax, rects, num, color):
        # attach some text labels
        for rect in rects:
            height = rect.get_height()
            ax.text(rect.get_x(), height + 0.05, '%.1lfx' % num, color=color, fontsize=15, weight='bold', ha='left', va='bottom')
width = 0.16
x = [0, 1, 2, 3, 4]
title = ['LLaMA-30B', 'OPT-30B']
systems = ['vLLM', 'HuggingFace TGI', 'FasterTransformer', 'Collie w/ Incremental Decoding', 'Collie']

title = ['OPT-6B', 'OPT-13B', 'OPT-30B']
systems =['FlexGen', 'Collie']

fig, axes = plt.subplots(ncols = 3, nrows = 1)
plt.subplots_adjust(wspace=0.3)  # 你可以根据需要调整这个值
fig.set_size_inches(15, 5)  # 设置整个图形的宽度和高度

for i in [0,1,2]:
    # print("i:",i)
	b1 = axes[i].bar(np.array(x)-2*width, np.array(spec_infer[i]), width, color = c5, edgecolor = "white")
	# b2 = axes[i].bar(np.array(x)-width, np.array(hf_tgi[i]), width, color = c2, edgecolor = "white")
	# b3 = axes[i].bar(np.array(x), np.array(ft[i]), width, color = c1, edgecolor="white")
	# b4 = axes[i].bar(np.array(x)+width, np.array(inc_decoding[i]), width, color = c4, edgecolor="white")
    #print(i)
	b5 = axes[i].bar(np.array(x) - width, np.array(FlexGen[i]), width, color = c3, edgecolor="white")

	#axes[i].axhline(y=metaflow[i], color = 'r', xmin = 3.5/7.5, xmax = 1, lw=2)
	axes[i].set_xlabel(title[i], fontsize = 14)
	axes[i].set_xlim(-3*width, 3*width)
	axes[i].tick_params(axis='both', which='major', labelsize=12)
	#axes[i].set_xticklabels(['A','B','C','D','E','F','G'], fontsize=12)

#plt.xticks(np.array(x) + 1.5 * width, ('GCN', 'GIN', 'GAT'))
plt.setp(axes, xticks=[0,1,2,3, 4], xticklabels=['BS=1','BS=2','BS=4','BS=8','BS=16'])
fig.text(0.02, 0.5, 'Per-token latency (ms)', fontweight='bold', ha='left', va='center', rotation='vertical', fontsize=14)
#fig.text(0.5, 0.02, 'Number of GPU devices', fontweight='bold', ha='center', va='bottom',  fontsize=18)
# fig.legend([b1, b2, b3, b4, b5], systems, loc = 'upper center', fontsize = 14, ncol = 3)
#fig.text(0.5, 0.02, 'Number of GPU devices', fontweight='bold', ha='center', va='bottom',  fontsize=18)
fig.legend([b1, b5], systems, loc = 'upper center', fontsize = 14, ncol = 3)
plt.show()
plt.savefig('bs_offload.pdf', bbox_inches='tight')