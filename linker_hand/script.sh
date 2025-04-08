#!/bin/bash

VERSION="1.0.0"

function LoadedColor_information()
{
  # 文本颜色
  BLACK='\033[0;30m'
  # 黑色
  RED='\033[0;31m'
  # 红色
  GREEN='\033[0;32m'
  # 绿色
  YELLOW='\033[0;33m'
  # 黄色
  BLUE='\033[0;34m'
  # 蓝色
  PURPLE='\033[0;35m'
  # 紫色
  CYAN='\033[0;36m'
  # 青色
  WHITE='\033[0;37m'
  # 白色

  RED_WHITE='\033[47;31m'
  # 红色
  GREEN_WHITE='\033[47;32m'
  # 绿色
  WHITE_BLACK='\033[40;37m'
  # 白色+黑色
  YELLOW_WHITE='\033[47;33m'

  # 黄色+白色
  # 背景颜色
  BLACK_B='\033[40m'
  # 黑色
  RED_B='\033[41m'
  # 红色
  GREEN_B='\033[42m'
  # 绿色
  yellow_B='\033[43m'
  # 黄色
  blue_B='\033[44m'
  # 蓝色
  purple_B='\033[45m'
  # 紫色
  Cyan_B='\033[46m'
  # 青色
  White_B='\033[47m'
  # 白色

  # 文本样式
  RESET='\033[0m'
  # 重置所有属性
  BOLD='\033[1m'
  # 粗体
  underline='\033[4m'
  # 下划线
  Blinking='\033[5m'
  # 闪烁
  Invert='\033[7m'
  # 反显
  Hidden='\033[8m'
  # 隐藏
#   echo "========================================================="
#   echo -e "${GREEN}Successfully loaded color information !${RESET}"
#   echo "========================================================="
}

CLOSE_RESERVE="exce bash" # exce bash

RUN() {
	local launch=$1
	gnome-terminal -- bash -c "source devel/setup.sh; $launch; $CLOSE_RESERVE"
	sleep 3
}


RUN1() {
	local launch=$1
	local command package launch_file

    # 分割launch参数为三个字段
    IFS=' ' read -r command package launch_file <<< "$launch"
	
	gnome-terminal -- bash -c "source devel/setup.sh; $launch; $CLOSE_RESERVE"  > /dev/null 2>&1
	
	echo "Node: $package"
	# 等待节点启动
    while ! rosnode list | grep -q $package; do
      echo -e "  启动中..."
      sleep 1
    done
    echo -e "  启动完成"
}


function build_sdk(){
    mkdir build; cd build; cmake ..; make; cd ..
}

function install_sdk(){
    cd build; sudo make install
}

function uninstall_sdk(){
    sudo rm -rf /usr/local/lib/liblinker_hand_lib.a
    sudo rm -rf /usr/local/include/LinkerHandL10Can.h
    sudo rm -rf /usr/local/include/CanBus.h
    sudo rm -rf /usr/local/include/HandFactory.h
    sudo rm -rf /usr/local/include/LinkerHandL20Can.h
    sudo rm -rf /usr/local/include/LinkerHandApi.h
    sudo rm -rf /usr/local/include/IHand.h
    sudo rm -rf /usr/local/config/setting.yaml
    sudo rm -rf /usr/local/bin/linker_hand_example
}

function run_example(){
    cd build; ./linker_hand_example
}

#------------------------------------------------ 选择菜单 ------------------------------------------

function select_menu(){
  cd $current_dir
  echo -e "${GREEN}请根据菜单栏输入你的选择：${RESET}"
  read -p "" select_num
  case $select_num in
      1)
        echo "编译SDK"
        build_sdk
        ;;
      2)
      	echo "安装SDK"
        build_sdk
        install_sdk
        ;;
      3)
        echo "卸载SDK"
        uninstall_sdk
        ;;
      6)
        echo "执行示例"
        run_example
        ;;
      0)
        echo "退出"
        exit
        ;;
      *)
        echo -e "${RED}输入错误，请重新输入！${RESET}"
        sleep 1
        ;;
  esac
}

#------------------------------------------------ 菜单栏 ------------------------------------------
function show_Info(){
  # 打印顶部边框
  echo -e "${YELLOW}"
  echo "================================================"
  echo -e "${YELLOW} 灵心巧手CPP-SDK  版本：${VERSION} ${RESET}"
  echo -e "${YELLOW}================================================${RESET}"

  # 打印菜单
  echo -e "${GREEN}"
  echo "RUN Choose Task:[请输入括号内的数字]"
  echo -e "${YELLOW}————————————————————————————————————————————————${RESET}"
  echo -e "${BLUE}[1]: 编译SDK;${RESET}"
  echo -e "${YELLOW}————————————————————————————————————————————————${RESET}"
  echo -e "${BLUE}[2]: 安装SDK;${RESET}"
  echo -e "${YELLOW}————————————————————————————————————————————————${RESET}"
  echo -e "${BLUE}[3]: 卸载SDK;${RESET}"
  echo -e "${YELLOW}————————————————————————————————————————————————${RESET}"
  echo -e "${RED}[0]: 退出;${RESET}"
  echo -e "${YELLOW}————————————————————————————————————————————————${RESET}"
  # sudo make DESTDIR=/home/lst/Desktop/install install
}

#------------------------------------------------ 初始化 ------------------------------------------

#初始化
function Init()
{
	
    # 加载颜色信息
    LoadedColor_information
	# 获取当前文件路径
  	current_dir=$(pwd)
    sleep 1
}

#------------------------------------------------ 主程序 ------------------------------------------

Init
while true
do
    show_Info
    select_menu
done
