# vim笔记
* 尽量不用插件
* ~/.vimrc
```
syntax on
set ts=4
set expandtab
set cursorline   "显示行线
set autoindent              " 设置自动缩进：即每行的缩进值与上一行相等；使用 noautoindent 取消设置
set cindent                 " 以C/C++的模式缩进
set grepprg=grep\ -nr\ --exclude-dir=\.svn\ --exclude=*\.o\ --exclude=tags
set incsearch    "智能查询
set showcmd       " 在状态栏显示目前所执行的指令，未完成的指令片段亦会显示出来
```

# 跳转
* t<char> 跳到本行下一个<char>字符前面
* f<char> 跳到本行下一个<char>字符
* H M L 跳到屏幕的顶上 中间 下方 

* w 跳到下一个单词的开始
* e 跳到单词的结束
* b 向后跳

* gg 跳到文件的开始 
* G 跳到文件的结束 
* 10gg 或10G 跳到第10行 
* CTRL-F next-page
* CTRL-B last-page
* CTRL-D next-half-page
* CTRL-U last-half-page
* ^ 行首 
* $ 行尾
* 0 行第一个字符
* gd 选中所有当前单纯 用n和N跳转
* CTRL+O 跳转到上一次跳转的地方 适用Ex/vimgrep/ctags
* CTRL+I 跳转到下一次跳转的地方 适用Ex/vimgrep/ctags
* :Ex/find 查看文件目录 

# 选中
* gd 选中所有当前单词 用n和N跳转(* and #)
* vi" 选中""之间的所有内容，同理{},[],'',<>都可
* v 选择模式
* V 行选择模式
* CTRL+v 列选择模式

# 删除，替换和复制
* dt<char> 一直删除到这行的<char>
* yw 复制当前光标所在位置到单词尾字符的内容到vi缓存区，相当于复制一个单词
* y$ 复制光标所在位置到行尾内容到缓存区
* y^ 复制光标所在位置到行首内容到缓存区
* byw 复制当前光标所在位置到单词尾字符的内容到vi缓存区，相当于复制一个单词
* s/xx/yy/gc 选中的内容中所有的xx字符变为yy，g为全匹配，c为每个都要用户确定
* 复制复制字符到vim的命令行窗口 先把光标定位到关键词 然后切换到:命令行按<CTRL-R><CTRL-W>

# 骚操作
* :ab name somni 以后再编辑时键入name，然后空格或回车name串就会被somni代替

# vimgrep
* vimgrep /pattern/ */****/*.cpp 搜索字目录里所有的cpp文件里查找pattern
* copen/cw 打开搜索结果
* cclose/ccl 关闭搜索结果
* cn                              查找下一个
* cp                              查找上一个

# spilt/vsp
* :sp filename 上下分出一个tab
* :vsp filename 左右分出一个tab
* CTRL+W+左/下/上/右 移动窗口

# ctags插件
* ctags -R ./ 在当前目录生成索引
* CTRL+T 跳转到上一次跳转的地方 适用ctags
* CTRL+] 取出当前光标下的word作为tag的名字并进行跳转
* Ctrl+W+] 分割当前窗口，并且跳转到光标下的tag
* g] 取出当前光标下的word作为tag的名字并输出结果选择
