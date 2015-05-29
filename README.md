

<div align="center">
	<a href="https://vimeo.com/timmwagener/twclothsolver" target="_blank"><img src="http://www.kiiia.com/tw_cloth_solver/repo_images/tw_cloth_solver_3.jpg"></a>
</div>
-----------------------
<div align="left">
	<a href="http://www.kiiia.com/tw_cloth_solver/repo_images/tw_cloth_solver_1.jpg" target="_blank"><img width="80" height="80" src="http://www.kiiia.com/tw_cloth_solver/repo_images/tw_cloth_solver_1_thumb.jpg"></a>
	<a href="http://www.kiiia.com/tw_cloth_solver/repo_images/tw_cloth_solver_2.jpg" target="_blank"><img width="80" height="80" src="http://www.kiiia.com/tw_cloth_solver/repo_images/tw_cloth_solver_2_thumb.jpg"></a>
	<a href="http://www.kiiia.com/tw_cloth_solver/repo_images/tw_cloth_solver_3.jpg" target="_blank"><img width="80" height="80" src="http://www.kiiia.com/tw_cloth_solver/repo_images/tw_cloth_solver_3_thumb.jpg"></a>
	<a href="http://www.kiiia.com/tw_cloth_solver/repo_images/tw_cloth_solver_4.jpg" target="_blank"><img width="80" height="80" src="http://www.kiiia.com/tw_cloth_solver/repo_images/tw_cloth_solver_4_thumb.jpg"></a>
	<a href="http://www.kiiia.com/tw_cloth_solver/repo_images/tw_cloth_solver_5.jpg" target="_blank"><img width="80" height="80" src="http://www.kiiia.com/tw_cloth_solver/repo_images/tw_cloth_solver_5_thumb.jpg"></a>
	<a href="http://www.kiiia.com/tw_cloth_solver/repo_images/tw_cloth_solver_6.jpg" target="_blank"><img width="80" height="80" src="http://www.kiiia.com/tw_cloth_solver/repo_images/tw_cloth_solver_6_thumb.jpg"></a>
</div>
-----------------------

[![Gitter chat](https://badges.gitter.im/gitterHQ/gitter.png)](https://gitter.im/timmwagener/tw_cloth_solver)

-----------------------


What is it ?
-----------------------
**twClothSolver** is a **Verlet Cloth Solver** implemented as a Maya Plugin. I created this as a university test project to explore the possibilities of GPU multithreading
within the context of Dependency Graph based VFX centered DCCs. It is by no means a stress-tested production ready solution.


Its features include:
* The deformer source code as a Python plugin blockout version
* The C++ API version
* CUDA and OpenMP multithreading, which can be switched during execution
* OOP codebase with solver functionality completely detached from Maya interface, for easy porting to other apps
* Position and convex collision constraints
* OpenGL viewport feedback of solver attributes
* Easy quality/speed balancing thanks to naive repetition based constraint calculation
* Convenience API commands to create and connect constraints


-----------------------

[**Vimeo**](https://vimeo.com/timmwagener/twclothsolver) [**More info**](http://www.timmwagener.com/gpu_cloth_solver.html) [**Author**](http://www.timmwagener.com/)

-----------------------