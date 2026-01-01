# -*- coding: utf-8 -*-
"""
Created on Tue Dec 16 11:47:56 2025

@author: Rupert
"""

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import AutoMinorLocator, MultipleLocator
from scipy.interpolate import splprep, splev
from moviepy.video.VideoClip import VideoClip

colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd',
          '#8c564b', '#e377c2', '#7f7f7f', '#bcbd22', '#17becf']

def Fourier_frame(xs, ys, phi, rs, phis, ns, lims, bins, ax, xss, yss):
    
    ax.clear()
    
    x0, y0 = xs, ys
    
    for i in range(len(rs)):
        
        lw0 = 1.5*rs[i]/rs[0]
        ms0 = 15*rs[i]/rs[0]
        
        i0 = i%10
        
        # circle center
        ax.plot([x0], [y0], marker=".", ms=ms0, c=colors[i0]) 
        
        # circle
        theta = np.linspace(0, 2*np.pi, 100, endpoint=True)
        _x = x0 + rs[i]*np.cos(theta)
        _y = y0 + rs[i]*np.sin(theta)
        ax.plot(_x,_y, c=colors[i0], lw=lw0, ls="-.")
        
        # arrow
        _phi = ns[i]*phi + phis[i]
        x_end = x0 + rs[i]*np.cos(_phi)
        y_end = y0 + rs[i]*np.sin(_phi)
        
        start_point = (x0, y0)
        end_point = (x_end, y_end)
        
        ax.annotate('',  xy=end_point, xytext=start_point,
                    arrowprops=dict(arrowstyle='->', lw=lw0, 
                                    color=colors[i0],
                                    shrinkA=0, shrinkB=0))
        
        # update the circle center
        x0, y0 = x_end, y_end
        
    if len(xss) > 1:
        ax.plot([xss[-1]], [yss[-1]], marker=".", ms=10, c="k")
        ax.plot(xss, yss, lw=1.1, c="k")
    
    edge_width = 1.1
    ax.set_aspect("equal")
    ax.set_xlim(lims[0], lims[1])
    ax.set_ylim(lims[2], lims[3])
    ax.xaxis.set_ticks_position('both')
    ax.yaxis.set_ticks_position('both')
    ax.tick_params(which='both', direction='out')
    ax.tick_params(which='major', labelsize=14)
    ax.tick_params(which='minor', labelsize=14)
    ax.tick_params(which='major', direction='in', length=5, width=edge_width)
    ax.tick_params(which='minor', direction='in', length=3, width=edge_width)
    
    ax.tick_params(labelbottom=False, labelleft=False)
    
    ax.xaxis.set_major_locator(MultipleLocator(bins[0]))
    ax.yaxis.set_major_locator(MultipleLocator(bins[1]))
    ax.xaxis.set_minor_locator(AutoMinorLocator(bins[2]))
    ax.yaxis.set_minor_locator(AutoMinorLocator(bins[3]))

    for posi in ['right','left','bottom','top']:
        ax.spines[posi].set_linewidth(edge_width)
                
    return x0, y0

def mplfig_to_npimage(fig):
    
    fig.canvas.draw()
    buf = fig.canvas.buffer_rgba()
    image = np.asarray(buf).copy()
    
    return image

def CreateGif(xs, ys, rs, phis, ns, lims, bins, duration):
    
    fig = plt.figure(dpi=150, figsize=(6,6))
    ax = fig.gca()
    
    xss, yss = [], []
    
    def make_frame(t):
        
        phi = t*2*np.pi/duration
        
        x0, y0 = Fourier_frame(xs, ys, phi, rs, phis, ns, lims, bins, ax, xss, yss)
        
        fig.subplots_adjust(wspace=0, hspace=0, 
                            left=0.05, right=0.95, bottom=0.05, top=0.95)
        
        xss.append(x0)
        yss.append(y0)
        
        return mplfig_to_npimage(fig)
    
    animation = VideoClip(make_frame, duration=duration)
    
    animation.write_gif("fourier.gif", fps=30) 
    plt.close(fig)
    
    return animation

def getFourier(x0s, y0s, order0):
    
    zs = np.asarray(x0s) + 1j*np.asarray(y0s)
    Ns = len(zs)
    
    Zs = np.fft.fft(zs)/Ns
    
    xs0 = Zs[0].real
    ys0 = Zs[0].imag
    
    orders = min(Ns//2-1, order0)
    
    rs = []
    phis = []
    ns = []

    for n in range(1, orders+1):
        c_pos = Zs[n]
        c_neg = Zs[-n]

        rs.append(np.abs(c_pos))
        phis.append(np.angle(c_pos))
        ns.append(+n)

        rs.append(np.abs(c_neg))
        phis.append(np.angle(c_neg))
        ns.append(-n)

    return xs0, ys0, np.array(rs), np.array(phis), np.array(ns)

def getPoint(image=None, max0=10):
    
    xs, ys = [], []
    
    fig = plt.figure(dpi=150)
    ax = fig.gca()
    
    ax.set_xlim(-10, 10)
    ax.set_ylim(-10, 10)
    
    if image is not None:
        img = plt.imread(image)
        ax.imshow(img, extent=[-max0, max0, -max0, max0])
    
    ax.set_aspect("equal")
    ax.grid(True)
    
    # get points from mouse
    def onclick(event):
        if event.inaxes is None:
            return
        
        if event.button == 1:
            xs.append(float(event.xdata))
            ys.append(float(event.ydata))
            ax.plot(event.xdata, event.ydata, 'ro', ms=8)
            fig.canvas.draw()
        elif event.button == 3:
            plt.close(fig)

    fig.canvas.mpl_connect('button_press_event', onclick)
    
    plt.show(block=True)
    
    # interpolate
    tck, _ = splprep([xs, ys], s=0, per=True)
    u_new = np.linspace(0, 1, 1000)
    xi, yi = splev(u_new, tck)
    
    return xi, yi


if __name__=="__main__":
    
    lims0 = [-10, 10, -10, 10]
    bins0 = [2, 2, 2, 2]
    duration = 10
    edge = 10
    orders = 50
    
    image = "figs/xin.png"
    x0s, y0s = getPoint(image=image, max0=edge)    
    xs0, ys0, rs0, phis0, ns0 = getFourier(x0s, y0s, orders)
    CreateGif(xs0, ys0, rs0, phis0, ns0, lims0, bins0, duration)
