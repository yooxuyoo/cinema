//头文件
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <stdbool.h>
#include <ctype.h>  
//宏
#define SESSION_COUNT 100 
#define HALL_FILE_PATH "D://2024//file//halls.txt" 
#define ADMIN_FILE_PATH "D://2024//file//admins.txt"
#define USER_FILE_PATH "D://2024//file//users.txt"
#define SESSION_FILE_PATH "D://2024//file//sessions.txt"
#define ORDER_FILE_PATH "D://2024//file//ticketOrder.txt"


// 定义结构体
struct Administrator {
    char adminID[11];    // 管理员ID，长度6-10字符
    char cinemaName[51]; // 所属影院名称
    char name[51];       // 管理员姓名
    char email[51];      // 管理员邮箱
    char password[51];   // 管理员密码
};

struct User {
    char userID[11]; // 用户ID，长度6-10字符
    char name[51];   // 用户姓名
    char gender;     // 性别
    char phone[12];  // 电话号码，11位数字
    char password[51];// 用户密码
    char email[51];  // 用户邮箱
    float balance;   // 账户余额
};

struct MovieSession {
    char sessionID[11]; // 场次号
    char movieName[51];// 影片名
    char cinemaName[51];// 所属影院
    int hallID;         // 影厅编号
    char startTime[6];  // 电影开始时间，格式hh:mm
    char endTime[6];    // 电影结束时间
    int duration;       // 电影时长，单位分钟
    int remainingSeats;// 场次余票数
    float ticketPrice;  // 场次票价
    char language[11]; // 放映类型，如英语、中文等
    float shangzuolv;  //场次上座率
    char typeofhall[11];//放映厅类型
    // 可能还有其他信息，如优惠活动等
};

struct Hall {
    int hallID;           // 影厅编号
    int totalSeats;       // 影厅总座位数
    // 影厅座位分布图和座位编号等信息可以根据需要添加
    char hallType[11];    // 影厅类型，如3D、4D、imax等
    char cinemaName[51];  // 所属影院名称
};

// 订单结构体
struct ticketOrder {
    char orderID[11];      // 订单ID
    char userID[11];       // 用户ID
    char sessionID[11];    // 场次号
    int seatabscissa1[1];  //座位号横坐标1  魏澄  
    int seatsordinate1[1]; //座位号纵坐标1 魏澄
    int seatabscissa2[1]; //座位号横坐标2  魏澄
    int seatsordinate2[1]; //座位号纵坐标2 魏澄
    int seatabscissa3[1];  //座位号横坐标3  魏澄 
    int seatsordinate3[1]; //座位号纵坐标3 魏澄
    float totalPrice;      // 订单价格
    char orderTime[20];    // 订单时间
    int isPaid;            // 是否已支付（1已支付，0未支付）魏澄
    int hallID;           // 影厅编号
    char cinemaName[51];  // 所属影院名称
};





//全局变量
int Uresult;                     //chw用户序号的全局变量
int Aresult;                     //chw管理员序号的全局变量
int sessionCount = 20;            //chw当前场次数量,初始化了20个，从20开始
int hallCount = 5;               // 魏澄 当前影厅数量
int userCount = 100;              // 魏澄 当前用户数量
int adminCount = 5;             // 魏澄 当前管理员数量
int orderCount = 3;             // 魏澄 当前订单数量
int filteredSessionCount = 0; // 筛选后场次数组中的元素数量


//结构体数组
struct User users[100];          // 用户数组
struct MovieSession sessions[SESSION_COUNT];// 电影场次数组
struct MovieSession filteredSessions[SESSION_COUNT]; // 筛选后的场次数组
struct Hall halls[100];            // 影厅数组
struct Administrator admins[5];    //管理员数组
struct ticketOrder orders[200];    //订单数组

// 函数声明
void saveAdminsToFile();//保存管理员信息到文件
void saveUsersToFile();//保存用户信息到文件
void saveSessionsToFile();//保存场次信息到文件
void saveOrdersToFile() ;//保存订单信息到文件
void saveHallsToFile();//保存影厅信息到文件
void saveDataToFile() ;//保存函数的封装
void loadUsersFromFile();//从文件读取用户信息
void loadAdminsFromFile();//从文件读取管理员信息
void loadSessionsFromFile();//从文件读取场次信息
void loadOrdersFromFile();//从文件读取订单信息
void loadHallsFromFile();//从文件读取影厅信息
void loadDataFromFile();//读取函数的封装
 void initializeAdministrators(struct Administrator admins[], int size);
 void initializeUsers(struct User users[], int size);
 void initializeHalls(struct Hall halls[], int size);
 void initializeMovieSessions(struct MovieSession sessions[], int size);
int userLogin();  //返回用户信息
int adminLogin();
void displayUserInfo(int a);//用户个人信息展示
void modifyUserInfo(int a);//用户个人信息修改
void searchMovie();//用户查询影片
void displayOrder();
void manageOrder();//管理员的订单管理
void addMovie();
void showMovies();
void manageHall(struct Hall halls[], struct MovieSession sessions[], int *hallCount, int sessionCount);//管理影厅
void showHalls(const struct Hall halls[], int size);
void addHall(struct Hall halls[], int *size);//添加影厅
void showHallInfo(const struct Hall *hall, const struct MovieSession sessions[], int sessionCount, const char* cinemaName);//展示指定影厅的场次信息
void manageSessions(struct MovieSession sessions[], int *sessionCount);    //场次管理函数
void showSessions();
void managePersonalInfo();
void querySessionsByAdminID(const char* adminID, struct Administrator admins[], int adminCount, struct MovieSession sessions[], int *sessionCount); //找到管理员相应的影院
void queryTodaySessions(struct MovieSession sessions[], int count, const char* input);//查询场次函数
void clearInputBuffer();//清空缓冲区
void addSession(struct MovieSession sessions[], int *sessionSize);//增加场次的函数
void filterSessionsByLanguage(struct MovieSession sessions[], int count, int choice);//根据语言筛选场次
void filterSessionsBytypeofhall(struct MovieSession sessions[], int count, int choice);//根据场次类型过滤
void buytickets();
void sortSessions();
void gobacktosearch();
void managePersonalInfo();//管理员个人信息管理
void modifymanageInfo(int A);//管理员信息修改
int getTicketsSold(const struct MovieSession *session, const struct ticketOrder orders[], int orderCount);//计算某个场次的已售票数
void sessionTicketSold(struct MovieSession sessions[], int sessionCount, struct ticketOrder orders[], int orderCount);//计算某个场次的已售票数
void sortSessionsByTicketsSold(struct MovieSession sessions[], int count);//按已售票数排序
int compareSessionsByTicketsSold(const struct MovieSession *session1, const struct MovieSession *session2);//比较已售票数的函数
void printSessionWithRank(struct MovieSession session, int rank);//打印场次信息，包括已售票数和排名
void sortAndPrintSessionsByTicketsSold(struct MovieSession sessions[], int sessionCount, struct ticketOrder orders[], int orderCount);//按已售票数排序并打印
void changePassword();//修改密码
bool isValidEmail(const char *email) {  
    const char *at = strchr(email, '@');  
    const char *dot = strchr(email, '.');  
    if (at == NULL || dot == NULL || dot <= at) {  
        return false;  
    }  
    // 这里可以添加更复杂的验证逻辑，比如检查域名部分是否有效  
    return true;  
}  
int compareStartTime(const char* time1, const char* time2);//比较开始时间的函数
void sortSessionsByStartTime(struct MovieSession sessions[], int count);//排序开始时间的函数
void printSession(struct MovieSession session);//打印场次函数
int compareTicketPrice(const struct MovieSession* session1, const struct MovieSession* session2);//比较票价的函数
void sortSessionsByTicketPrice(struct MovieSession sessions[], int count);//排序票价的函数
int compareRemainingSeats(const struct MovieSession* session1, const struct MovieSession* session2);//比较余票数的函数
void sortSessionsByRemainingSeats(struct MovieSession sessions[], int count);//排序余票数的函数



