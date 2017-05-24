##需要设计一种方案处理输入问题，现在的方案不够灵活。
！！相机控制器计算优化，微调
！！！曲面细分着色器计算过程优化，补全光照
！提升画质，其他可优化效率的地方


使用QSharedPointer和模板泛型函数修改了uniform更新系统。
数据的创建和删除都通过uniformManager.注册uniform的类不再拥有uniform值的所有权，内存全交由uniform manager管理。

