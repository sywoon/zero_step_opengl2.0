# zero_step_opengl2.0
本来打算1.0搞熟练后再开始2.0部分，由于熟悉cocos2.2.6源码过程中，
在DrawNode部分卡主了，为了找清楚原因，提前研究2.0部分。


## Framework_SDL
基于sdl封装的一个opengl学习框架，方便做些简单的测试。
[学习记录](http://note.youdao.com/noteshare?id=68064699f373106ebbcd0661add2493e)
注意：
若用clang编译需要加上：-Xlinker /subsystem:console   
若用了-Xlinker /subsystem:windows 也能成功 但启动少了控制台
且需要9.0以上版本

若用cl编译需要加上：/SUBSYSTEM:CONSOLE
若用了/SUBSYSTEM:windows   同上
