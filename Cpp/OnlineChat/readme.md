## 多人在线聊天软件

### 一 展示





### 二 项目构建和运行

打开每个项目下的a.bat，在cmd运行qmake -tp vc，使用qmake构建项目，然后双击生成的client.vcxproj打开vs项目。

#### qmake示例：

```makefile
# qmake -tp vc
QT += core gui

QT += widgets

CONFIG += console qt c++11

DEFINES += QT_DEPRECATED_WARNINGS

TARGET = client

TEMPLATE = app

SOURCES += ChatWindow.cpp \
    mainwindow.cpp \
    Client.cpp \
    Client2.cpp \
    myUseFun.cpp \
    main.cpp

HEADERS += mainwindow.h \
    chatWindow.h \
    Client.h \
    Client2.h \
    myUseFun.h \

```



### 三  解决vs2019的mysql的odbc依赖问题

在服务器程序所在的VS项目中，由于需要操纵MySQL数据库，需要设置项目属性。

#### 1. 项目/属性/vc++目录/包含目录（放的.h头文件）

C:\Program Files\MySQL\Connector C++ 8.0\include\jdbc

#### 2. 库目录（放的.lib文件）

C:\Program Files\MySQL\Connector C++ 8.0\lib64\vs14

#### 3. 链接器/输入/附加依赖项

C:\Program Files\MySQL\Connector C++ 8.0\lib64\vs14\mysqlcppconn.lib

#### 4. 将 DLL 复制到项目输出目录：项目属性/生成事件/生成后事件

如果你有多个 DLL 文件，你可以设置一个自定义的“生成事件”，在编译时将这些 DLL 文件复制到项目

输出目录（Debug 或 Release 文件夹）。复制 DLL 文件到输出目录。

copy /Y "Path1\mysqlcppconn-9-vs14.dll" "$(TargetDir)"

copy /Y "Path2\other.dll" "$(TargetDir)"

生成后事件： 

copy /Y "C:\Program Files\MySQL\Connector C++ 8.0\lib64\mysqlcppconn-9-vs14.dll" "$(TargetDir)"



### 四 数据库相关

```sql
DROP TABLE Users;
DROP TABLE Msg;

// 创建两张表，用户表和消息表，设置实体完整性和参照完整性
create table Users(
User_id INT(4) AUTO_INCREMENT,
User_Cde varchar(20), 
User_Pass varchar(20) NOT NULL, 
User_LogTime varchar(20) NOT NULL,
PRIMARY KEY (User_Cde),
UNIQUE KEY (User_id)
)ENGINE=InnoDB;

create table Msg(
Msg_id INT(4) AUTO_INCREMENT,
User_Cde varchar(20) NOT NULL , 
Msg varchar(30) NOT NULL, 
WriteTime varchar(20) NOT NULL,
PRIMARY KEY (Msg_id),
FOREIGN KEY(User_Cde) REFERENCES Users(User_Cde)
)ENGINE=InnoDB;

创建索引
CREATE UNIQUE INDEX msg_id_index ON Msg(Msg_id);

插入
INSERT INTO users VALUES(0 , 'zhc' , '123' ,  '2023-09-15 10:54:23')
insert into Msg(User_Cde,Msg,WriteTime) values('zhc' , '12223' ,  '2023-09-15 10:54:23');

搜索
select User_Cde from Users where User_Cde=s1
select User_Pass from Users where User_Cde=s1
select User_Cde,Msg,WriteTime From Msg order by Msg_id;
```

