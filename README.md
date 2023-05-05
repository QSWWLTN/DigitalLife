# DigitalLife

# 下载前请先看完此教程先

## 使用版本
- UE：5.0.3（可以升级到5.1.1，但需要将插件同样升级到5.1.1）
- Protobuf：3.14.0
---

## 对于蓝图玩家的教程
### 	 添加模型和动作方法
1.  首先，确保你导入的模型中拥有变形目标，因为此项目显示人物表情需要使用到变形目标，如果没有的话可以在建模软件中设置，这里就不展开了

2. 	打开项目的.ueproject文件，如果提示需要编译的情况，如图：
![iTAq9d.png](https://i.328888.xyz/2023/05/05/iTAq9d.png)
请下载VisualStudio2022或者其他UE支持的编译器，并下载了WindowsSDK和.Net环境，具体可以自行百度UE编译环境搭建。

3. 下载完成后右键后缀名为.uproject的文件，选择生成VisualStudio项目文件，等待进度条完成
![iTDNJE.png](https://i.328888.xyz/2023/05/05/iTDNJE.png)

4. 点击后缀为.sln的文件，打开VisualStudio,右键解决方案管理器中的名称为T的项目并设置为启动项目
![iTD6Dd.png](https://i.328888.xyz/2023/05/05/iTD6Dd.png)

![iTD5bw.png](https://i.328888.xyz/2023/05/05/iTD5bw.png)

5. 确保箭头中下拉框中选择了Development Editor后点击本地调试器
![iTD9XL.png](https://i.328888.xyz/2023/05/05/iTD9XL.png)
等待编译完成后就会自动开启项目，当编译完成后，之后的开启项目就不需要重复2-4步了

6. 开启项目后就可以导入你制作的模型和动作，位置没有规定，可以放在项目的任何位置

7. 点击TargetActor目录下的BaseTargetActor，将SkeletalMesh或者SkeletalMesh_2替换为你制作的模型；SkeletalMesh对应的服务器选项为PaiMon，SkeletalMesh_2对应YunFei；SkeletalMesh自带随机飘动效果和摄像机晃动，SkeletalMesh_2没有；创建你的动画蓝图到项目任意位置，打开动画蓝图的事件图表，在事件蓝图更新动画后添加
如图的内容
![iTnE4d.png](https://i.328888.xyz/2023/05/05/iTnE4d.png)

8. 添加完成后打开动画图表，在输出姿势前添加插槽动画，默认即可
![iTtbaa.png](https://i.328888.xyz/2023/05/05/iTtbaa.png)

9. 状态机可以根据获取到的NowState来进行判断，这里就靠你自己发挥了

###	添加表情部分
1. 在任意位置创建一个继承于ExpressionStruct的数据表

> #### ExpressionStruct结构

> ID：表情命名，需要遵照MoodType枚举中规定的名称并以如：开心_1 命名，同时行命名必须与ID相同

> ExpressionMap：表情字典，即变形目标名称+对应的想变化的值

> Jump：废弃字段，不需要填写

> ActionMap：人物状态字典，用于规定某个动作触发的概率

> HaveExtraData：是否存在额外的数据，在蒙太奇中加入SwitchExpression通知后将会检查这个字段，如果为真的情况下，将会在原有的表情上额外添加变形目标变化，相当于在播放蒙太奇时加上额外表情

> ExtraData：额外的表情，如果存在多个的情况下，将会随机抽取一个，注意，不要添加与ExpressionMap中一样的部分，比如2个同时调整了お变形目标，否则将会出现很奇怪的情况

> PlayMontage：是否播放蒙太奇的权重

> Montage：需要播放的蒙太奇列表，当出现多个的时候将随机抽取一个

> DontSwitchState：在播放完后是否不要切换状态

2. 添加完表情后打开UI目录下的StartWidget控件，将MorphTargetName改为你模型中嘴巴的变形目标名称，并将ExpressionTable修改为你刚才创建的数据表
![iTCMPw.png](https://i.328888.xyz/2023/05/05/iTCMPw.png)

### 修改触发说话部分
BaseTargetActor的Say输入事件为Windows下的触发事件，SayPressed和SayReleased为安卓的触屏触发事件
![iTOHbN.png](https://i.328888.xyz/2023/05/05/iTOHbN.png)

![iTOX6p.png](https://i.328888.xyz/2023/05/05/iTOX6p.png)

想要自己修改触发方式的话可以在这里修改

### 无聊的触发时间设置
![iTbZxb.png](https://i.328888.xyz/2023/05/05/iTbZxb.png)

### 眨眼的触发时间设置
![iTjGOH.png](https://i.328888.xyz/2023/05/05/iTjGOH.png)

###	注意事项
# 1.MoodType枚举不允许修改！！！

2. StateType枚举可以随意修改，具体的状态可以自己实现，但正常状态和思考必须要保留

3. Movies文件夹请不要删除，Android下需要使用到

---

---
## 对于会使用C++的玩家
### 如果只是想修改表情和动作的情况下，只需要看蓝图部分即可

#### 切换通信协议
将T.Build.cs中的

    PublicDefinitions.Add("NO_PROTOBUF=1");   //不使用Protobuf协议
	
改为

    PublicDefinitions.Add("NO_PROTOBUF=0");  //使用Protobuf协议
	
即可
注意，需要服务器端同时切换为Protobuf才能正常通信

#### 代码中职责划分
1. ClientGameInstance：保存和管理Socket，负责触发蓝图广播和窗口管理，处理音频文件
2. TestFunctionLibrary：JNI事件，文件管理，杂项函数
3. SelectKeyWidget：用户的快捷键设置
4. UserSetWidget：用户界面设置
5. SendMessageAsyncTask：发送客户端音频数据线程
6. ReadMessageAsyncTask：接受服务器数据线程
7. HookKeyboardAsyncTask：勾起键盘事件线程

#### 概述
C++部分其实没什么好说的，大部分的代码都是看一眼就知道怎么写的了，所以也不过多介绍了，根据上面的代码职责一个一个看过去就行了

---

#### 末尾的一点话
蓝图因为没有整理会有点乱，但是代码逻辑部分都是清晰的，就不要吐槽了；项目是带有热更新插件的，如果想发布Mod之类的，可以用这个来打个热更新包自己玩。