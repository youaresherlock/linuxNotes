>内容简介:
&emsp; 总结nohup command &以及forever的知识点，让程序后台运行.
>建议阅读时间: 10 minutes
___
**背景:**
&emsp;我们经常会碰到类似问题，用telnet/ssh登录了远程的linux服务器，运行了一些耗时较长的任务(训练神经网络通常需要好几个小时，写个node.js服务端想要一直在后台运行)，结果却由于网络的不稳定导致任务中图失败。如何让命令提交后不受本地关闭中断窗口/网络断开连接的干扰?下面提供一些方法

##### 1. nohup command &
###### 1. 简介
&emsp;**参考文档:** [nohup](https://www.computerhope.com/unix/unohup.htm)
>`&`:
>&emsp;想让某个程序在后台运行可以`command &`,但是一般情况下如果终端关闭，那么程序也将被关闭
>`nohup`:
>&emsp;nohup(no hang up)不挂起的意思。如果你正在运行一个进程，并且你退出账户/终端还不会结束，那么可以使用nohup命令,我们希望后台能够一直运行，我们可以在终端做其他的事情，那么使用`nohup command &`

###### 2. 注意事项
注: 
>&emsp;If standard input is a terminal, nohup redirects it from `/dev/null`. Therefore, terminal input is not possible when running a command with nohup.(会重新将标准输出定向为`/dev/null`.使用nohuo命令终端不会输出日志)
&emsp;If standard output is a terminal, command output is appended to the file nohup.out if possible, or `$HOME/nohup.out` otherwise.(标准输出为终端时，命令输出添加到一个叫做`$HOME/nohup.out`的文件)
&emsp;If standard error is a terminal, it is redirected to standard output.
&emsp;To save output to a file named file, use "`nohup command > file`".(可以使用`nohup command > file`输出重定向)

###### 3. 案例
可以看到我们将wsServer.js后台持续运行,并且输出的日志都在nohup.out中，当然也可以使用`nohup command > file`更改.
```
ubuntu@machine-07:~/nodeJsPractice/websocketPractice/demo7$ nohup node wsServer.js &
[1] 12387
ubuntu@machine-07:~/nodeJsPractice/websocketPractice/demo7$ nohup: ignoring input and appending output to 'nohup.out'
^C
ubuntu@machine-07:~/nodeJsPractice/websocketPractice/demo7$ ps aux | grep 'node'
ubuntu   12387  1.6  2.0 565176 39104 pts/0    Sl   14:43   0:00 node wsServer.js
ubuntu   12413  0.0  0.0  13232   936 pts/0    S+   14:44   0:00 grep --color=auto node
ubuntu@machine-07:~/nodeJsPractice/websocketPractice/demo7$ cat nohup.out
websocket server listening on port: xxxx, on hostname: xxx.xxx.xxx.xxx
```

**杀死任务:**
```
kill -s 9 <id>
# 杀死如上任务[1] 12387 job序号是1,pid是12387
kill -s 9 12387
```

注: 使用nohup之后，如果账户非正常退出或者结束，程序可能结束。因此使用exit/logout正常退出当前账户，保证命令一直在后台运行.
___
##### 2. setsid
&emsp;**run a program in a new session**
###### 1. 案例
`eg:`
```
machiubuntu@machine-07: setsid ping www.baidu.com
machiubuntu@machine-07: PING www.a.shifen.com (220.181.112.244) 56(84) bytes of data.
64 bytes from 220.181.112.244: icmp_seq=1 ttl=54 time=3.22 ms
64 bytes from 220.181.112.244: icmp_seq=2 ttl=54 time=3.22 ms
64 bytes from 220.181.112.244: icmp_seq=3 ttl=54 time=3.22 ms
64 bytes from 220.181.112.244: icmp_seq=4 ttl=54 time=3.15 ms
64 bytes from 220.181.112.244: icmp_seq=5 ttl=54 time=3.19 ms
64 bytes from 220.181.112.244: icmp_seq=6 ttl=54 time=3.22 ms
^C
machiubuntu@machine-07: ps -ef|grep ping
root      8003     1  0 10:05 ?        00:00:00 ping www.baidu.com
root      8020  8004  0 10:06 pts/2    00:00:00 grep ping
```
注: 有兴趣的可以去了解screen
___
###### 3. forever-A simple CLI tool for ensuring that a given script runs continuously (i.e. forever).
###### 1. 简介
&emsp;**参考文档:** [forever](https://www.npmjs.com/package/forever) [github_forever](https://github.com/nodejitsu/forever)
>&emsp;forever是一个简单的命令式nodejs的守护进程，能够启动，停止，重启App应用.forever完全基于命令行操作，在forever进程之下，创建node的子进程，通过monitor监控node子进程的运行情况,一旦文件更新，或者进程挂掉，forever会自动重启node服务器，确保应用正常运行

###### 2. 安装:
```
[sudo] npm install forever -g
note:  If you are using forever programmatically you should install forever-monitor.
注: 如果以编程的方式应用forever那么需要安装forever-monitor.
$ cd /path/to/your/project
$ [sudo] npm install forever-monitor
```

###### 3. 使用:
&emsp;可以通过两种方式来使用forever: 通过命令行或者在代码中使用forever.如果要在代码中使用需要安装forever-monitor
命令行用法: 
**1.查看options**
&emsp;`forever --help`查看可选参数
**2. 简单的启动**
&emsp;`forever app.js #前台进程`
&emsp;`forever start app.js #后台进程, start参数就是设置脚本为守护进程` 
**3. 指定日志输出文件(默认~/.forever/forever.log)**
&emsp;`forever start -l LOGFILE app.js`
**4. 指定输出文件与错误文件**
&emsp;`forever start -o out.log -e err.log app.js`
**5. 追加日志,默认不覆盖**
&emsp;可以在forever/developmet.json中设置配置文件修改
&emsp;`forever start -l LOGFILE -a app.js`
**6. 监听当前文件夹下的所有文件改动**
&emsp;`forever start -w app.js`
**7. 显示所有运行的服务**
`forever list`
**8. 停止、重启任务**
`forever stopall`
`forever stop app.js`
`forever stop [id] #id是forever list对应的id`
`forever restartall #启动所有`