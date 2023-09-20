#ifndef sql_H
#define sql_H

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include<iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") // 链接到WS2_32库

namespace my_sql_use {
    /*
    获取当前时间
    */
    std::string getCurrentTimeAsString();
    /*
    获取当前IP
    */
    std::string getLocalIPAddress();
    /*
    将一个字符串以"#"分割成多个子串
    */
    std::vector<std::string> splitString(const std::string& input, char delimiter);

}


class SQL_con {
private:
    static SQL_con* instance;

    std::string server = "tcp://localhost:3306"; // or "tcp://127.0.0.1:3306"
    std::string username = "root";
    std::string password = "123456";
    std::string database = "data1";

    SQL_con() {
        server = "tcp://localhost:3306"; // or "tcp://127.0.0.1:3306"
        username = "root";
        password = "123456";
        database = "data1";
    }

public:
    static SQL_con* getInstance() {
        static int count = 0; // 声明并初始化静态变量，记录实例的个数
        if (instance == nullptr) {
            instance = new SQL_con();
            count++;
        }
        return instance;
    }

    void init(std::string server, std::string username, std::string password, std::string database) {
        this->server = server;
        this->username = username;
        this->password = password;
        this->database = database;
    }

    /*
    执行insert语句
    */
    bool insert_sql(std::string table_name, std::string s1, std::string s2, std::string s3) {
        bool insert_result = true;
        try {
            sql::mysql::MySQL_Driver* driver = NULL;
            sql::Connection* con = NULL;;
            sql::PreparedStatement* pstmt = NULL;;

            // 创建MySQL连接
            driver = sql::mysql::get_mysql_driver_instance();
            con = driver->connect(server, username, password);

            // 连接到特定的数据库
            con->setSchema(database);

            // 准备INSERT语句
            std::string insertQuery = "INSERT INTO ";
            insertQuery.append(table_name);
            insertQuery.append(" VALUES(? , ? , ? , ? )");
            std::cout << std::endl << insertQuery << std::endl;

            if (table_name._Equal("users")) {
                sql::Statement* stmt;
                sql::ResultSet* res;
                // 组合成select语句
                std::string selectQuery = "SELECT User_Pass FROM Users WHERE User_Cde='";
                selectQuery.append(s1);
                selectQuery.append("'");
                // 执行SELECT语句
                stmt = con->createStatement();
                res = stmt->executeQuery(selectQuery);
                // 输出查询结果
                if (res->next()) {
                    std::cout << s1 << " is exist" << std::endl;
                    insert_result = false;
                }
                else {
                    std::cout << s1 << " is not exist" << std::endl;
                }
                delete res;
                delete stmt;
            }
            // 准备insert语句
            pstmt = con->prepareStatement(insertQuery);
            pstmt->setInt(1, 0);
            pstmt->setString(2, s1);
            pstmt->setString(3, s2);
            pstmt->setString(4, s3);

            if (insert_result) {
                // 执行插入操作
                int rowsAffected = pstmt->executeUpdate();
                // 输出插入的行数
                std::cout << "Rows inserted: " << rowsAffected << std::endl;
            }
            // Clean up
            delete pstmt;
            delete con;
        }
        catch (sql::SQLException& e) {
            // Handle any exceptions that may occur during the process
            std::cerr << "SQLException: " << e.what() << std::endl;
            return false;
        }
        return insert_result;
    }

    /*
    执行select语句, 输出Msg表
    */
    std::string select_all_msg() {
        std::string all_msg = "";

        sql::mysql::MySQL_Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        // 创建MySQL连接
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(server, username, password);

        // 连接到特定的数据库
        con->setSchema(database);

        // 组合成select语句
        // SELECT User_Cde, Msg, WriteTime FROM Msg ORDER BY WriteTime
        std::string selectQuery = "SELECT User_Cde, Msg, WriteTime FROM Msg ORDER BY Msg_id";
        std::cout << std::endl << selectQuery << std::endl;
        
        // 执行SELECT语句
        stmt = con->createStatement();
        res = stmt->executeQuery(selectQuery);

        // 输出查询结果
        while (res->next()) {
            //std::cout << "User_Cde: " << res->getString("User_Cde") << ", ";
            //std::cout << "Msg: " << res->getString("Msg") << ", ";
            //std::cout << "WriteTime: " << res->getString("WriteTime") << std::endl;
            all_msg.append(res->getString("WriteTime") + "  " + res->getString("User_Cde") + " :" + res->getString("Msg") + "\n");
        }
        // 释放资源
        delete res;
        delete stmt;
        delete con;

        return all_msg;
    }

