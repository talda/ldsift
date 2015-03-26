descriptor matlab toolbox - a toolbox containing my work on local features for meshes: MeshScaleDoG local features detector, the Scale Invariant Spin Image descriptor and the Local Depth SIFT descriptor.

if you use this code please cite: Tal Darom and Yosi Keller, “Scale Invariant Features for 3D Mesh Models”. IEEE Transactions on Image Processing 21(5): 2758-2769 (2012).

The examples include models taken from: 
Doug M. Boyer, Yaron Lipman, Elizabeth St. Clair, Jesus Puente, Biren A. Patel, Thomas Funkhouser, Jukka Jernvall, and Ingrid Daubechies. 'Algorithms to Automatically Quantify the Geometric Similarity of Anatomical Surfaces'. PNAS, vol. 108 no. 45, November 8, 2011

before using the toolbox please run compile_mex_files.m to compile all mex files.

main functions:

dog( vertex, faces) - runs the MeshScaleDoG detector on the mesh.

create_mesh_sift_features(vertex, faces) - runs the MeshScaleDoG detector on the mesh and extract the LD-SIFT descriptors for these points. It needs the VLFeat toolbox installed.

create_mesh_spin_features( vertex, faces) - runs the MeshScaleDoG detector on the mesh and extract the SISI descriptors for these points.

find_candidates_example - loads 2 mesh models, calculates descriptors for both and for each feature point of the first model finds matching candidates on the second based on descriptor distance.

This toolbox contains some code taken from "Toolbox Graph" by Gabriel Peyre and from "Patch Software Render" by Dirk-Jan Kroon.