int main() {
    int userType;
    char input1_zym[102];//定义输入变量（用户输入影片名或者影院名）

    // 从文件中加载数据
    loadAdminsFromFile();
    loadUsersFromFile();
    loadSessionsFromFile();
    loadHallsFromFile();
    loadOrdersFromFile();
    sessionTicketSold(sessions, sessionCount, orders, orderCount);//计算已售票数并赋值给sessions
    // 初始化数据
    //魏澄initializeUsers(users, 100);
    //魏澄initializeAdministrators(admins, 5);
    //魏澄initializeHalls(halls, 20);
    //魏澄initializeMovieSessions(sessions, 20);
    //初始化空ID
    for(int i=20; i<100; i++){
        strcpy(sessions[i].sessionID, "");
    }
    while (1) {
        printf("请选择登录类型:1.用户 2.管理员\n");
        scanf("%d", &userType);
        clearInputBuffer();//清空缓冲区
        if (userType == 1) {
            Uresult = userLogin(); // 用户登录
            int userChoice;

            do {
                printf("\n用户功能:\n");
                printf("1. 查询影片\n2. 显示订单\n3. 查看个人信息\n4. 修改个人信息\n5. 注销\n");
                scanf("%d", &userChoice);
                clearInputBuffer();//清空缓冲区
                switch (userChoice) {
                    case 1:
                        
                        printf("请输入想要查询的影片名或者影院名（或者输入‘影片名 影院名’的格式）:\n");
                        fgets(input1_zym, sizeof(input1_zym), stdin);
                        input1_zym[strcspn(input1_zym, "\n")] = 0; // 去除换行符
                        queryTodaySessions(sessions, 20, input1_zym); 
                        break;
                    case 2:
                        displayOrder(); // 显示订单
                        break;
                    case 3:
                        displayUserInfo(Uresult); // 查看个人信息
                        break;
                    case 4:
                        modifyUserInfo(Uresult); // 修改个人信息
                        break;
                    case 5:
                        printf("注销中...\n");
                        break;
                    default:
                        printf("无效选择，请重试。\n");
                }
            } while (userChoice != 5);
            break; // 用户操作完成后退出循环
        } 
        
        
        else if (userType == 2) {
            Aresult = adminLogin(); // 管理员登录
            int adminChoice;

            do {
                printf("\n管理员功能:\n");
                printf("1. 订单管理\n2. 影厅管理\n3. 场次管理\n4. 个人信息管理\n5. 注销\n");
                scanf("%d", &adminChoice);

                switch (adminChoice) {
                    case 1:
                        manageOrder(); // 订单管理
                        break;
                    case 2:
                        manageHall(halls, sessions, &hallCount, sessionCount); // 影厅管理
                        break;
                    case 3:
                        manageSessions(sessions, &sessionCount); // 场次管理
                        break;
                    case 4:
                        managePersonalInfo(Aresult); // 个人信息管理
                        break;
                        break;
                    case 5:
                        printf("注销中...\n");
                        break;
                    default:
                        printf("无效选择，请重试。\n");
                }
            } while (adminChoice != 5);
            break; // 管理员操作完成后退出循环
        } else {
            printf("无效选择，请重试。\n");
        }
    }

    // 保存数据到文件
    saveDataToFile();


    return 0;
}

//存入管理员信息 魏澄
void saveAdminsToFile() {
    FILE *file = fopen(ADMIN_FILE_PATH, "w");
    if (file == NULL) {
        printf("无法打开管理员文件进行写入。\n");
        return;
    }
    for (int i = 0; i < adminCount; i++) {
        fprintf(file, "%s %s %s %s %s\n",
         admins[i].adminID,
         admins[i].cinemaName, 
         admins[i].name,
         admins[i].email,
         admins[i].password);
    }
    fclose(file);
}

//存入用户信息
void saveUsersToFile() {
    FILE *file = fopen(USER_FILE_PATH, "w");
    if (file == NULL) {
        printf("无法打开用户文件进行写入。\n");
        return;
    }
    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%s %s %c %s %s %s %.2f\n", 
                users[i].userID, 
                users[i].name, 
                users[i].gender,
                users[i].phone,
                users[i].password, 
                users[i].email, 
                users[i].balance);
    }
    fclose(file);
}

//存入场次信息
void saveSessionsToFile() {
    FILE *file = fopen(SESSION_FILE_PATH, "w");
    if (file == NULL) {
        printf("无法打开场次文件进行写入。\n");
        return;
    }
    for (int i = 0; i < sessionCount; i++) {
        printf("写入场次信息: %s %s %s %d %s %s %d %d %.2f %s %.2f %s\n", 
               sessions[i].sessionID, 
               sessions[i].movieName, 
               sessions[i].cinemaName,
               sessions[i].hallID, 
               sessions[i].startTime,
               sessions[i].endTime,
               sessions[i].duration,
               sessions[i].remainingSeats,
               sessions[i].ticketPrice,
               sessions[i].language, 
               sessions[i].shangzuolv, 
               sessions[i].typeofhall);
        fprintf(file, "%s %s %s %d %s %s %d %d %.2f %s %.2f %s %d\n", 
                sessions[i].sessionID, 
                sessions[i].movieName,
                sessions[i].cinemaName,
                sessions[i].hallID,
                sessions[i].startTime,
                sessions[i].endTime, 
                sessions[i].duration, 
                sessions[i].remainingSeats,
                sessions[i].ticketPrice, 
                sessions[i].language,
                sessions[i].shangzuolv, 
                sessions[i].typeofhall);
    }
    fclose(file);
}

//存入订单信息
void saveOrdersToFile() {
    FILE *file = fopen(ORDER_FILE_PATH, "w");
    if (file == NULL) {
        printf("无法打开订单文件进行写入。\n");
        return;
    }
    for (int i = 0; i < orderCount; i++) {
        fprintf(file, "%s %s %s %d %d %d %d %d %d %.2f %s %d %d %s\n", 
                orders[i].orderID,
                orders[i].userID, 
                orders[i].sessionID,
                orders[i].seatabscissa1[0], 
                orders[i].seatabscissa2[0], 
                orders[i].seatabscissa3[0],
                orders[i].seatsordinate1[0], 
                orders[i].seatsordinate2[0],
                orders[i].seatsordinate3[0],
                orders[i].totalPrice, 
                orders[i].orderTime,
                orders[i].isPaid,
                orders[i].hallID,
                orders[i].cinemaName);
    }
    fclose(file);
}

//存入影厅信息
void saveHallsToFile() {
    FILE *file = fopen(HALL_FILE_PATH, "w");
    if (file == NULL) {
        printf("无法打开影厅文件进行写入。\n");
        return;
    }
    for (int i = 0; i < hallCount; i++) {
        fprintf(file, "%d %d %s %s\n", 
                halls[i].hallID,
                halls[i].totalSeats, 
                halls[i].hallType,
                halls[i].cinemaName);
    }
    fclose(file);
}

//存入文件函数的封装
void saveDataToFile() {
    saveAdminsToFile();
    saveUsersToFile();
    saveSessionsToFile();
    saveHallsToFile();
    saveOrdersToFile();
}