    /*
    执行select语句，输出用户的password
    */
    void select_sql2(std::string User_Cde) {
        sql::mysql::MySQL_Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        // 创建MySQL连接
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(server, username, password);

        // 连接到特定的数据库
        con->setSchema(database);

        // 组合成select语句
        std::string selectQuery = "SELECT User_Pass FROM Users WHERE User_Cde='";
        selectQuery.append(User_Cde);
        selectQuery.append("'");
        std::cout << std::endl << selectQuery << std::endl;

        // 执行SELECT语句
        stmt = con->createStatement();
        res = stmt->executeQuery(selectQuery);
        // 输出查询结果
        while (res->next()) {
            std::cout << "User_Pass: " << res->getString("User_Pass") << std::endl;
        }
        // 释放资源
        delete res;
        delete stmt;
        delete con;
    }

    /*
    执行select语句, 判断表中是否存在用户
    */
    bool is_exist(std::string User_Cde) {
        bool is_exist = false;
        sql::mysql::MySQL_Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        // 创建MySQL连接
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(server, username, password);

        // 连接到特定的数据库
        con->setSchema(database);

        // 组合成select语句
        std::string selectQuery = "SELECT User_Pass FROM Users WHERE User_Cde='";
        selectQuery.append(User_Cde);
        selectQuery.append("'");
        std::cout << std::endl << selectQuery << std::endl;

        // 执行SELECT语句
        stmt = con->createStatement();
        res = stmt->executeQuery(selectQuery);
        // 输出查询结果
        if (res->next()) {
            std::cout << User_Cde << " is exist" << std::endl;
            is_exist = true;
        }
        else {
            std::cout << User_Cde << " is not exist" << std::endl;
        }

        // 释放资源
        delete res;
        delete stmt;
        delete con;

        // 返回结果
        return is_exist;

    }

    /*
    执行select语句,用户密码是否正确
    */
    std::string check_password(std::string User_Cde, std::string User_password) {
        std::string result = "ok";

        sql::mysql::MySQL_Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        // 创建MySQL连接
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(server, username, password);

        // 连接到特定的数据库
        con->setSchema(database);

        // 组合成select语句
        std::string selectQuery = "SELECT User_Pass FROM Users WHERE User_Cde='";
        selectQuery.append(User_Cde);
        selectQuery.append("'");
        std::cout << std::endl << selectQuery << std::endl;

        // 执行SELECT语句
        stmt = con->createStatement();
        res = stmt->executeQuery(selectQuery);
        // 输出查询结果
        if (res->next()) {
            std::cout << User_Cde << " is exist" << std::endl;
            std::string real_password = res->getString("User_Pass");
            if (!real_password._Equal(User_password)) {
                result = "Log in error!  Password error!\n";
                std::cout << "pass is not right" << std::endl;
            }
        }
        else {
            result = "Log in error!  The user is not exist!\n";
            std::cout << User_Cde << " is not exist" << std::endl;
        }

        // 释放资源
        delete res;
        delete stmt;
        delete con;

        // 返回结果
        return result;
    }

    /*
    执行无返回值，无参数的sql语句
    */
    void execute_sql(std::string sql_sentence) {
        sql::mysql::MySQL_Driver* driver;
        sql::Connection* con;
        sql::Statement* stmt;
        sql::ResultSet* res;

        // 创建MySQL连接
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(server, username, password);

        // 连接到特定的数据库
        con->setSchema(database);

        // 执行无返回值，无参数的sql语句
        stmt = con->createStatement();
        stmt->execute(sql_sentence);

        std::cout << std::endl << sql_sentence << std::endl;
        // 例如：
        // "CREATE TABLE Msg2 (User_Cde varchar(20), Msg varchar(20), WriteTime varchar(20))"
        // stmt->execute("CREATE TABLE NewTable (ID INT, Name VARCHAR(50))");
        // stmt->execute("ALTER TABLE Users ADD COLUMN Email VARCHAR(100)");
        // stmt->execute("DROP TABLE OldTable");
        //stmt->execute("DELETE FROM Users");
        // stmt->execute("DELETE FROM Msg");

        // 释放资源
        delete stmt;
        delete con;
    }

};

#endif // sql_H