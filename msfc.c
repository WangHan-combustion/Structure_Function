/*
* Yongxiang Huang, last modification: 28/03/2010
* yongxianghuang@gmail.com
*
*/

/* This function is to estimate the structure function scaling by considering a sign power
 * The  structure function is further termed into four part
 * S_1(\tau) (sfP) is the structure function with positive contribution
 * S_2(\tau) (sfN) is the structure function with negative contribution
 * S_3(\tau) (sfO) is the structure function with all contribution (|P|+|N|)
 * S_4(\tau) (sfM) is the structure function with all contribution (|P|-|N|)
 */

#include <stdlib.h>
#include <stdio.h>
#include "mex.h"
#include <math.h>






/************************************************************************/
/*                                                                      */
/* MAIN FUNCTION                                                        */
/*                                                                      */
/************************************************************************/

void mexFunction(int nlhs,mxArray *plhs[],int nrhs,const mxArray *prhs[]) {
  
    /* declarations */
    int i,j,m,n,k,k1,k2,k3,k4,k5,tn,Nx,Ny,NTau,Nm;
    
    double *qm,*qn,*sfN,*sfP,*sfO,*sfM,*Tau,*tmpx,*tmpy,*Nk,*q,*Xsign,*Ysign,*x,*y; 
    /*instantaneous frequency, instantaneous amplitude, resolution of amplitude resolution frequency*/
      
    
/*     check input*/
    if (nrhs!=5)     mexErrMsgTxt("You have to input five parameters!");
    if (mxIsEmpty(prhs[0]))mexErrMsgTxt("The first time series is empty!");
    if (mxIsEmpty(prhs[1]))mexErrMsgTxt("The second time series is empty!");
    if (mxIsEmpty(prhs[2]))mexErrMsgTxt("The order m is empty!");
    if (mxIsEmpty(prhs[3]))mexErrMsgTxt("The order n is empty!");
    if (mxIsEmpty(prhs[4]))mexErrMsgTxt("The time delay is empty!");

    /* get input data */
    x=mxGetPr(prhs[0]);
    Nx=mxGetN(prhs[0]);
    tn=mxGetM(prhs[0]);
    if (tn>Nx) Nx=tn;
    
    y=mxGetPr(prhs[1]);
    Ny=mxGetN(prhs[1]);
    tn=mxGetM(prhs[1]);
    if (tn>Ny) Ny=tn;
    
    if (Nx!=Ny)mexErrMsgTxt("The length of two time series should be the same.");
    
    
    
    qm=mxGetPr(prhs[2]);
    Nm=mxGetN(prhs[2]);
    tn=mxGetM(prhs[2]);
    if (tn>Nm) Nm=tn;
    
    if (Nm>1)mexErrMsgTxt("The length of statistical order m should be one.");
    
    qn=mxGetPr(prhs[3]);
    Nm=mxGetN(prhs[3]);
    tn=mxGetM(prhs[3]);
    if (tn>Nm) Nm=tn;
    
    if (Nm>1)mexErrMsgTxt("The length of statistical order n should be one.");
    
    Tau=mxGetPr(prhs[4]);
    NTau=mxGetN(prhs[4]);
    tn=mxGetM(prhs[4]);
    if (tn>NTau) NTau=tn;
    
    if (Nm>1)mexErrMsgTxt("The length of statistical order n should be one.");
    
    
    
    Xsign=(double *)malloc(Nx*sizeof(double)); /*specify sign*/
    Ysign=(double *)malloc(Nx*sizeof(double)); /*specify sign*/

    tmpx=(double *)malloc(Nx*sizeof(double)); /*specify tmp for moments*/
    tmpy=(double *)malloc(Nx*sizeof(double)); /*specify tmp for moments*/
    
    plhs[0]=mxCreateDoubleMatrix(1,NTau,mxREAL);
    sfP=mxGetPr(plhs[0]);
    plhs[1]=mxCreateDoubleMatrix(1,NTau,mxREAL);
    sfN=mxGetPr(plhs[1]);
    plhs[2]=mxCreateDoubleMatrix(1, NTau, mxREAL);
    sfO=mxGetPr(plhs[2]);
    plhs[3]=mxCreateDoubleMatrix(1,NTau,mxREAL);
    sfM=mxGetPr(plhs[3]);
    
    plhs[4]=mxCreateDoubleMatrix(2,NTau,mxREAL);
    Nk=mxGetPr(plhs[4]);
     
  
  
    for(i=0;i<NTau;i++)/*Bigest loop for for different time delay*/
    {
        k2=Tau[i];  /*separation tau*/
        k1=Nx-k2; /*length of velocity increment*/
      
         
        for(j=0;j<k1;j++) /*velocity increment and its sign*/
        {
            tmpx[j]=x[j+k2]-x[j];
            if(tmpx[j]>0){ Xsign[j]=1.0;}
            if(tmpx[j]<0){ Xsign[j]=-1.0;tmpx[j]=-tmpx[j];}
            if(tmpx[j]==0){ Xsign[j]=0.0;}
            tmpx[j]=pow(tmpx[j], qm[0]);

            tmpy[j]=y[j+k2]-y[j];
            if(tmpy[j]>0){ Ysign[j]=1.0;}
            if(tmpy[j]<0){ Ysign[j]=-1.0;tmpy[j]=-tmpy[j];}
            if(tmpy[j]==0){ Ysign[j]=0.0;}
            tmpy[j]=pow(tmpy[j], qn[0]);

        }
        
        k4=0;
        k5=0;
        

        for(j=0;j<k1;j++)/*loop for average*/
        {
                
            if(Xsign[j]*Ysign[j]>0)
            {
                sfP[i]=sfP[i]+tmpx[j]*tmpy[j];
                k4=k4+1;
            }
            if(Xsign[j]*Ysign[j]<0)
            {
                sfN[i]=sfN[i]+tmpx[j]*tmpy[j];
                k5=k5+1;
            }
        }/*loop for average*/
           
        Nk[i*2]=k4/k1;
        Nk[i*2+1]=k5/k1;
        
        sfO[i]=(sfP[i]+sfN[i])/k1;
        sfM[i]=(sfP[i]-sfN[i])/k1;
        sfP[i]=sfP[i]/k4;
        sfN[i]=sfN[i]/k5;
        
       
    }
       
      
    mxFree(Xsign);
    mxFree(tmpx);
    mxFree(tmpy);
    mxFree(Ysign);
}