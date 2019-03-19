# OS-exp
experiments for Course 'Advanced Operation System'.

In order to compile these modules, you should first get kernel code with version greater than 3.10:
```
wget http://mirrors.aliyun.com/linux-kernel/v4.x/linux-4.2.6.tar.xz   
xz -d linux-4.2.6.tar.xz
tar -xvf linux-4.2.6.tar
```
Next, download the code of this proj to local machine. Here are the steps to compile **proc_hello**:
```
cp ~/proc_hello/proc_hello.c ~/linux-4.2.6/fs/proc    # copy proc_hello.c to destination
mv Makefile Makefile.bak # since proc has its own Makefile, we need to backup it first
cp ~/proc_hello/Makefile ~/linux-4.2.6/fs/proc  # copy Makefile to proc
```
```
make   # compile
```
![make](https://github.com/Larry955/OS-exp/blob/master/imgs/make.png)
```
sudo insmod proc_hello.ko   # install this module if you have compiled it successfully
```
![insmod](https://github.com/Larry955/OS-exp/blob/master/imgs/insmod.png)

Following these steps above, you shall see a "**hello**" directory in **/proc**, and a file called "**world**" under "hello". Cat world and you will print "**hello world**" if everything goes well.

![proc_hello](https://github.com/Larry955/OS-exp/blob/master/imgs/proc_hello.png)
