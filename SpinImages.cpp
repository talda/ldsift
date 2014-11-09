#include "mex.h"
#include "string.h"
#include <math.h>

#define PI_2   1.57079632679489661923	/* pi/2 */
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))

void mexFunction(
        int nlhs,
        mxArray *plhs[],
        int nrhs,
        const mxArray *prhs[]
        ) {

    enum INPUTS {
        I_VERTEX = 0,
        I_NORMALS,
        I_PTS,
        I_BINSIZE,
        I_SIZE
    };

    if (nrhs != 5) {
        mexErrMsgTxt("Only 5 input arguments allowed.");
    } else if (nlhs > 1) {
        mexErrMsgTxt("Too many output params");
    }

    if (mxGetN(prhs[I_VERTEX]) != 3){
        mexErrMsgTxt("Incorrect vertex size");
    }

    if (mxGetN(prhs[I_NORMALS]) != 3) {
        mexErrMsgTxt("Incorrect normals size");
    }

    if (mxGetN(prhs[I_BINSIZE]) != 1) {
        mexErrMsgTxt("Incorrect bin size");
    }

    if (mxGetN(prhs[I_PTS]) != 1) {
        mexErrMsgTxt("Incorrect pts size");

    }

    if (mxGetN(prhs[I_SIZE]) != 1 || mxGetM(prhs[I_SIZE]) != 1) {
        mexErrMsgTxt("Incorrect image size size");
    }

    if (mxGetClassID(prhs[I_VERTEX]) != mxDOUBLE_CLASS || mxGetClassID(prhs[I_NORMALS]) != mxDOUBLE_CLASS || mxGetClassID(prhs[I_PTS]) != mxDOUBLE_CLASS || mxGetClassID(prhs[I_BINSIZE]) != mxDOUBLE_CLASS || mxGetClassID(prhs[I_SIZE]) != mxDOUBLE_CLASS) {
        mexErrMsgTxt("Only double arrays are supported");
    }

    int nNumVertex = (int) mxGetM(prhs[I_VERTEX]);
    if (mxGetM(prhs[I_NORMALS]) != nNumVertex || mxGetM(prhs[I_BINSIZE]) != nNumVertex) {
        mexErrMsgTxt("Inconsistent vector lengths");
    }


    double *pVertex = mxGetPr(prhs[I_VERTEX]);
    double *pNormals = mxGetPr(prhs[I_NORMALS]);

    int nNumIndices = (int) mxGetM(prhs[I_PTS]);
    double *pPtsIndices = mxGetPr(prhs[I_PTS]);

    double *pBinSize = mxGetPr(prhs[I_BINSIZE]);
    int nSpinImageSize = (int) floor(*mxGetPr(prhs[I_SIZE]));

    mxArray *pOutArray = mxCreateCellMatrix(1, nNumIndices);

    for (int nPtsIndex = 0; nPtsIndex < nNumIndices; nPtsIndex++) {
        int nPtIndex = (int) pPtsIndices[nPtsIndex] - 1;
        double Pt[3] = {pVertex[nPtIndex], pVertex[nPtIndex + nNumVertex], pVertex[nPtIndex + nNumVertex * 2]};
        double Normal[3] = {pNormals[nPtIndex], pNormals[nPtIndex + nNumVertex], pNormals[nPtIndex + nNumVertex * 2]};

        mxArray* pSpinImageArr = mxCreateDoubleMatrix(nSpinImageSize, nSpinImageSize, mxREAL);
        double *pSpinImage = mxGetPr(pSpinImageArr);
        memset(pSpinImage, 0, nSpinImageSize * nSpinImageSize * sizeof (double));

        int nSum = 0;
        for (int nVertIndex = 0; nVertIndex < nNumVertex; nVertIndex++) {

            double ThisPt[3] = {pVertex[nVertIndex], pVertex[nVertIndex + nNumVertex], pVertex[nVertIndex + nNumVertex * 2]};

            double ThisNormal[3] = {pNormals[nVertIndex], pNormals[nVertIndex + nNumVertex], pNormals[nVertIndex + nNumVertex * 2]};
            double dTemp = MAX(-1, MIN(1, (Normal[0] * ThisNormal[0] + Normal[1] * ThisNormal[1] + Normal[2] * ThisNormal[2])));
            double dAng = acos(dTemp);

            if (dAng > PI_2)
                continue;

            double PtDiff[3] = {ThisPt[0] - Pt[0], ThisPt[1] - Pt[1], ThisPt[2] - Pt[2]};
            double dCross[3];

            dCross[0] = PtDiff[1] * Normal[2] - PtDiff[2] * Normal[1];
            dCross[1] = PtDiff[2] * Normal[0] - PtDiff[0] * Normal[2];
            dCross[2] = PtDiff[0] * Normal[1] - PtDiff[1] * Normal[0];

            double dAlpha = sqrt(dCross[0] * dCross[0] + dCross[1] * dCross[1] + dCross[2] * dCross[2]);
            double dBeta = Normal[0] * PtDiff[0] + Normal[1] * PtDiff[1] + Normal[2] * PtDiff[2];

            dBeta -= (nSpinImageSize * pBinSize[nPtIndex]) / 2.0;

            if (dBeta > 0 || dBeta <= (-nSpinImageSize * pBinSize[nPtIndex]))
                continue;

            double dA = floor(dAlpha / pBinSize[nPtIndex]);
            if (dA >= nSpinImageSize)
                continue;

            double dB = floor(-dBeta / pBinSize[nPtIndex]);

            int nSIIndex = int(dA + dB * nSpinImageSize);

            if (nSIIndex >= nSpinImageSize * nSpinImageSize) {
                //mexPrintf("bahhhhh %d\n", nSIIndex);
                mexPrintf("bahhhhh %f %f\n", dBeta, pBinSize[nPtIndex]);
                continue;
            }

            pSpinImage[nSIIndex] += 1.0;
            nSum++;
        }
        for (int nIndex = 0; nIndex < nSpinImageSize * nSpinImageSize; nIndex++) {
            pSpinImage[nIndex] /= nSum;
        }
        mxSetCell(pOutArray, nPtsIndex, pSpinImageArr);
    }

    plhs[0] = pOutArray;

}
