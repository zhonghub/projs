# 期末项目 -- iOS应用程序

## 项目内容

* 运用综合知识编写功能比较完善的iOS项目
* **2022.11.30前提交所有内容**

---

## 分组名单

每小组最多3人，允许单独成组。

[分组名单](https://docs.qq.com/sheet/DQmNldE9PVkhzQVp4?tab=BB08J2)

---

## 提交内容

### 小组提交（每组一份）

* 流程管理设计
* 调研分析
* 交互设计（用户使用手册）
* 技术选型文档
* 架构设计文档
* 展示PPT（及demo视频）
* 代码仓库（成员贡献表和Git提交记录）
* 模块划分
* 重点难点记录
* 如涉及后端需提供后端代码及部署文档

参考本仓库documents目录下的例子文档。

### 个人提交（每人一份）

个人实验总结，主要体现个人贡献。

* 个人完成的功能截图与代码，最好不要出现两个人贴了同一段代码的情况。
* 遇到的问题与解决方案
* 个人总结与个人贡献评分
* 思想感悟
* 文档书写整理，需求分析设计，程序的测试工具与方法等均可归入个人贡献中。

---

## 评分标准

* 主题设计与创新性 【20%】
* 功能实现与所用技术 【40%】
* 展示与文档 【30%】
* 个人成绩根据小组成绩以及贡献度百分比给分【10%】

---

## 主题设计要求

1. 基于iOS设计与开发一款应用
2. 可选方向一. 联合国17个可持续发展目标
   * 17个可持续发展目标是实现所有人更美好和更可持续未来的蓝图。目标提出了我们面临的全球挑战，包括与贫困、不平等、气候、环境退化、繁荣以及和平与正义有关的挑战。 这些目标相互关联，旨在不让任何一个人掉队，我们必须在2030年之前实现每个目标。
   * [可持续发展目标(un.org)](https://www.un.org/sustainabledevelopment/zh/sustainable-development-goals/)
3. 可选方向二. 移动应用创新赛
   * [移动应用创新赛燃起无数中国学生开发者的编程热情 - Apple (中国大陆)](https://www.apple.com.cn/newsroom/2021/05/mobile-app-innovation-contest-sparks-passion-for-coding-among-thousands-of-chinese-students/)
4. 其他自由选题. 例如社交应用、工具应用等。
5. 要求符合创新性，实用性，美观等感性要求; 设计满足一定设计规范，软件开发符合软件开发周期。

---

## 技术要求

根据项目需要自行选择，生硬堆砌技术不加分。

* 基础UI控件布局与交互
* 界面动画实现
* 异步网络请求
* 本地持久化
* 多线程编程
* 前端与客户端交互：JSBridge
* 设计模式：工厂模式、单例模式等
* 多媒体技术
* 3D/VR/AR技术
* ····

---

### 期末项目Q&A

---

如应用运行依赖后端服务，要求如下：

1. 自建后台要求
   * 语言没硬性规定，python，go，java都行。
   * 需要提供后台源码以及部署说明，如果有docker-compose支持一键部署更好。
   * 切忌后台开发喧宾夺主

2. 网络服务要求
   * 无论是自建后台的还是使用第三方API的。需要保证服务可用能持续到学期结束，方便测试评分。

---

## 小组项目提交方式

1. 组长将期末项目fork到自己个人仓库下
2. 在小组项目中，组长分别在code、report目录下新建目录，目录名为“Group+应用名”；
3. 组员fork小组项目到个人仓库下，然后clone到本地目录，在“code\Group+应用名”目录下，新建iOS项目，按分工编写程序，注意.gitignore的编写，避免添加不必要的中间文件、临时文件到git中；
4. 实验报告文档按给出的参考模版，以md的格式，写在“report\Group+应用名\”目录下，截图也放在该目录下；
5. 项目完成时，组员Pull Request回小组项目的master分支，组长合并调整后，再Pull Request回主项目的master分支；
6. 组长Pull Request时，请确保“code\Group+应用名”目录下的iOS项目是可执行的，“report\Group+应用名”含所有项目文档及视频
7. 建议组员Pull Request时，不要Pull Request配置文件，避免配置文件随意被覆盖，导致最终项目无法运行；
