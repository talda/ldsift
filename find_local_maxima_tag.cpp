#include <mex.h>
#include <string.h>
#include <math.h>

#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))

void mexFunction(
        int nlhs,
        mxArray *plhs[],
        int nrhs,
        const mxArray *prhs[]
        ) {

    enum INPUTS {
        I_oct_inds = 0,
        I_diff,
        I_W,
        I_NUM_INPUTS
    };

    enum OUTPUTS {
        O_MAXIMA,
        O_NUM_OUTPUTS
    };

    char strError[100];

    if (nrhs != I_NUM_INPUTS) {
        sprintf(strError, "Only %d input arguments allowed.",I_NUM_INPUTS);
        mexErrMsgTxt(strError);
    } else if (nlhs != O_NUM_OUTPUTS) {
        mexErrMsgTxt("Too many output params");
    }

    if (mxGetM(prhs[I_oct_inds]) != 1){
        mexErrMsgTxt("Incorrect oct_inds size");
    }
    
    int nNumOctaves = mxGetN(prhs[I_diff]);
    int nNumVertex = mxGetN(prhs[I_W]);

    if (mxGetN(prhs[I_W]) != mxGetM(prhs[I_W])){
        mexErrMsgTxt("W sould be square");
    }

    if (!mxIsSparse(prhs[I_W])){
        mexErrMsgTxt("W sould be sparse");
    }

    mwIndex dims[1] = {nNumOctaves};
    plhs[0] = mxCreateCellArray(1, dims);


    double *pOctInds = mxGetPr(prhs[I_oct_inds]);
    double *pDiff = mxGetPr(prhs[I_diff]);
    mwIndex *pWjc = mxGetJc(prhs[I_W]);
    mwIndex *pWir = mxGetIr(prhs[I_W]);
    double *pOctMaximas = new double[nNumVertex];
    const mxArray *pW = prhs[I_W];
    mxArray *pOctRes;
    for (int nOctave = 0; nOctave < nNumOctaves - 1; nOctave++) {
        double dMaxDiff = 0;
        int nOctStart = nOctave * nNumVertex;
//        for (int nInd2 = 0; nInd2 < nNumVertex; nInd2++) {
//            dMaxDiff = MAX(dMaxDiff, pDiff[nOctStart + nInd2]);
//        }
        //mexPrintf("Octave = %d max = %f\n",nOctave, dMaxDiff);
        
        int nNumMaximas = 0;
        for (int nInd1 = 0; nInd1 < nNumVertex; nInd1++) {

//            if (pDiff[nOctStart + nInd1] < dMaxDiff * 0.015)
//                continue;
            
            if (abs(pOctInds[nInd1] - (nOctave + 1)) > 0.1)
                continue;

            if ((nOctave < nNumOctaves - 1) && (pDiff[nOctStart + nInd1] < pDiff[nOctStart + nNumVertex + nInd1]))
                continue;

            if ((nOctave > 0) && (pDiff[nOctStart + nInd1] < pDiff[nOctStart - nNumVertex + nInd1]))
                continue;
            mwIndex starting_row_index, stopping_row_index;

            starting_row_index = pWjc[nInd1];
            stopping_row_index = pWjc[nInd1+1];

            if (starting_row_index == stopping_row_index)
                continue;

            double dDiffMax = 0;
            for (int nOct = MAX(nOctave-1,0); nOct < MIN(nOctave+2,nNumOctaves); nOct++) {
                for (mwIndex nWind = starting_row_index; nWind < stopping_row_index; nWind++) {
                    int nIndex = nOct * nNumVertex +pWir[nWind];
                    dDiffMax = MAX(dDiffMax, pDiff[nIndex]);
                }
            }

            if (pDiff[nOctStart + nInd1] < dDiffMax)
                continue;

            pOctMaximas[nNumMaximas++] = nInd1 + 1;

        }
        if (nNumMaximas > 0) {
            pOctRes = mxCreateDoubleMatrix(1, nNumMaximas, mxREAL);
            memcpy(mxGetPr(pOctRes), pOctMaximas, (nNumMaximas) * sizeof (double));
            mxSetCell(plhs[0], nOctave, pOctRes);
        }
    }
    delete[] pOctMaximas;

}

