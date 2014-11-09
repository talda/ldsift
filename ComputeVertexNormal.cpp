#include "mex.h"

#include "string.h"
#include "math.h"

void mexFunction(
        int nlhs,
        mxArray *plhs[],
        int nrhs,
        const mxArray *prhs[]
        ) {

    enum INPUTS {
        I_FACE = 0,
        I_NORMALF,
        I_NVERT,
        I_LAST
    };

    if (nrhs != I_LAST) {
        mexErrMsgTxt("Only 3 input arguments allowed.");
    } else if (nlhs != 1) {
        mexErrMsgTxt("Only 1 output arguments allowed.");
    } 

    if (mxGetM(prhs[I_FACE]) != 3){
        mexErrMsgTxt("Error with face array width");
    }

    if (mxGetM(prhs[I_NORMALF]) != 3){
        mexErrMsgTxt("Error with normal array width");
    }
    
    //mexPrintf("M = %d, N = %d",)
    
    if (mxGetM(prhs[I_NVERT]) != 1 || mxGetN(prhs[I_NVERT]) != 1 ){
        mexErrMsgTxt("nvert should be scalar");
    }

    
//    normal = zeros(3,nvert);
//for i=1:nface
//    f = face(:,i);
//    for j=1:3
//        normal(:,f(j)) = normal(:,f(j)) + normalf(:,i);
//    end
//end
    
    int nvert = mxGetScalar(prhs[I_NVERT]);
    int nface = mxGetN(prhs[I_FACE]);
    mxArray *normal = mxCreateDoubleMatrix(3, nvert, mxREAL);
    double *pNormal = mxGetPr(normal);
    double* pFaces = mxGetPr(prhs[I_FACE]);
    double* pNormalF = mxGetPr(prhs[I_NORMALF]);
    double f[3];
    
    for (int i = 0; i < nface; i++) {
        memcpy(&f, &pFaces[i*3],3*sizeof(double));
        //mexPrintf("f = %d %d %d\n", int(f[0]),int(f[1]),int(f[2]));
        for (int j = 0; j < 3; j++) {
            for (int l = 0; l < 3; l++) {
                int nIndex = floor(f[j]-1)*3 + l;
                char err[1000];
                if (nIndex > nvert*3) {
                    sprintf(err,"damn: nIndex = %d, f(j) = %d, i=%d, j=%d l=%d", nIndex, int(f[j]),i,j,l);
                    mexErrMsgTxt(err);
                }
                pNormal[nIndex] += pNormalF[i*3+l];
            }
        }
   }
    plhs[0] = normal;
}

