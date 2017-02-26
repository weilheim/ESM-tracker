# ESM-tracker
C++ code for template-based tracking method, shortened as ESM, proposed in **Homography-based 2D Visual Tracking and Servoing**. ESM method elimites the need to compute the Hessian matrix of the input images. Thus it significantly improves the speed to 75 frames per second (8G RAM, Intel I7).

## Prerequisites
1. Classes and methods from OpenCV 2.4.12 are used in the code. OpenCV 2.4.12 can be downloaded from http://opencv.org/. The .props file included in the folder must be imported before you run the code.
2. Debug and Win64 should be choosed. If you want to run in Release mode, you need to create and include a new .props file. 

## Reference
1. Gregory Hager and Peter Belhumeur. Efficient Region Tracking With Parametric Models of Geometry and Illumination. _IEEE Trans. on Pattern Analysis and Machine Intelligence_, 1998.
2. Selim Benhimane and Ezio Malis. Real-time image-based tracking of planesusing Efficient Second-order Minimization. In Proc _IEEE Conference on Intelligent Robotics and Systems_, 2004.
3. Selim Benhimane and Ezio Malis. Homography-based 2D Visual Tracking and Servoing. _International Journal of Robotics Research_, 2007.
4. Rogerio Richa, Raphael Sznitman and Gregory Hager. Robust Similarity Measures for Gradient-based Direct Visual Tracking. _CIRL Technical Report_, 2012

### P.S.
The code was written a while ago and I was not good at programming at that time. You might find the code confusing due to lack of comments.
