// Copyright (c) 2017, Lawrence Livermore National Security, LLC. Produced at
// the Lawrence Livermore National Laboratory. LLNL-CODE-734707. All Rights
// reserved. See files LICENSE and NOTICE for details.
//
// This file is part of CEED, a collection of benchmarks, miniapps, software
// libraries and APIs for efficient high-order finite element and spectral
// element discretizations for exascale applications. For more information and
// source code availability see http://github.com/ceed.
//
// The CEED research is supported by the Exascale Computing Project 17-SC-20-SC,
// a collaborative effort of two U.S. Department of Energy organizations (Office
// of Science and the National Nuclear Security Administration) responsible for
// the planning and preparation of a capable exascale ecosystem, including
// software, applications, hardware, advanced system engineering and early
// testbed platforms, in support of the nation's exascale computing imperative.
#include "raja.hpp"

// *****************************************************************************
template<int Q1, int D1>
static void rMassMultAdd2D_CPU(const int NUM_DOFS_1D,
                               const int NUM_QUAD_1D,
                               const int numElements,
                               const double* restrict dofToQuad,
                               const double* restrict dofToQuadD,
                               const double* restrict quadToDof,
                               const double* restrict quadToDofD,
                               const double* restrict oper,
                               const double* restrict solIn,
                               double* restrict solOut) {
  forall(e,numElements,{
      double sol_xy[Q1][Q1];
      for (int qy = 0; qy < NUM_QUAD_1D; ++qy) {
        for (int qx = 0; qx < NUM_QUAD_1D; ++qx) {
          sol_xy[qy][qx] = 0.0;
        }
      }
      for (int dy = 0; dy < NUM_DOFS_1D; ++dy) {
        double sol_x[Q1];
        for (int qy = 0; qy < NUM_QUAD_1D; ++qy) {
          sol_x[qy] = 0.0;
        }
        for (int dx = 0; dx < NUM_DOFS_1D; ++dx) {
          const double s = solIn[ijkN(dx,dy,e,NUM_DOFS_1D)];
          for (int qx = 0; qx < NUM_QUAD_1D; ++qx) {
            sol_x[qx] += dofToQuad[ijN(qx,dx,NUM_QUAD_1D)]* s;
          }
        }
        for (int qy = 0; qy < NUM_QUAD_1D; ++qy) {
          const double d2q = dofToQuad[ijN(qy,dy,NUM_QUAD_1D)];
          for (int qx = 0; qx < NUM_QUAD_1D; ++qx) {
            sol_xy[qy][qx] += d2q * sol_x[qx];
          }
        }
      }
      for (int qy = 0; qy < NUM_QUAD_1D; ++qy) {
        for (int qx = 0; qx < NUM_QUAD_1D; ++qx) {
          sol_xy[qy][qx] *= oper[ijkN(qx,qy,e,NUM_QUAD_1D)];
        }
      }
      for (int qy = 0; qy < NUM_QUAD_1D; ++qy) {
        double sol_x[D1];
        for (int dx = 0; dx < NUM_DOFS_1D; ++dx) {
          sol_x[dx] = 0.0;
        }
        for (int qx = 0; qx < NUM_QUAD_1D; ++qx) {
          const double s = sol_xy[qy][qx];
          for (int dx = 0; dx < NUM_DOFS_1D; ++dx) {
            sol_x[dx] += quadToDof[ijN(dx,qx,NUM_DOFS_1D)] * s;
          }
        }
        for (int dy = 0; dy < NUM_DOFS_1D; ++dy) {
          const double q2d = quadToDof[ijN(dy,qy,NUM_DOFS_1D)];
          for (int dx = 0; dx < NUM_DOFS_1D; ++dx) {
            solOut[ijkN(dx,dy,e,NUM_DOFS_1D)] += q2d * sol_x[dx];
          }
        }
      }
    });
}