//读取管理员信息
void loadAdminsFromFile(){
    FILE *file = fopen(ADMIN_FILE_PATH, "r");
    if (file == NULL) {
        printf("无法打开管理员文件进行读取。\n");
        return;
    }
    adminCount = 0;
    while (fscanf(file, "%10s %50s %50s %50s %50s", 
                  admins[adminCount].adminID,
                  admins[adminCount].cinemaName,
                  admins[adminCount].name,
                  admins[adminCount].email,
                  admins[adminCount].password) == 5) {
                     printf("读取管理员信息: %s %s %s %s %s\n", 
               admins[adminCount].adminID,
               admins[adminCount].cinemaName,
               admins[adminCount].name,
               admins[adminCount].email,
               admins[adminCount].password);
        adminCount++;
    }
    fclose(file);
}
//读取用户信息
void loadUsersFromFile(){
    FILE *file = fopen(USER_FILE_PATH,"r");
    if(file==NULL){
        printf("无法打开用户文件进行读取。\n");
        return;
    }
    userCount = 0;
    while (fscanf(file, "%10s %50s %c %11s %50s %50s %f", 
                  users[userCount].userID,
                   users[userCount].name, 
                   &users[userCount].gender,
                  users[userCount].phone, 
                  users[userCount].password,
                   users[userCount].email, 
                   &users[userCount].balance) != EOF) {
        userCount++;
    }
    fclose(file);
}

//读取场次信息
void loadSessionsFromFile() {
    FILE *file = fopen(SESSION_FILE_PATH, "r");
    if (file == NULL) {
        printf("无法打开场次文件进行读取。\n");
        return;
    }
    sessionCount = 0;
    while (fscanf(file, "%10s %50s %50s %d %5s %5s %d %d %f %10s %f %10s", 
                  sessions[sessionCount].sessionID, 
                  sessions[sessionCount].movieName, 
                  sessions[sessionCount].cinemaName,
                  &sessions[sessionCount].hallID, 
                  sessions[sessionCount].startTime,
                  sessions[sessionCount].endTime,
                  &sessions[sessionCount].duration,
                  &sessions[sessionCount].remainingSeats,
                  &sessions[sessionCount].ticketPrice,
                  sessions[sessionCount].language, 
                  &sessions[sessionCount].shangzuolv, 
                  sessions[sessionCount].typeofhall) == 12) {
        printf("读取场次信息: %s %s %s %d %s %s %d %d %.2f %s %.2f %s\n", 
               sessions[sessionCount].sessionID, 
               sessions[sessionCount].movieName, 
               sessions[sessionCount].cinemaName,
               sessions[sessionCount].hallID, 
               sessions[sessionCount].startTime,
               sessions[sessionCount].endTime,
               sessions[sessionCount].duration,
               sessions[sessionCount].remainingSeats,
               sessions[sessionCount].ticketPrice,
               sessions[sessionCount].language, 
               sessions[sessionCount].shangzuolv, 
               sessions[sessionCount].typeofhall);
        sessionCount++;
    }
    fclose(file);
}


//读取订单信息
void loadOrdersFromFile() {
    FILE *file = fopen(ORDER_FILE_PATH, "r");
    if (file == NULL) {
        printf("无法打开订单文件进行读取。\n");
        return;
    }
    orderCount = 0;
    while (fscanf(file, "%10s %10s %10s %d %d %d %d %d %d %f %19s %d %d %50s", 
                  orders[orderCount].orderID,
                  orders[orderCount].userID, 
                  orders[orderCount].sessionID,
                  &orders[orderCount].seatabscissa1[0], 
                  &orders[orderCount].seatabscissa2[0], 
                  &orders[orderCount].seatabscissa3[0],
                  &orders[orderCount].seatsordinate1[0], 
                  &orders[orderCount].seatsordinate2[0], 
                  &orders[orderCount].seatsordinate3[0],
                  &orders[orderCount].totalPrice, 
                  orders[orderCount].orderTime, 
                  &orders[orderCount].isPaid,
                  &orders[orderCount].hallID, 
                  orders[orderCount].cinemaName) == 14) {
        printf("读取订单信息: %s %s %s %d %d %d %d %d %d %.2f %s %d %d %s\n", 
               orders[orderCount].orderID,
               orders[orderCount].userID, 
               orders[orderCount].sessionID,
               orders[orderCount].seatabscissa1[0], 
               orders[orderCount].seatabscissa2[0], 
               orders[orderCount].seatabscissa3[0],
               orders[orderCount].seatsordinate1[0], 
               orders[orderCount].seatsordinate2[0], 
               orders[orderCount].seatsordinate3[0],
               orders[orderCount].totalPrice, 
               orders[orderCount].orderTime, 
               orders[orderCount].isPaid,
               orders[orderCount].hallID, 
               orders[orderCount].cinemaName);
        orderCount++;
    }
    fclose(file);
}

//读取影厅信息
void loadHallsFromFile() {
    FILE *file = fopen(HALL_FILE_PATH, "r");
    if (file == NULL) {
        printf("无法打开影厅文件进行读取。\n");
        return;
    }
    hallCount = 0;
    while (fscanf(file, "%d %d %10s %50s", 
                  &halls[hallCount].hallID, 
                  &halls[hallCount].totalSeats, 
                  halls[hallCount].hallType,
                  halls[hallCount].cinemaName) == 4) {
        printf("读取影厅信息: %d %d %s %s\n", 
               halls[hallCount].hallID, 
               halls[hallCount].totalSeats, 
               halls[hallCount].hallType,
               halls[hallCount].cinemaName);
        hallCount++;
    }
    fclose(file);
}


//读取文件函数的封装
//void loadDataFromFile() {
 //   loadAdminsFromFile();
 //   loadUsersFromFile();
 //   loadSessionsFromFile();
 //   loadHallsFromFile();
 //   loadOrdersFromFile();
//}

// 初始化管理员数组的函数
void initializeAdministrators(struct Administrator admins[], int size) {
    for (int i = 0; i < size; i++) {
        snprintf(admins[i].adminID, sizeof(admins[i].adminID), "ADM%06d", i);
        snprintf(admins[i].cinemaName, sizeof(admins[i].cinemaName), "Cinema%d", i);
        snprintf(admins[i].name, sizeof(admins[i].name), "AdminName%d", i);
        snprintf(admins[i].email, sizeof(admins[i].email), "admin%d@example.com", i);
        snprintf(admins[i].password, sizeof(admins[i].password), "password%d", i);
    }
}

// 初始化用户数组的函数
void initializeUsers(struct User users[], int size) {
    for (int i = 0; i < size; i++) {
        snprintf(users[i].userID, sizeof(users[i].userID), "U%06d", i);
        snprintf(users[i].name, sizeof(users[i].name), "UserName%d", i);
        users[i].gender = 'M'; // 假设所有用户都是男性
        snprintf(users[i].phone, sizeof(users[i].phone), "13800138%02d", i);
        snprintf(users[i].password, sizeof(users[i].password), "password%d", i);
        snprintf(users[i].email, sizeof(users[i].email), "user%d@example.com", i);
        users[i].balance = 100.0f; // 所有用户账户余额初始化为100.0元
    }
}

// 初始化影厅数组的函数
void initializeHalls(struct Hall halls[], int size) {
    for (int i = 0; i < size; i++) {
        halls[i].hallID = i + 1;
        halls[i].totalSeats = 100 + i * 50; // 假设座位数递增
        snprintf(halls[i].hallType, sizeof(halls[i].hallType), "Type%d", i);
        snprintf(halls[i].cinemaName, sizeof(halls[i].cinemaName), "Cinema%d", i % 5);
    }
}

