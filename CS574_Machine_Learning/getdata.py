# -*- coding: utf-8 -*-
"""
Created on Thu Nov 20 14:27:47 2014
"""


import numpy as N
import nibabel
import glob
from scipy.ndimage.interpolation import zoom


def getdata():
    rdn = checksize()
    num = len(rdn)

    index = N.zeros((num))
    centers = N.zeros((num,2))
    boundbox = N.zeros((num,2))

    lis = glob.glob("./data/DJ_RECURATED/*")
    for i in range(num):    
        lis1 = glob.glob(lis[rdn[i]]+"/*"+"/*.nii.gz")[0]
#        lis2 = glob.glob(lis[rdn[i]]+"\\*"+"\\BACKGROUND_IMAGES\\*.nii.gz")[0]
        
        mask_mtx = nibabel.load(lis1).get_data()
#        bg_mtx = nibabel.load(lis2).get_data()
        
        size = mask_mtx.shape[2]
        masksum = N.zeros((size))
        for j in range(size):
            masksum[j] = mask_mtx[:,:,j].sum()
        index[i] = masksum.tolist().index(masksum.max()) 
        mask = mask_mtx[:,:,index[i]]
        m,n = mask.shape
        if m==512 and n == 512:
            mask = zoom(mask,0.5)
        m,n = mask.shape
        msum = mask.sum()
        contours = N.zeros((msum,2))
        counter = 0
        for j in range(m):
            for k in range(n):
                if mask[j,k] == 1:
                    contours[counter,0] = j
                    contours[counter,1] = k
                    counter += 1
                    
        boundbox[i] = contours.max(0)-contours.min(0)
        centers[i] = contours.mean(0)
        
    boundmn = boundbox.max(0)

    print boundbox[:,0].tolist().index(boundmn[0])
    print boundbox[:,1].tolist().index(boundmn[1])
    print boundmn
    
    trainingdata = N.zeros((num,round(boundmn[0]),round(boundmn[1]))) 
    
    for k in range(num):
  
#        lis1 = glob.glob(lis[rdn[k]]+"\\*"+"\\*.nii.gz")[0]
        lis2 = glob.glob(lis[rdn[k]]+"/*"+"/BACKGROUND_IMAGES/*.nii.gz")[0]
        
#        mask_mtx = nibabel.load(lis1).get_data()
        bg_mtx = nibabel.load(lis2).get_data()
        
        bg = bg_mtx[:,:,index[k]]

        if len(bg.shape)>2:
            bg = bg_mtx[:,:,index[k],0]
            p,q = bg.shape
        else:
            p,q = bg.shape
        if p==512 and q ==512:
            bg = zoom(bg,0.5)
        
        p,q = bg.shape
        print rdn[k], index[k]
        print bg.shape
        x1 = (round(centers[k][0]) - round(boundmn[0])/2.0)
        x2 = (round(centers[k][0]) + round(boundmn[0])/2.0)
        y1 = (round(centers[k][1]) - round(boundmn[1])/2.0)
        y2 = (round(centers[k][1]) + round(boundmn[1])/2.0)
        
        if x1>=0 and x2<=p and y1>=0 and y2<=q:
            trainingdata[k] = bg[x1:x2,y1:y2]
            print 0

        if x1<0 and x2<=p and y1>=0 and y2<=q:
            xx = abs(x1)
            x1=0
#            trainingdata[k,:x2,:] = bg[x1:x2,y1:y2]
            trainingdata[k,abs(xx):,:] = bg[x1:x2,y1:y2]
            trainingdata[k,:abs(xx),:] = trainingdata[k,abs(xx),:].reshape(1,boundmn[1])
            print 1
        if x1>=0 and x2>p and y1>=0 and y2<=q:
            x2=p
            trainingdata[k,:(x2-x1+1),:] = bg[x1:x2,y1:y2]
            trainingdata[k,(x2-x1+1):,:] = trainingdata[k,(x2-x1),:].reshape(1,boundmn[1])
            print 2
        if x1>=0 and x2<=p and y1<0 and y2<=q:
            yy = abs(y1)
            y1 = 0
#            trainingdata[k,:,:y2] = bg[x1:x2,y1:y2]
            trainingdata[k,:,(abs(yy)+1):] = bg[x1:x2,y1:y2]
            trainingdata[k,:,:(abs(yy)+1)] = trainingdata[k,:,(abs(yy)+1)].reshape(boundmn[0],1)
            print 3
        if x1>=0 and x2<=p and y1>=0 and y2>q:
            y2=q
            trainingdata[k,:,:(y2-y1+1)] = bg[x1:x2,y1:y2]
            trainingdata[k,:,(y2-y1+1):] = trainingdata[k,:,(y2-y1)].reshape(boundmn[0],1)
            print 4
        if x1<0 and x2<=p and y1<0 and y2<=q:
            x1=0
            y1=0
            trainingdata[k,:x2,:y2] = bg[x1:x2,y1:y2]
            print 5
        if x1<0 and x2<=p and y1>=0 and y2>q:
            x1=0
            y2=q
            trainingdata[k,:x2,:(y2-y1+1)] = bg[x1:x2,y1:y2]
            print 6
        if x1>=0 and x2>p and y1<0 and y2<=q:
            x2=p
            y1=0
            trainingdata[k,:(x2-x1+1),:y2] = bg[x1:x2,y1:y2]
            print 7
        if x1>=0 and x2>p and y1>=0 and y2>q:
            x2=p
            y2=q
            trainingdata[k,:(x2-x1+1),:(y2-y1+1)] = bg[x1:x2,y1:y2]
            print 8
    return trainingdata
    
#    return trainingdata,centers,boundbox,index,rdn
#    return centers,boundbox,boundmn,index,rdn


def checksize():
    index = N.zeros((185))
    count = N.zeros((185))
    lis = glob.glob("./data/DJ_RECURATED/*")
    for i in range(185):
        lis1 = glob.glob(lis[i]+"/*"+"/*.nii.gz")[0]
        mask_mtx = nibabel.load(lis1).get_data()
        m,n,r = mask_mtx.shape
        if (m == 256 and n == 256) or (m == 512 and n == 512):
            index[i]=i
            count[i]=1
    index[0]=1
    new = index[N.nonzero(index)].astype(N.int16)
    new[0]=0
    
    return new
    
if __name__ == "__main__":
    traindata = getdata()