// *****************************************************************************
static void rMassMultAdd2D(const int NUM_DOFS_1D,
                           const int NUM_QUAD_1D,
                           const int numElements,
                           const double* dofToQuad,
                           const double* dofToQuadD,
                           const double* quadToDof,
                           const double* quadToDofD,
                           const double* oper,
                           const double* solIn,
                           double* __restrict solOut) {
  //printf("\033[31m[rMassMultAdd2D] %d\033[m\n",NUM_QUAD_1D);
  //printf("\033[31m[rMassMultAdd2D] %d\033[m\n",NUM_DOFS_1D);
  if (NUM_QUAD_1D==4 && NUM_DOFS_1D==2) {
    if (!is_managed)
      rMassMultAdd2D_CPU<4,2>(NUM_DOFS_1D,NUM_QUAD_1D,numElements,
                              dofToQuad,dofToQuadD,quadToDof,quadToDofD,
                              oper,solIn,solOut);
    else
      rMassMultAdd2D_CPU<4,2>(NUM_DOFS_1D,NUM_QUAD_1D,numElements,
                              dofToQuad,dofToQuadD,quadToDof,quadToDofD,
                              oper,solIn,solOut);
    return;
  }
  if (NUM_QUAD_1D==4 && NUM_DOFS_1D==3) {
    if (!is_managed)
      rMassMultAdd2D_CPU<4,3>(NUM_DOFS_1D,NUM_QUAD_1D,numElements,
                              dofToQuad,dofToQuadD,quadToDof,quadToDofD,
                              oper,solIn,solOut);
    else
      rMassMultAdd2D_CPU<4,3>(NUM_DOFS_1D,NUM_QUAD_1D,numElements,
                              dofToQuad,dofToQuadD,quadToDof,quadToDofD,
                              oper,solIn,solOut);
    return;
  }
  if (NUM_QUAD_1D==16 && NUM_DOFS_1D==3) {
    if (!is_managed)
      rMassMultAdd2D_CPU<16,3>(NUM_DOFS_1D,NUM_QUAD_1D,numElements,
                               dofToQuad,dofToQuadD,quadToDof,quadToDofD,
                               oper,solIn,solOut);
    else
      rMassMultAdd2D_CPU<16,3>(NUM_DOFS_1D,NUM_QUAD_1D,numElements,
                               dofToQuad,dofToQuadD,quadToDof,quadToDofD,
                               oper,solIn,solOut);
    return;
  }
  assert(false);  
}

