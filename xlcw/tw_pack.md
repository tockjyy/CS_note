# TW打包流程

* 准备工作：三个机器 
* A:JenkinsMachine 
* B:CompileMachine 编译机器
* C:RunMachine 运行机器

- jenkins有默认插件是支持资源锁的，可以避免编译时全量，或者全量时编译->Lockable Resources

* Jenkins伪代码(不知道是什么语法，类python，最终会用shell):
``` py
# 远程连接C机器，获取运行目录下所有应该同步文件的md5列表
run_md5s = ssh(C).get_file_md5s()

# 远程连接B机器，获取当前运行目录下所有应该同步文件的md5列表
compile_md5s = ssh(B).get_file_md5s()

# 对比两个机器上的文件，在compare_result结构下输出以下类型的结果
# file1:UPDATE
# file2:DEL
# file3:DEL
compare_result = compare_mds(run_md5s, compile_md5s)

# 远程连接B机器，将run目录下的compare_result标识为UPDATE的文件打包，要支持每一次打包在B机器上不同的工作目录，以方便支持多机器同时更新
tar_file_name = ssh(B).gen_tar(compare_result)

# 

```