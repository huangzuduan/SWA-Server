更新日志：
	2017/7/9  : 将数据库操作改为protobuf形式，非常方便数据的操作,优化了dependence下的目录结构，用老版本对比需要注意下
	2016/5/16 : 上传cmake初始版本

版权所有代码作者qq296464231所有，代码可用于复制，修改，商业目的等，但不得去掉版权所有标识，或在文件头加于注明即可 

下面两个库也是需要x64
boost 1.60-x64 http://pan.baidu.com/s/1pL6DheN
mysql 5.56-x64 http://pan.baidu.com/s/1nu5KZsH
protobuf 3.3.0 https://pan.baidu.com/s/1mh7AJ7y

也可以下载已经生成好的整包：http://pan.baidu.com/s/1dFtmebf，下载解压后可用

安装步聚：
从Github下载后，将项目放在E:/SWA_SERVER/目录下
从百度云盘下载我编译好的Boost库、Mysql库、Protobuf库，这里需要注意的是，本项目是x64的，所以请在win7-64以上机器测试，mysql要安装5.5-x64以上
导入Server/sql/swa_data.sql 文件 


目录结构：
E:/SWA_SERVER/BIN/
E:/SWA_SERVER/BIN/DEBUG
E:/SWA_SERVER/Server
E:/SWA_SERVER/Server/dependence/boost  		下载boost解压覆盖这个目录
E:/SWA_SERVER/Server/dependence/mysql  		下载mysql解压覆盖这个目录
E:/SWA_SERVER/Server/dependence/protobuf  	下载protobuf解压覆盖这个目录
E:/SWA_SERVER/Server/doc			   		项目的一些临时设置思想文档
E:/SWA_SERVER/Server/sql			   		导入到数据库
E:/SWA_SERVER/Server/Server		   	   		进程服务器代码


生成项目文件.sln
1、你可以通过项目结构创建创建项目
2、也可以通过Cmake来构建项目（Cmake中是用1.60 -x64 vs2015 -x64版本设定）
3、SWA相关的帮助请浏览器官网http://www.hmx-server.org(暂无开通)

生成好后，在Bin/Debug/目录下启动，双击Server.exe启动进程，这时要检查各个进程之前的报错，如打印中有[ERROR]:xxxx
再启动测试Client.exe 
在上面测试的过程中会偶尔弹出assert提示，进程会挂掉，测试阶段这样方便找到问题或者知道哪里可能需要优化，
你可以将ASSERT这个宏定义值 assert 改为 printf("assert\n");就不会影响你开发与测试了，后面我继续完善，这些ASSERT会慢慢去掉

cmake 设置为x64 vs
cmake 3.4 以上

vs编译也需要设置为x64
vs2015 
