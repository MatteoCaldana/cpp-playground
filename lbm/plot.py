import numpy as np 
import os
import matplotlib.pyplot as plt

plt.close('all')


def load_u():
    files = os.listdir(".")
    files = [file for file in files if file.startswith("u") and file.endswith(".txt")]
    files.sort()
    assert len(files) % 2 == 0
    us = []
    for i in range(0, len(files), 2):
        ux = np.loadtxt(files[i])
        uy = np.loadtxt(files[i+1])
        us.append(np.stack([ux, uy]))

    u = np.stack(us)
    print(u.shape)
    return u

def plot_test(u):
    nx, ny = u.shape[2], u.shape[3]
    plt.plot()
    plt.plot(np.linspace(0, 1, nx), u[-1, 1, :, ny//2])
    plt.plot(np.linspace(0, 1, ny), u[-1, 0, nx//2, :])

def plot_last(u):
    fig, axs = plt.subplots(1, 3, figsize=(15, 4))
    cs = axs[0].contourf(u[-1, 0, :, :], cmap='jet')
    fig.colorbar(cs, ax=axs[0], shrink=0.9)
    cs = axs[1].contourf(u[-1, 1, :, :], np.linspace(-.1, .1, 20), cmap='jet')
    fig.colorbar(cs, ax=axs[1], shrink=0.9)
    cs = axs[2].contourf(u[-1, 0, :, :]**2 + u[-1, 1, :, :]**2, cmap='jet')
    fig.colorbar(cs, ax=axs[2], shrink=0.9)
    plt.show()


def plot_animation(u):
    fig, axs = plt.subplots(1, 3, figsize=(15, 4))
    u2 = u[:, 0, :, :]**2 + u[:, 1, :, :]**2
    
    for i in range(u.shape[0]):
        print(i)
        cs = axs[0].contourf(u[i, 0], cmap='jet')
        cs = axs[1].contourf(u[i, 1], np.linspace(-.1, .1, 20), cmap='jet')        
        cs = axs[2].contourf(u2[i], np.linspace(0, np.max(u2), 20), cmap='jet')
        plt.pause(0.01)
    plt.show() 
     


u = load_u()
plot_animation(u)