// 初始化场次数组的函数
void initializeMovieSessions(struct MovieSession sessions[], int size) {
    for (int i = 0; i < size; i++) {
        snprintf(sessions[i].sessionID, sizeof(sessions[i].sessionID), "S%06d", i);
        snprintf(sessions[i].movieName, sizeof(sessions[i].movieName), "Movie%d", i % 4); // 假设有3个影片
        snprintf(sessions[i].cinemaName, sizeof(sessions[i].cinemaName), "Cinema%d", i % 5); // 假设有5个影院
        sessions[i].hallID = i % 10 + 1; // 假设每个影院有10个影厅
        snprintf(sessions[i].startTime, sizeof(sessions[i].startTime), "%02d:%02d", i % 24, 0);
        snprintf(sessions[i].endTime, sizeof(sessions[i].endTime), "%02d:%02d", (i % 24) + 2, 0); // 每场电影2小时
        sessions[i].duration = 120; // 电影时长2小时
        sessions[i].remainingSeats = 100 - i; // 假设余票数递减
        sessions[i].ticketPrice = 50 + i * 5; // 票价递增
        snprintf(sessions[i].language, sizeof(sessions[i].language), "Lang%d", i % 3); // 假设有三种语言
        if (strcmp(sessions[i].language, "Lang0") == 0)
        strcpy(sessions[i].language, "中文");
    else if (strcmp(sessions[i].language, "Lang1") == 0)
        strcpy(sessions[i].language, "英文");
    else if (strcmp(sessions[i].language, "Lang2") == 0)
        strcpy(sessions[i].language, "日语");
        if(sessions[i].hallID==1) strcpy(sessions[i].typeofhall, "2D");
        else if(sessions[i].hallID==2) strcpy(sessions[i].typeofhall, "2D");
        else if(sessions[i].hallID==3) strcpy(sessions[i].typeofhall, "3D");
        else if(sessions[i].hallID==4) strcpy(sessions[i].typeofhall, "3D");
        else if(sessions[i].hallID==5) strcpy(sessions[i].typeofhall, "3D");
        else if(sessions[i].hallID==6) strcpy(sessions[i].typeofhall, "2D");
        else if(sessions[i].hallID==7) strcpy(sessions[i].typeofhall, "2D");
        else if(sessions[i].hallID==8) strcpy(sessions[i].typeofhall, "4D");
        else if(sessions[i].hallID==9) strcpy(sessions[i].typeofhall, "imax");
        else if(sessions[i].hallID==10) strcpy(sessions[i].typeofhall, "imax");
    }
}

//用户登录函数
int userLogin() {
    printf("用户登录...\n");
    char inputUserID[11];
    char inputPassword[51];
    int chance = 3;
    int userFound = 0;
    for (int j = 1; j <= 3; j++) {
        printf("请输入用户ID: ");
        scanf("%s", inputUserID);
        printf("请输入密码: ");
        scanf("%s", inputPassword);

        int i_chw=0;
        for (int i_chw = 0; i_chw < 100; i_chw++) {
            if (strcmp(users[i_chw].userID, inputUserID) == 0 && strcmp(users[i_chw].password, inputPassword) == 0) {
                printf("用户登录成功！\n");
                userFound = 1;
                break;
            }
        }

        if (userFound) {
            return i_chw;
        } else {
            printf("用户登录失败,用户ID或密码错误,请重试,还剩%d次机会。\n", chance - j);
            if ((chance - j) == 0) {
                 printf("用户登录失败, 用户ID或密码错误\n");
                exit(0); // 登录失败，返回0
            }
        }
    }
    return 0;
}
//管理员登陆函数
int adminLogin() {
    printf("管理员登录...\n");
    char inputAdminID[11];
    char inputPassword[51];
    int chance = 3;
    int adminFound = 0;

    for (int j = 1; j <= 3; j++) {
        printf("请输入管理员ID: ");
        scanf("%s", inputAdminID);
        printf("请输入密码: ");
        scanf("%s", inputPassword);
        int i_adm;
        for ( i_adm=0; i_adm < 5; i_adm++) {
            if (strcmp(admins[i_adm].adminID, inputAdminID) == 0 && strcmp(admins[i_adm].password, inputPassword) == 0) {
                printf("管理员登录成功！\n");
                adminFound = 1;
                break;
            }
        }

        if (adminFound) {
            return i_adm;
            break;
        } else {
            printf("管理员登录失败,管理员ID或密码错误,请重试,还剩%d次机会。\n", chance - j);
            if ((chance - j) == 0) {
                printf("管理员登录失败,管理员ID或密码错误\n");
                exit(0);
            }
        }
    }
    return 0;
}

//选座购票函数
void buytickets() {
    printf("选座购票...\n");
}

//场次排序函数
void sortSessions() {
    printf("场次排序...\n");
}

//返回查询函数
void gobacktosearch() {
    printf("返回查询...\n");
}

// 查询场次的示例函数
void queryTodaySessions(struct MovieSession sessions[], int count, const char* input) {
    filteredSessionCount = 0;// 重置筛选数组的计数器
    printf("今天关于'%s'的场次有:\n", input);

    for (int i = 0; i < count; ++i) {
        char combined[102];
        snprintf(combined, sizeof(combined), "%s %s", sessions[i].movieName, sessions[i].cinemaName);

        if (strcmp(sessions[i].movieName, input) == 0 || strcmp(sessions[i].cinemaName, input) == 0 || strcmp(combined, input) == 0) {
            
            memcpy(&filteredSessions[filteredSessionCount++], &sessions[i], sizeof(struct MovieSession));// 复制当前场次到筛选数组
            
        }
    }

    if (filteredSessionCount == 0) {
        printf("当前提供信息未能查询到结果。\n");
    }
    else if(filteredSessionCount !=0){
         for (int i = 0; i < filteredSessionCount; ++i) {
       printf("场次 ID: %s, 影院: %s, 时间: %s-%s, 余票数: %d, 票价: %.2f, 语言: %s\n",
                   filteredSessions[i].sessionID, filteredSessions[i].cinemaName, filteredSessions[i].startTime, filteredSessions[i].endTime,
                   filteredSessions[i].remainingSeats, filteredSessions[i].ticketPrice, filteredSessions[i].language);
     }
    }


    int userChoice1_zym;
    int userChoicepaixv_zym;
    do {
        printf("1.选座购票\n2.场次筛选\n3.场次排序\n4.返回查询\n");
        scanf("%d", &userChoice1_zym);
        clearInputBuffer(); // 清空缓冲区

        switch (userChoice1_zym) {
            case 1:
                buytickets(); // 选座购票
                break;
            case 2: {
                // 场次筛选
                int userChoice2_zym;
                printf("请选择想要筛选的语言类型:\n1.中文\n2.英文\n3.日语\n");
                scanf("%d",&userChoice2_zym);
                clearInputBuffer(); // 清空缓冲区
                filterSessionsByLanguage(sessions, 20, userChoice2_zym);
                break;
            }
            case 3:
                 printf("请选择您需要的排序方式:\n1.时间（顺序）\n2.票价（从低到高）\n3.余票数（从多到少）\n");
                 scanf("%d",&userChoicepaixv_zym);
                 clearInputBuffer(); // 清空缓冲区
                 if(userChoicepaixv_zym==1){
                    sortSessionsByStartTime(filteredSessions, filteredSessionCount);
                    printf("根据开始时间顺序排序结果如下:\n");
                    for (int i = 0; i < filteredSessionCount; i++) {
                    printSession(filteredSessions[i]);
                    }
                    }
                else if(userChoicepaixv_zym==2){
                    sortSessionsByTicketPrice(filteredSessions, filteredSessionCount);
                    printf("根据票价（从低到高）排序结果如下n");
                    for (int i = 0; i < filteredSessionCount; i++) {
                        printSession(filteredSessions[i]);
                     }
                    }
                else if(userChoicepaixv_zym==3){
                     // 按照余票数从多到少排序场次信息
                     sortSessionsByRemainingSeats(filteredSessions, filteredSessionCount);
                     // 打印排序后的场次信息
                     printf("根据余票数（从多到少）排序结果如下:\n");
                     for (int i = 0; i < filteredSessionCount; i++) {
                     printSession(filteredSessions[i]);
                     }
                    }
                else printf("请选择合适的排序方式。\n");
                break;
            case 4:
                gobacktosearch(); // 返回查询
                break;
            default:
                printf("无效选择，请重试。\n");
        }
    } while (userChoice1_zym != 4);
}


