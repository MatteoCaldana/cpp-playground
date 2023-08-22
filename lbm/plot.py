import numpy as np 
import os
import matplotlib.pyplot as plt
import re

plt.close('all')


def load_u():
    files = os.listdir(".")
    files = [file for file in files if re.match(r"u\.[0-9]{6}\.[0-1]\.txt", file)]
    files.sort()
    us = []
    for i in range(0, len(files), 2):
        assert files[i][:8] == files[i+1][:8]
        ux = np.loadtxt(files[i])
        uy = np.loadtxt(files[i+1])
        us.append(np.stack([ux, uy]))

    u = np.stack(us)
    print(u.shape)
    return u


def load_u_binary():
    files = os.listdir(".")
    files = [file for file in files if re.match(r"u\.[0-9]{6}\.txt", file)]
    files.sort()
    us = []
    for file in files:
        u = np.fromfile(file, dtype=np.float64, sep="")
        us.append(u.reshape((2, 100, 100)))

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
    ux_lvls = np.linspace(np.min(u[:, 0]), np.max(u[:, 0]), 20)
    uy_lvls = np.linspace(np.min(u[:, 1]), np.max(u[:, 1]), 20)
    u2 = u[:, 0, :, :]**2 + u[:, 1, :, :]**2
    u2_lvls = np.linspace(0, np.max(u2), 20) 
    
    for i in range(u.shape[0]):
        print(i)
        cs1 = axs[0].contourf(u[i, 0], ux_lvls, cmap='jet')
        cs2 = axs[1].contourf(u[i, 1], uy_lvls, cmap='jet')        
        cs3 = axs[2].contourf(u2[i], u2_lvls, cmap='jet')
        if i == 0:
            fig.colorbar(cs1, ax=axs[0], shrink=0.9)
            fig.colorbar(cs2, ax=axs[1], shrink=0.9)
            fig.colorbar(cs3, ax=axs[2], shrink=0.9)
        plt.pause(0.01)
    plt.show() 
     


u = load_u_binary()
plot_animation(u)