#一个假装是设计文档的东西
----
##目标
首先把需求文档中的必做项目罗列在此：  
###1 人物  
####1.1 控制人物
玩家可以控制人物，实现下列情况的任一种即可：  
● 通过键盘上的方向键控制人物；  
● 通过鼠标拖动控制人物。  
####1.2 自由下落
人物在下列情况时，有下落效果：  
● 跳跃后能够下落；  
● 遇到低的平面时下落。  
####1.3 人物死亡
人物在下列两种情况下死亡：  
● 人物掉落到陷阱里；  
● 人物跑的太慢，被后面场景卡死；  
● 其它更有创意的设计，如：被击中等。  
###2 场景
####2.1 场景组成
场景组成需要满足以下条件：   
● 场景必须有高有低，且人物有可能跳跃通过；   
● 场景每隔一段必须有一些陷阱，且人物有可能跳跃通过；  
● 场景能够自动向后移动。  
####2.2 场景变化
场景随着人物奔跑会发生变化。完成下列情况的任一种即可：   
● 按照固定规律重复变化；  
● 随机变化，要求人物必须有可能通过场景；  
● 其它更有创意的设计。  
###3 控制界面
####3.1 显示分数
显示玩家奔跑距离或得分。
####3.2 显示暂停
游戏可以实现暂停。
####3.3 开始和结束界面  
必须有开始界面和结束界面：  
● 显示游戏开始加载界面；  
● 游戏结束显示玩家奔跑距离与得分；  
● 设计一些有创意的动画。  
###4 可选方案选择：
####4.1 连续跳跃
人物能够进行连续跳跃。完成下列情况的任一种即可：  
● 在跳跃时，按方向键向上键连续跳跃。  
####4.2 动态效果1 
人物可以有多种动态效果：  
● 人物奔跑效果。
####4.3 动态效果2
人物可以有多种动态效果：  
● 人物死亡后效果。
####4.4 扩展1
扩展功能：  
●Boss战设定。
####4.5 扩展2
扩展功能：  
●人物可以积蓄能量放大招。
####4.6 扩展3
扩展功能：  
●人物奔跑超过一定距离场景出现特效。
####4.7 加分道具
场景中可以随机产生一些加分道具：  
● 金币、五角星等。
####4.8 音效1
在游戏过程中，有音乐效果。完成下列情况的任一种即可：  
● 背景音效、跳跃、死亡或吃到奖励音效，实现两项即可。  
####4.9 音效2
在游戏过程中，有音乐效果。完成下列情况的任一种即可：  
●其它更有创意的设计：放大招时的咆哮。
####4.10 奖励
玩家在游戏过程中，出现一些特殊物品，人物碰到后会出现特殊效果。完成下列情况的任一种即可：  
● 人物暂时无敌，持续时间 N 秒。   


##基本设定：
名字：奔跑吧！Doge！/Name:Running Doge!  
###整体流程
（开始界面）   
OS:我是doge，那一天，我照常关灯睡觉。但当我睁开眼时，眼前的一切，包括我，都失去了色彩……（载入画面但不奔跑，几句话之后开始。）  （把开始设在这里）  
游戏开始  +若干句扯淡  
完成若干次爆发后，开启boss战。  
——是谁在吵闹？  
——这是哪里？为何没有颜色？  
——聒噪！  
（boss战开始）  
（胜利）  
结束画面：  
OS:然后我就醒了,眼前的世界重新恢复了正常，刚刚发生的一切，真的只是一场梦么？那个巨人又是谁？如果我走不出来，我是否就真的出不来了？我不知道，或许我永远也不会知道了。我只知道，我现在很好。  
（图片刷屏，大狗在中央）  
--The End  
（ps:如果死亡，出现画面：黑暗，又是黑暗，这该死的黑暗要持续到什么时候！XXXXX (分数），为什么我突然想到这个数字，什么意思？头好痛……我刚刚在说什么啊……（按键继续）然后回到一开始）  

##剩下的锅
更换素材  
开始界面  
开始动画效果  
结束动画效果  
过场动画  
死亡动画  
场景切换   
音效（主体、金币、咆哮、无敌）  
整体背景  
障碍设定（尖刺）  
下滑操作及动画  
跳跃下落的物理引擎及动画  
卡死判定、掉落判定、尖刺判定  
卡住的追及设定  
随机地图、随机障碍  
速度设定及逐步增大设定  
得分设定  
二段跳    
boss战击打设定、boss导弹攻击之类  
大招计时与计数、冲刺动画、屏幕显示  
金币随机出现设定  
无敌道具设定   
（我真的要写不完了啊啊啊啊啊啊啊啊啊啊啊）
