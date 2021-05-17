import os

import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

sns.set()

X = None  # TODO: remove this
N = len(X)

output_dir = 'output'
os.makedirs('output', exist_ok=True)

step = 5
for ii in range(N // step + 1):
    i = ii * step
    fig = plt.figure(figsize=(8, 4), dpi=120)

    # Generated data
    sns.histplot(X[:i], stat='density',
                 label='Generated Data',
                 linewidth=0)
    sns.scatterplot(x=X[:i], y=np.zeros(i),
                    marker='.', color='red', linewidth=0,
                    zorder=5)

    # True PDF f(x)
    x = np.linspace(start=0, stop=5, num=1000)
    f = None  # TODO: add pdf
    sns.lineplot(x=x, y=f, label='True PDF')

    plt.legend()
    plt.xlim(-0.1, 5)  # TODO: change x-limit
    plt.ylim(-0.1, 2.1)  # TODO: change y-limit
    plt.savefig(f'{output_dir}/{i:05d}')
    plt.close()
