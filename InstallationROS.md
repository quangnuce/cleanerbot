# Cài đặt ROS
1. Cài đặt máy ảo Ubuntu
- Download và cài đặt Virtual Box : https://www.virtualbox.org/wiki/Downloads
- Cài đặt Virtual Box Extension Pack : https://download.virtualbox.org/virtualbox/5.2.22/Oracle_VM_VirtualBox_Extension_Pack-5.2.22.vbox-extpack
- Download Ubuntu 16.04.5 LTS Xenial :http://releases.ubuntu.com/16.04/ubuntu-16.04.5-desktop-amd64.iso
- Tạo máy ảo Virtual Box với bản Ubuntu 16.04:
	+ Chọn New
	+ Thiết lập các thông số: RAM ít nhất 2G, Hard Disk: 15GB
	+ Chọn máy ảo vừa tạo, nhấn Setting, ở tab Storage chọn ổ đĩa trong Controller:IDE, ở phần Attributes nhấn vào biểu tượng đĩa để mở "Choose Virtual Optical Disk file", tìm đến file iso của Ubuntu 16.0.4
	+ Start máy ảo để cài đặt Ubuntu, chú ý ghi nhớ mật khẩu cho các tài khoản root và user
2. Cài đặt ROS kinetic (xem hướng dẫn cài đặt ở http://wiki.ros.org/kinetic/Installation/Ubuntu )
Mở Terminal trên Ubuntu và gõ các lệnh theo hướng dẫn:
- Set up source.list:
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
- Setup key:
sudo apt-key adv --keyserver hkp://ha.pool.sks-keyservers.net:80 --recv-key 421C365BD9FF1F717815A3895523BAEEB01FA116
-Installation:
sudo apt-get update
sudo apt-get install ros-kinetic-desktop-full
- Install rosdep:
sudo rosdep init
rosdep update
- Environment setup:
echo "source /opt/ros/kinetic/setup.bash" >> ~/.bashrc
source ~/.bashrc
- Dependencies for building packages
sudo apt-get install python-rosinstall python-rosinstall-generator python-wstool build-essential
3. Thiết lập môi trường phát triển;
- Kiểm tra các biến môi trường xem đã được thiết lập chưa:
printenv | grep ROS
- Setup
source /opt/ros/kinetic/setup.bash
- Tạo workspace làm việc:
$ mkdir -p ~/catkin_ws/src
$ cd ~/catkin_ws/
$ catkin_make
- Source file setup
source devel/setup.bash
- Kiểm tra lại:
 echo $ROS_PACKAGE_PATH