// 比较两个时间字符串（格式为hh:mm）
int compareStartTime(const char* time1, const char* time2) {
    // 将字符串转换为小时和分钟
    int hour1 = (time1[0] - '0') * 10 + (time1[1] - '0');
    int hour2 = (time2[0] - '0') * 10 + (time2[1] - '0');
    int minute1 = (time1[3] - '0') * 10 + (time1[4] - '0');
    int minute2 = (time2[3] - '0') * 10 + (time2[4] - '0');

    // 比较小时和分钟
    if (hour1 != hour2) {
        return hour1 - hour2;
    } else {
        return minute1 - minute2;
    }
}

// 选择排序函数
void sortSessionsByStartTime(struct MovieSession sessions[], int count) {
    int i, j, minIndex;
    for (i = 0; i < count - 1; i++) {
        // 假设当前索引为最小
        minIndex = i;
        for (j = i + 1; j < count; j++) {
            if (compareStartTime(sessions[j].startTime, sessions[minIndex].startTime) < 0) {
                minIndex = j; // 找到更小的元素
            }
        }
        // 交换找到的最小元素与当前索引的元素
        struct MovieSession temp = sessions[i];
        sessions[i] = sessions[minIndex];
        sessions[minIndex] = temp;
    }
}


// 打印场次信息的函数
void printSession(struct MovieSession session) {
    printf("Session ID: %s\n", session.sessionID);
    printf("Movie Name: %s\n", session.movieName);
    printf("Cinema Name: %s\n", session.cinemaName);
    printf("Hall ID: %d\n", session.hallID);
    printf("Start Time: %s\n", session.startTime);
    printf("End Time: %s\n", session.endTime);
    printf("Duration: %d minutes\n", session.duration);
    printf("Remaining Seats: %d\n", session.remainingSeats);
    printf("Ticket Price: %.2f\n", session.ticketPrice);
    printf("Language: %s\n", session.language);
    printf("上座率: %0.2f\n", session.shangzuolv);
    printf("放映厅类型: %s\n", session.typeofhall);
    printf("\n");
}


// 比较两个场次的票价
int compareTicketPrice(const struct MovieSession* session1, const struct MovieSession* session2) {
    return session1->ticketPrice < session2->ticketPrice;
}

// 选择排序函数，按照票价从低到高排序
void sortSessionsByTicketPrice(struct MovieSession sessions[], int count) {
    int i, j, minIndex;
    for (i = 0; i < count - 1; i++) {
        minIndex = i;
        for (j = i + 1; j < count; j++) {
            if (!compareTicketPrice(&sessions[j], &sessions[minIndex])) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            struct MovieSession temp = sessions[i];
            sessions[i] = sessions[minIndex];
            sessions[minIndex] = temp;
        }
    }
}

// 比较两个场次的余票数
int compareRemainingSeats(const struct MovieSession* session1, const struct MovieSession* session2) {
    return session1->remainingSeats > session2->remainingSeats;
}

// 选择排序函数，按照余票数从多到少排序
void sortSessionsByRemainingSeats(struct MovieSession sessions[], int count) {
    int i, j, maxIndex;
    for (i = 0; i < count - 1; i++) {
        maxIndex = i;
        for (j = i + 1; j < count; j++) {
            if (compareRemainingSeats(&sessions[j], &sessions[maxIndex]) > 0) {
                maxIndex = j; // 找到余票数更多的会话
            }
        }
        if (maxIndex != i) {
            struct MovieSession temp = sessions[i];
            sessions[i] = sessions[maxIndex];
            sessions[maxIndex] = temp;
        }
    }
}
// 根据语言类型筛选场次的函数
void filterSessionsByLanguage(struct MovieSession sessions[], int count, int choice) {
    int found = 0;
    if(choice==1){ 
        printf("筛选结果，语言类型为'中文'的场次有:\n");
        for (int i = 0; i < filteredSessionCount; ++i) {
        if (strcmp(filteredSessions[i].language, "中文") == 0) {
            printf("场次ID: %s\n 影院: %s\n 时间: %s-%s\n 余票数: %d\n 票价: %.2f\n 语言: %s\n 影厅类型: %s\n\n",
                   filteredSessions[i].sessionID, filteredSessions[i].cinemaName, filteredSessions[i].startTime, filteredSessions[i].endTime,
                   filteredSessions[i].remainingSeats, filteredSessions[i].ticketPrice, filteredSessions[i].language,filteredSessions[i].typeofhall);
            found = 1;
        }
    }
    }
    else if(choice==2){ 
        printf("筛选结果，语言类型为'英文'的场次有:\n");
        for (int i = 0; i < filteredSessionCount; ++i) {
        if (strcmp(filteredSessions[i].language, "英文") == 0) {
            printf("场次ID: %s\n 影院: %s\n 时间: %s-%s\n 余票数: %d\n 票价: %.2f\n 语言: %s\n 影厅类型: %s\n\n",
                   filteredSessions[i].sessionID, filteredSessions[i].cinemaName, filteredSessions[i].startTime, filteredSessions[i].endTime,
                   filteredSessions[i].remainingSeats, filteredSessions[i].ticketPrice, filteredSessions[i].language,filteredSessions[i].typeofhall);
            found = 1;
        }
    }
    }
    else if(choice==3){ 
        printf("筛选结果，语言类型为'日语'的场次有:\n");
        for (int i = 0; i < filteredSessionCount; ++i) {
        if (strcmp(filteredSessions[i].language, "日语") == 0) {
            printf("场次ID: %s\n 影院: %s\n 时间: %s-%s\n 余票数: %d\n 票价: %.2f\n 语言: %s\n 影厅类型: %s\n\n",
                   filteredSessions[i].sessionID, filteredSessions[i].cinemaName, filteredSessions[i].startTime, filteredSessions[i].endTime,
                   filteredSessions[i].remainingSeats, filteredSessions[i].ticketPrice, filteredSessions[i].language,filteredSessions[i].typeofhall);
            found = 1;
        }
    }
    }
    if (!found) {
        printf("没有找到指定语言类型的场次。\n");
    }
}

// 根据放映厅类型筛选场次的函数
void filterSessionsBytypeofhall(struct MovieSession sessions[], int count, int choice) {
    int found = 0;
    if(choice==1){ 
        printf("筛选结果，语言类型为'2D'的场次有:\n");
        for (int i = 0; i < filteredSessionCount; ++i) {
        if (strcmp(filteredSessions[i].typeofhall, "2D") == 0) {
            printf("场次ID: %s\n 影院: %s\n 时间: %s-%s\n 余票数: %d\n 票价: %.2f\n 语言: %s\n 影厅类型: %s\n\n",
                   filteredSessions[i].sessionID, filteredSessions[i].cinemaName, filteredSessions[i].startTime, filteredSessions[i].endTime,
                   filteredSessions[i].remainingSeats, filteredSessions[i].ticketPrice, filteredSessions[i].language,filteredSessions[i].typeofhall);
            found = 1;
        }
    }
    }
    else if(choice==2){ 
        printf("筛选结果，语言类型为'3D'的场次有:\n");
        for (int i = 0; i < filteredSessionCount; ++i) {
        if (strcmp(filteredSessions[i].typeofhall, "3D") == 0) {
            printf("场次ID: %s\n 影院: %s\n 时间: %s-%s\n 余票数: %d\n 票价: %.2f\n 语言: %s\n 影厅类型: %s\n\n",
                   filteredSessions[i].sessionID, filteredSessions[i].cinemaName, filteredSessions[i].startTime, filteredSessions[i].endTime,
                   filteredSessions[i].remainingSeats, filteredSessions[i].ticketPrice, filteredSessions[i].language,filteredSessions[i].typeofhall);
            found = 1;
        }
    }
    }
    else if(choice==3){ 
        printf("筛选结果，语言类型为'4D'的场次有:\n");
        for (int i = 0; i < filteredSessionCount; ++i) {
        if (strcmp(filteredSessions[i].typeofhall, "4D") == 0) {
            printf("场次ID: %s\n 影院: %s\n 时间: %s-%s\n 余票数: %d\n 票价: %.2f\n 语言: %s\n 影厅类型: %s\n\n",
                   filteredSessions[i].sessionID, filteredSessions[i].cinemaName, filteredSessions[i].startTime, filteredSessions[i].endTime,
                   filteredSessions[i].remainingSeats, filteredSessions[i].ticketPrice, filteredSessions[i].language,filteredSessions[i].typeofhall);
            found = 1;
        }
    }
    }
    else if(choice==4){ 
        printf("筛选结果，语言类型为'imax'的场次有:\n");
        for (int i = 0; i < filteredSessionCount; ++i) {
        if (strcmp(filteredSessions[i].typeofhall, "imax") == 0) {
            printf("场次ID: %s\n 影院: %s\n 时间: %s-%s\n 余票数: %d\n 票价: %.2f\n 语言: %s\n 影厅类型: %s\n\n",
                   filteredSessions[i].sessionID, filteredSessions[i].cinemaName, filteredSessions[i].startTime, filteredSessions[i].endTime,
                   filteredSessions[i].remainingSeats, filteredSessions[i].ticketPrice, filteredSessions[i].language,filteredSessions[i].typeofhall);
            found = 1;
        }
    }
    }
    if (!found) {
        printf("没有找到指定放映厅类型的场次。\n");
    }
}




