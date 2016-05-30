项目由来：
	实习公司要求客户端程序自学完成的软件渲染器。

使用技术：
	c++, Win32 API

功能 :
	实现各种坐标变换算法；
	实现画线算法,2DDA, bresenham,画线反锯齿；
	实现一个fps游戏的相机；
	实现Gouraud光照算法,支持环境光和方向光；
	实现Gouraud三角形着色散发，并对三角形进行填充，同时实现自己的深度缓冲功能，支持由三角形构成的物体的显示；
	实现TextureMapping的Gouraud的三角形着色算法，及纹理映射的Perspective Correction；
	实现屏幕裁剪和三维近面裁剪；
	实现SSAA。

SSAA功能开启：
	main.cpp中反注释#define SSAA