// *****************************************************************************
static void rMassMultAdd3D(const int NUM_DOFS_1D,
                           const int NUM_QUAD_1D,
                           const int numElements,
                           const double* dofToQuad,
                           const double* dofToQuadD,
                           const double* quadToDof,
                           const double* quadToDofD,
                           const double* oper,
                           const double* solIn,
                           double* __restrict solOut) {
  assert(NUM_QUAD_1D==4);
  assert(NUM_DOFS_1D==4);
  const int Q1 = 4;
  const int D1 = 4;
  forall(e,numElements,{
    double sol_xyz[Q1][Q1][Q1];
    for (int qz = 0; qz < NUM_QUAD_1D; ++qz) {
      for (int qy = 0; qy < NUM_QUAD_1D; ++qy) {
        for (int qx = 0; qx < NUM_QUAD_1D; ++qx) {
          sol_xyz[qz][qy][qx] = 0;
        }
      }
    }
    for (int dz = 0; dz < NUM_DOFS_1D; ++dz) {
      double sol_xy[Q1][Q1];
      for (int qy = 0; qy < NUM_QUAD_1D; ++qy) {
        for (int qx = 0; qx < NUM_QUAD_1D; ++qx) {
          sol_xy[qy][qx] = 0;
        }
      }
      for (int dy = 0; dy < NUM_DOFS_1D; ++dy) {
        double sol_x[Q1];
        for (int qx = 0; qx < NUM_QUAD_1D; ++qx) {
          sol_x[qx] = 0;
        }
        for (int dx = 0; dx < NUM_DOFS_1D; ++dx) {
          const double s = solIn[ijklN(dx,dy,dz,e,NUM_DOFS_1D)];
          for (int qx = 0; qx < NUM_QUAD_1D; ++qx) {
            sol_x[qx] += dofToQuad[ijN(qx,dx,NUM_QUAD_1D)] * s;
          }
        }
        for (int qy = 0; qy < NUM_QUAD_1D; ++qy) {
          const double wy = dofToQuad[ijN(qy,dy,NUM_QUAD_1D)];
          for (int qx = 0; qx < NUM_QUAD_1D; ++qx) {
            sol_xy[qy][qx] += wy * sol_x[qx];
          }
        }
      }
      for (int qz = 0; qz < NUM_QUAD_1D; ++qz) {
        const double wz = dofToQuad[ijN(qz,dz,NUM_QUAD_1D)];
        for (int qy = 0; qy < NUM_QUAD_1D; ++qy) {
          for (int qx = 0; qx < NUM_QUAD_1D; ++qx) {
            sol_xyz[qz][qy][qx] += wz * sol_xy[qy][qx];
          }
        }
      }
    }
    for (int qz = 0; qz < NUM_QUAD_1D; ++qz) {
      for (int qy = 0; qy < NUM_QUAD_1D; ++qy) {
        for (int qx = 0; qx < NUM_QUAD_1D; ++qx) {
          sol_xyz[qz][qy][qx] *= oper[ijklN(qx,qy,qz,e,NUM_QUAD_1D)];
        }
      }
    }
    for (int qz = 0; qz < NUM_QUAD_1D; ++qz) {
      double sol_xy[D1][D1];
      for (int dy = 0; dy < NUM_DOFS_1D; ++dy) {
        for (int dx = 0; dx < NUM_DOFS_1D; ++dx) {
          sol_xy[dy][dx] = 0;
        }
      }
      for (int qy = 0; qy < NUM_QUAD_1D; ++qy) {
        double sol_x[D1];
        for (int dx = 0; dx < NUM_DOFS_1D; ++dx) {
          sol_x[dx] = 0;
        }
        for (int qx = 0; qx < NUM_QUAD_1D; ++qx) {
          const double s = sol_xyz[qz][qy][qx];
          for (int dx = 0; dx < NUM_DOFS_1D; ++dx) {
            sol_x[dx] += quadToDof[ijN(dx,qx,NUM_DOFS_1D)] * s;
          }
        }
        for (int dy = 0; dy < NUM_DOFS_1D; ++dy) {
          const double wy = quadToDof[ijN(dy,qy,NUM_DOFS_1D)];
          for (int dx = 0; dx < NUM_DOFS_1D; ++dx) {
            sol_xy[dy][dx] += wy * sol_x[dx];
          }
        }
      }
      for (int dz = 0; dz < NUM_DOFS_1D; ++dz) {
        const double wz = quadToDof[ijN(dz,qz,NUM_DOFS_1D)];
        for (int dy = 0; dy < NUM_DOFS_1D; ++dy) {
          for (int dx = 0; dx < NUM_DOFS_1D; ++dx) {
            solOut[ijklN(dx,dy,dz,e,NUM_DOFS_1D)] += wz * sol_xy[dy][dx];
          }
        }
      }
    }
  });
}

// *****************************************************************************
void rMassMultAdd(const int dim,
                  const int NUM_DOFS_1D,
                  const int NUM_QUAD_1D,
                  const int numElements,
                  const double* dofToQuad,
                  const double* dofToQuadD,
                  const double* quadToDof,
                  const double* quadToDofD,
                  const double* op,
                  const double* x,
                  double* __restrict y) {
  if (dim==1) assert(false);
  if (dim==2)
    rMassMultAdd2D(NUM_DOFS_1D,
                   NUM_QUAD_1D,
                   numElements,
                   dofToQuad,
                   dofToQuadD,
                   quadToDof,
                   quadToDofD,
                   op,x,y);

  if (dim==3)
    rMassMultAdd3D(NUM_DOFS_1D,
                   NUM_QUAD_1D,
                   numElements,
                   dofToQuad,
                   dofToQuadD,
                   quadToDof,
                   quadToDofD,
                   op,x,y);
}