//找到相应的电影院的所有场次函数
void querySessionsByAdminID(const char* adminID, struct Administrator admins[], int adminCount, struct MovieSession sessions[], int *sessionCount) {

    char cinemaName[51];
    for (int i = 0; i < adminCount; i++) {
        if (strcmp(adminID, admins[i].adminID) == 0) {
            strcpy(cinemaName, admins[i].cinemaName);
            break;
        }
    }
    printf("影院场次: %s\n", cinemaName);
    for (int i = 0; i < *sessionCount; i++) {
        if (strcmp(cinemaName, sessions[i].cinemaName) == 0) {
            printf("场次ID: %s\n", sessions[i].sessionID);
            printf("影厅ID: %d\n", sessions[i].hallID);
            printf("开始时间: %s\n", sessions[i].startTime);
            printf("结束时间: %s\n", sessions[i].endTime);
            printf("持续时间: %d minutes\n", sessions[i].duration);
            printf("剩余时间: %d\n", sessions[i].remainingSeats);
            printf("票价: %.2f\n", sessions[i].ticketPrice);
            printf("语言: %s\n\n", sessions[i].language);
        }
    }
    return;
}
//个人信息显示
void displayUserInfo(int Uresult){
    printf("显示用户信息...\n");
    int n=0;
    int a=Uresult;
    printf("用户ID: %s\n", users[a].userID);
    printf("用户名: %s\n", users[a].name);
    printf("性别: %c\n", users[a].gender);
    printf("电话: %s\n", users[a].phone);
    printf("邮箱: %s\n", users[a].email);
    printf("余额: %.2f\n", users[a].balance);
    printf("1.编辑信息\t2.订单信息\t3.返回\n");
    scanf("%d",&n);
    switch(n){
        case 1:
        modifyUserInfo(a);
        case 2:
        displayOrder(a);
        case 3:
        return;
    }
}
//用户个人信息修改
void modifyUserInfo(int Uresult) {
    int b=0;
    int a=Uresult;
    int j;
    char inputPass[51];
    char email[101]; // 假设电子邮件地址不会超过100个字符   
    printf("修改用户信息...\n");
    printf("请选择需要修改的信息\n");
    printf("1.用户名\n2.性别\n3.电话\n4.邮箱\n5.密码\n6.返回");
    scanf("%d",&b);
    clearInputBuffer();
    switch (b) {
        case 1:
            printf("当前用户名: %s\n", users[a].name);
            printf("请输入新的用户名 (最多50个字符)：");
            scanf("%50s", users[a].name);
            clearInputBuffer();
            printf("修改后用户名: %s\n", users[a].name);
            saveUsersToFile();
            break;
        case 2:
            printf("当前性别: %c\n", users[a].gender);  
            printf("请输入您的性别 (M 或 F)：");  
            while (1) {
                scanf(" %c", &users[a].gender); 
                clearInputBuffer();
                if (users[a].gender == 'M' || users[a].gender == 'F') {  
                    printf("修改后Gender: %c\n", users[a].gender); 
                    saveUsersToFile(); 
                    break;
                } else {  
                    printf("错误：请输入'M'或'F'。\n");  
                    clearInputBuffer();
                    while (getchar() != '\n');  
                }  
            }  
            break;
        case 3:  
            printf("当前Phone: %s\n", users[a].phone);  
            printf("请输入新的Phone (11位数字)：");  
            char newPhone[12];
            if (scanf("%11s", newPhone) == 1) {  
                if (strlen(newPhone) == 11 && isdigit((unsigned char)newPhone[0])) { 
                    for (int i = 1; i < 11; i++) {
                        if (!isdigit((unsigned char)newPhone[i])) {  
                            printf("输入错误，电话号码必须全部为数字。\n");  
                            return; 
                        }  
                    }  
                    strcpy(users[a].phone, newPhone); // 如果检查通过，则更新电话号码  
                    printf("修改后Phone: %s\n", users[a].phone);  
                    saveUsersToFile();
                } else {  
                    printf("输入错误，电话号码必须是11位数字。\n");  
        }  
    }  
    break;  
        case 4:
            printf("当前邮箱: %s：\n", users[Uresult].email);
            printf("请输入新的邮箱(最多50个字符)");
            scanf("%100s", email); 
            if (isValidEmail(email)) {  
                printf("电子邮件地址有效。\n");  
                strcpy(users[Uresult].email, email); 
                printf("修改后邮箱: %s：\n", users[Uresult].email);
                saveUsersToFile();
            } else {  
                printf("电子邮件地址无效。\n");  
            }  
            break;
        case 5:
        changePassword(0, Uresult);
        break;
        default:
            printf("无效选择，请重试。\n");
            displayUserInfo(Uresult);
    }
}

void searchMovie() {
    printf("搜索查询影片...\n");
    printf("请输入想要查询的影片名或者影院名（或者输入“影片名 影院名”的格式）:\n");
    // printf("按回车键返回上一步\n");
    // getchar();
    // if (getchar() != '\n') {
    //      return;
    //  }
}

void displayOrder() {
    printf("显示订单...\n");
}

void manageOrder() {
    //订单管理
   char userID[11];
   int choice, sortChoice=0;
   int orderCount = 0;
   
    do{ 
        printf("1. 查看用户订单情况\n");
        printf("2. 统计用户订单信息\n");
        printf("3. 返回\n");
        scanf("%d", &choice);
        clearInputBuffer(); 

        switch (choice) {
            case 1:
            
            printf("请输入用户ID: ");
            scanf("%10s", userID);
            
            //从文件中读取订单信息
                FILE *file = fopen("D://2024//file//ticketOrder.txt", "r");
                char line[1024];
                while (fgets(line, sizeof(line), file) && orderCount < 200) {
        if (sscanf(line, "%10s %10s %10s %d %d %d %d %d %d %f %19s %d %d %50s",
                   orders[orderCount].orderID,
                   orders[orderCount].userID,
                   orders[orderCount].sessionID,
                   &orders[orderCount].seatabscissa1[0],
                   &orders[orderCount].seatsordinate1[0],
                   &orders[orderCount].seatabscissa2[0],
                   &orders[orderCount].seatsordinate2[0],
                   &orders[orderCount].seatabscissa3[0],
                   &orders[orderCount].seatsordinate3[0],
                   &orders[orderCount].totalPrice,
                   orders[orderCount].orderTime,
                   &orders[orderCount].isPaid,
                   &orders[orderCount].hallID,
                   orders[orderCount].cinemaName) == 14) {
            orderCount++;
        }
    }

    fclose(file);
                
                //打印指定用户ID的所有订单信息
                int userFound = 0;
    for (int i = 0; i < orderCount; i++) {
        if (strcmp(orders[i].userID, userID) == 0) {
            printf("订单ID: %s\n", orders[i].orderID);
            printf("用户ID: %s\n", orders[i].userID);
            printf("场次号: %s\n", orders[i].sessionID);
            printf("座位坐标1: (%d, %d)\n", orders[i].seatabscissa1[0], orders[i].seatsordinate1[0]);
            printf("座位坐标2: (%d, %d)\n", orders[i].seatabscissa2[0], orders[i].seatsordinate2[0]);
            printf("座位坐标3: (%d, %d)\n", orders[i].seatabscissa3[0], orders[i].seatsordinate3[0]);
            printf("订单总价: %.2f\n", orders[i].totalPrice);
            printf("订单时间: %s\n", orders[i].orderTime);
            printf("是否已支付: %s\n", orders[i].isPaid ? "已支付" : "未支付");
            printf("影厅编号: %d\n", orders[i].hallID);
            printf("所属影院名称: %s\n\n", orders[i].cinemaName);
            userFound = 1;
        }
    }

    if (!userFound) {
        printf("没有找到用户ID为 %s 的订单。\n", userID);
    }

                    
                break;

            case 2:
                //统计用户订单情况
            printf("请选择排序方式:\n");
            printf("1. 按已购票人数排序\n");
            printf("2. 按场次上座率排序\n");
            printf("3. 按总票价收入排序\n");
            scanf("%d", &sortChoice); 

           switch (sortChoice) {
            case 1:
            sortAndPrintSessionsByTicketsSold(sessions, sessionCount, orders, orderCount);
            break;
            case 2:
            //按场次排序（按日期分区段进行排序）

            break;
            case 3:
            //按总票价收入排序（按日期分区段进行排序）

            //计算总票价
            break;
        default:
            printf("无效选项，请重新选择。\n");
            break;
            }
         break;
            case 3:
            printf("返回管理员功能菜单。\n");
                break;
            default:
                printf("输入错误\n");
                break;
        }
    }while(choice!=3);

}


void showMovies() {
    printf("显示影片...\n");
}



//影厅管理wc
// 管理影厅的函数
void manageHall(struct Hall halls[], struct MovieSession sessions[], int *hallCount, int sessionCount) {
    int choice;
    const char* cinemaName_chw = admins[Aresult].cinemaName;
    do {
        printf("\n影厅管理功能:\n");
        printf("1. 查询影厅\n2. 添加影厅\n3. 返回\n");
        printf("请选择操作: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // 显示所有影厅信息并允许查看详细信息
                // 这里我们假设管理员已经登录，并且Aresult变量保存了管理员的索引
                // 首先获取管理员所在影院的名称
                printf("影院 '%s' 的影厅信息:\n", cinemaName_chw);

                int found = 0;
                for (int i = 0; i < *hallCount; i++) {
                    if (strcmp(halls[i].cinemaName, cinemaName_chw) == 0) {
                        printf("影厅ID: %d\n", halls[i].hallID);
                        printf("总座位数: %d\n", halls[i].totalSeats);
                        printf("影厅类型: %s\n\n", halls[i].hallType);
                        found = 1;
                    }
                }

                if (!found) {
                    printf("影院 '%s' 没有找到影厅信息。\n", cinemaName_chw);
                }
                break;
            case 2:
                addHall(halls, hallCount); // 添加新的影厅
                break;
            case 3:
                printf("返回管理员功能菜单。\n");
                break;
            default:
                printf("输入无效，请重试。\n");
                break;
        }
    } while (choice != 3);
}
// 显示指定影厅的详细信息
void showHallInfo(const struct Hall *hall, const struct MovieSession sessions[], int sessionCount, const char* cinemaName) {
    printf("\n影厅场次信息:\n");
    printf("影厅ID: %d\n", hall->hallID);
    printf("总座位数: %d\n", hall->totalSeats);
    printf("影厅类型: %s\n", hall->hallType);

    int found = 0;
    for (int i = 0; i < sessionCount; i++) {
        if (strcmp(sessions[i].cinemaName, cinemaName) == 0 && sessions[i].hallID == hall->hallID) {
            printf("场次ID: %s\n", sessions[i].sessionID);
            printf("影片名: %s\n", sessions[i].movieName);
            printf("开始时间: %s\n", sessions[i].startTime);
            printf("结束时间: %s\n", sessions[i].endTime);
            printf("持续时间: %d 分钟\n", sessions[i].duration);
            printf("剩余座位: %d\n", sessions[i].remainingSeats);
            printf("票价: %.2f\n", sessions[i].ticketPrice);
            printf("语言: %s\n\n", sessions[i].language);
            found = 1;
        }
    }

    if (!found) {
        printf("该影厅当前没有安排任何场次。\n");
    }
}

//显示所有影厅信息
void showHalls(const struct Hall halls[], int size) {
     printf("当前影厅信息:\n");
    for (int i = 0; i < size; i++) {
        printf("影厅ID: %d\n", halls[i].hallID);
        printf("总座位数: %d\n", halls[i].totalSeats);
        printf("影厅类型: %s\n\n", halls[i].hallType);
    }
}

// 添加新的影厅
void addHall(struct Hall halls[], int *size) {
    if (*size >= 100) {
        printf("影厅数量达到上限，无法添加新影厅。\n");
        return;
    }

    struct Hall newHall;
    printf("请输入影厅ID: ");
    scanf("%d", &newHall.hallID);
    printf("请输入总座位数: ");
    scanf("%d", &newHall.totalSeats);
    printf("请输入影厅类型: ");
    scanf("%s", newHall.hallType);
    scanf("%50s", newHall.cinemaName); // 确保影院名称不超过50个字符

    halls[(*size)++] = newHall; // 添加新的影厅并更新数量
    printf("影厅已添加。\n");

    // 保存影厅数据到文件 魏澄
    saveHallsToFile();
}

//场次管理chw
//场次管理函数声明
void manageSessions(struct MovieSession sessions[], int *sessionCount) {
    printf("场次管理...\n");
    printf("1. 查询场次\n2. 添加场次\n3. 返回主菜单\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            querySessionsByAdminID(admins[Aresult].adminID, admins, 5, sessions, sessionCount); //如后续需要添加管理员，这里需要指针
            break;
        case 2:
            addSession(sessions, sessionCount);
            break;
        case 3:
            return;
        default:
            printf("无效选择，请重试。\n");
    }
}
//添加场次的函数
void addSession(struct MovieSession sessions[], int *sessionSize) {
    if (*sessionSize >= 100) {
        printf("场次数量达到上限，无法添加新影厅。\n");
        return;
    }
    
    struct MovieSession newSession; //用来添加场次的结构体
    printf("添加场次...\n");
    printf("请输入场次ID (6-10字符): ");
    scanf("%10s", newSession.sessionID);
    printf("请输入影片名 (最多50个字符): ");
    scanf("%50s", newSession.movieName);
    printf("请输入影院名 (最多50个字符): ");
    scanf("%50s", newSession.cinemaName);
    printf("请输入影厅ID: ");
    scanf("%d", &newSession.hallID);
    printf("请输入开始时间 (格式hh:mm): ");
    scanf("%5s", newSession.startTime);
    printf("请输入结束时间 (格式hh:mm): ");
    scanf("%5s", newSession.endTime);
    printf("请输入电影时长 (分钟): ");
    scanf("%d", &newSession.duration);
    printf("请输入场次余票数: ");
    scanf("%d", &newSession.remainingSeats);
    printf("请输入票价: ");
    scanf("%f", &newSession.ticketPrice);
    printf("请输入放映类型 (最多10个字符): ");
    scanf("%10s", newSession.typeofhall);
    printf("请选择影片的语言类型:\n1.中文\n2.英文\n3.日语\n请选择合适的语言类型。\n");
    int languageChoice;
    scanf("%d", &languageChoice);
    switch (languageChoice) {
        case 1:
            strcpy(newSession.language, "中文");
            break;
        case 2:
            strcpy(newSession.language, "英文");
            break;
        case 3:
            strcpy(newSession.language, "日语");
            break;
        default:
            strcpy(newSession.language, "未知");
            break;
    }
    newSession.shangzuolv = 0.0f; // 初始化上座率为0

    sessions[(*sessionSize)++] = newSession; // 添加新的影厅并更新数量
    saveSessionsToFile();
    printf("场次已添加。\n");
}

//管理员信息显示
void managePersonalInfo(int Aresult) {
    printf("个人信息管理...\n");
    int n=0;
    int a=Aresult;
    printf("管理员ID: %s\n", admins[a].adminID);
    printf("管理员姓名: %s\n", admins[a].name); 
    printf("所属影院名称: %s\n", admins[a].cinemaName);
    printf("邮箱: %s\n", admins[a].email);
    printf("1.编辑信息\t2.返回");
    scanf("%d",&n);
    if(n == 1){
        modifymanageInfo(Aresult);
    }else if(n == 2){
        return;
    }
}

//管理员信息修改
void modifymanageInfo(int Aresult){
    int b=0;
    int a=Aresult;
    int j;
    char email[101];   
    char inputPass[51];
    printf("修改用户信息...\n");
    printf("请选择需要修改的信息\n");
    printf("1.管理员姓名\n2.所属影院名称\n3.邮箱\n4.密码");
    scanf("%d",&b);
    clearInputBuffer();
    switch (b) {
        case 1:
            printf("当前管理员姓名: %s\n", admins[a].name);
            printf("请输入新的管理员姓名：");
            scanf("%50s", admins[a].name);
            clearInputBuffer();
            printf("修改后管理员姓名: %s\n", admins[a].name);
            saveAdminsToFile();
            break;
        case 2:
            printf("当前所属影院名称: %s\n", admins[a].cinemaName);
            printf("请输入新的所属影院名称(最多50字符)：");
            scanf("%50s", &admins[a].cinemaName); 
            clearInputBuffer();
            printf("修改后所属影院名称: %s\n", admins[a].cinemaName);
            saveAdminsToFile();
            break;
        case 3:
            printf("当前邮箱: %s@example.com：\n", admins[a].email);
            printf("请输入新的邮箱(最多50个字符)");
            scanf("%100s", email); 
            clearInputBuffer();
            if (isValidEmail(email)) {  
                printf("电子邮件地址有效。\n");  
                strcpy(admins[a].email, email); 
                printf("修改后邮箱: %s：\n", admins[a].email);
                saveAdminsToFile();
            } else {  
                printf("电子邮件地址无效。\n");  
            }  
            break;
        case 4:
            changePassword(1, a);
            saveAdminsToFile();
            break;
        default:
            printf("无效选择，请重试。\n");
            displayUserInfo(a);
    }
}

//计算某个场次的已售票数
int getTicketsSold(const struct MovieSession *session, const struct ticketOrder orders[], int orderCount) {
    int ticketsSold = 0;
    for (int i = 0; i < orderCount; i++) {
        if (strcmp(session->sessionID, orders[i].sessionID) == 0) {
            ticketsSold++;
        }
    }
    return ticketsSold;
}

void sessionTicketSold(struct MovieSession sessions[], int sessionCount, struct ticketOrder orders[], int orderCount) {
    for (int i = 0; i < sessionCount; i++) {
        sessions[i].remainingSeats = 100 - getTicketsSold(&sessions[i], orders, orderCount);
    }
}

//修改密码函数
void changePassword(int isAdmin, int index) {
    char inputPass[51];
    char newPassword[51];
    int loginFailed = 0;
    int chance = 3;
    if (isAdmin) {
            for (int j = 1; j <= 3; j++) {  
            printf("请输入密码: ");  
            scanf("%50s", inputPass); 
            if (strcmp(admins[index].password, inputPass) == 0) { 
                loginFailed =0; 
                printf("登录成功！\n");  
                printf("当前密码: %s\n", admins[index].password);  
                printf("请输入新的密码:(最多50个字符)：");  
                scanf("%50s", admins[index].password);
                printf("修改后Password: %s\n", admins[index].password);
                break; // 成功登录后退出循环  
            } else {  
                loginFailed = 1; // 标记为登录失败  
                printf("登录失败,密码错误,请重试,还剩%d次机会。\n", chance - j);  
            }  
        }  
            if (loginFailed ==1 ) { // 如果登录失败并且已经尝试了3次  
            printf("登录失败，管理员密码错误\n");  
            printf("系统将冷却1分钟...\n");  
            sleep(60); // 暂停60秒
            printf("冷却时间结束，您可以再次尝试。\n");  
            return;
}

    } else {
            for (int j = 1; j <= 3; j++) {  
            printf("请输入密码: ");  
            scanf("%50s", inputPass); 
            if (strcmp(users[index].password, inputPass) == 0) { 
                loginFailed =0; 
                printf("登录成功！\n");  
                printf("当前密码: %s\n", users[index].password);  
                printf("请输入新的密码: (最多50个字符)：");  
                scanf("%50s", users[index].password);
                printf("修改后密码: %s\n", users[index].password);
                saveAdminsToFile();
                break; // 成功登录后退出循环  
            } else {  
                loginFailed = 1; // 标记为登录失败  
                printf("登录失败,密码错误,请重试,还剩%d次机会。\n", chance - j);  
            }  
        }  
            if (loginFailed ==1 ) { // 如果登录失败并且已经尝试了3次  
            printf("登录失败，用户密码错误\n");  
            printf("系统将冷却1分钟...\n");  
            sleep(60); // 暂停60秒
            printf("冷却时间结束，您可以再次尝试。\n");  
            return;
        }   
    }
}



// 比较两个场次的已售票数
int compareSessionsByTicketsSold(const struct MovieSession *session1, const struct MovieSession *session2) {
    return session1->remainingSeats - session2->remainingSeats;
}

// 冒泡排序函数，按照已售票数从多到少排序场次数组
void sortSessionsByTicketsSold(struct MovieSession sessions[], int count) {
    int i, j;
    struct MovieSession temp;
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            if (compareSessionsByTicketsSold(&sessions[j], &sessions[j + 1]) > 0) {
                // 交换已售票数较多的场次到前面
                temp = sessions[j];
                sessions[j] = sessions[j + 1];
                sessions[j + 1] = temp;
            }
        }
    }
}

// 打印场次信息，包括已售票数和排名
void printSessionWithRank(struct MovieSession session, int rank) {
    printf("排名: %d, 场次ID: %s, 已售票数: %d\n", rank, session.sessionID, session.remainingSeats);
}

void sortAndPrintSessionsByTicketsSold(struct MovieSession sessions[], int sessionCount, struct ticketOrder orders[], int orderCount) {
    // 计算每个场次的已售票数
    for (int i = 0; i < sessionCount; i++) {
        sessions[i].remainingSeats = getTicketsSold(&sessions[i], orders, orderCount);
    }

    // 对场次数组按已售票数进行排序
    sortSessionsByTicketsSold(sessions, sessionCount);

    // 打印排序后的场次信息
    for (int i = 0; i < sessionCount; i++) {
        printSessionWithRank(sessions[i], i + 1);
    }
}
//清空缓冲区
void clearInputBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}